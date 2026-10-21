/*********************************************************************************************************************
* 文件名称          vofa.c
* 功能简介          VOFA+ 上位机串口通信：JustFloat 波形上传 + ASCII 在线调参
*
* *************************** 一帧数据长什么样 ***************************
*   4 个 float（每个 4 字节，小端） + 4 字节帧尾 00 00 80 7F，共 20 字节。
*   TriCore 是小端、float 是 IEEE754 单精度，所以直接把 float 数组 memcpy 进发送缓冲就行，
*   不需要自己拆字节。
*
* *************************** 为什么收发都在这里 ***************************
*   发送：vofa_update() 里按 20ms 分频，填通道 -> 组帧 -> uart_write_buffer()。
*   接收：串口中断里只做"把字节塞进环形缓冲"，真正的解析放到主循环 vofa_update() 里，
*         这样中断里不会跑 sscanf，也就不会因为一条长命令把控制周期拖乱。
*
* *************************** 环形缓冲的并发 ***************************
*   中断只改 head，主循环只改 tail，单生产者/单消费者，不需要关中断。
*   缓冲满时丢新字节（说明上位机发得比解析快，正常人手打不会发生）。
*********************************************************************************************************************/

#include "vofa.h"
#include "motor.h"
#include "track.h"
#include "image.h"
#include "imu.h"

//==================================================== 内部参数 ====================================================
#define VOFA_RX_BUF_SIZE    (128)                       // 接收环形缓冲大小
#define VOFA_LINE_MAX       (64)                        // 一条命令最长多少字符

//==================================================== 内部变量 ====================================================
static float32      vofa_data[VOFA_CH_NUM];             // 本帧要发的 12 个通道
static uint8        vofa_tx_buf[VOFA_CH_NUM * 4 + 4];   // 组好的整帧（含帧尾）
static const uint8  vofa_tail[4] = {0x00, 0x00, 0x80, 0x7F};    // JustFloat 帧尾

static volatile uint8  vofa_rx_buf[VOFA_RX_BUF_SIZE];   // 中断写、主循环读
static volatile uint16 vofa_rx_head = 0;                // 中断改
static volatile uint16 vofa_rx_tail = 0;                // 主循环改

static char   vofa_line[VOFA_LINE_MAX];                 // 正在拼的一行命令
static uint8  vofa_line_len = 0;

static uint32 vofa_last_ms = 0;                         // 上一帧发送时刻

//==================================================== 内部函数 ====================================================
//-------------------------------------------------------------------------------------------------------------------
// 函数简介      把收到的字节塞进环形缓冲（中断里调用）
//-------------------------------------------------------------------------------------------------------------------
void vofa_uart_callback (void)
{
    uint8  dat;
    uint16 next;

    while(uart_query_byte(VOFA_UART, &dat))
    {
        next = (uint16)((vofa_rx_head + 1) % VOFA_RX_BUF_SIZE);
        if(next != vofa_rx_tail)                        // 没满才存
        {
            vofa_rx_buf[vofa_rx_head] = dat;
            vofa_rx_head = next;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      执行一条 ASCII 命令
// 参数说明      line            以 0 结尾的一行，比如 "p 2.0 0.5 0"
// 备注信息      解析失败/参数不够就静默忽略；具体限幅在各 set 函数里
//-------------------------------------------------------------------------------------------------------------------
static void vofa_exec (char *line)
{
    char  cmd[16];
    float a = 0.0f;
    float b = 0.0f;
    float c = 0.0f;
    int   n;
    uint8 i;

    n = sscanf(line, "%15s %f %f %f", cmd, &a, &b, &c);
    if(n < 1)
    {
        return;
    }

    // 命令转小写，这样 VOFA+ 里大小写都能用
    for(i = 0; 0 != cmd[i]; i++)
    {
        if((cmd[i] >= 'A') && (cmd[i] <= 'Z'))
        {
            cmd[i] = (char)(cmd[i] + ('a' - 'A'));
        }
    }

    if((0 == strcmp(cmd, "p")) && (n >= 4))
    {
        motor_set_pid(1, a, b, c);
        motor_set_pid(2, a, b, c);
    }
    else if((0 == strcmp(cmd, "p1")) && (n >= 4))
    {
        motor_set_pid(1, a, b, c);
    }
    else if((0 == strcmp(cmd, "p2")) && (n >= 4))
    {
        motor_set_pid(2, a, b, c);
    }
    else if((0 == strcmp(cmd, "kp")) && (n >= 2))       // 只改 Kp，Ki/Kd 和积分都不动
    {
        motor_set_pid_term(1, 0, a);
        motor_set_pid_term(2, 0, a);
    }
    else if((0 == strcmp(cmd, "ki")) && (n >= 2))       // 只改 Ki
    {
        motor_set_pid_term(1, 1, a);
        motor_set_pid_term(2, 1, a);
    }
    else if((0 == strcmp(cmd, "kd")) && (n >= 2))       // 只改 Kd
    {
        motor_set_pid_term(1, 2, a);
        motor_set_pid_term(2, 2, a);
    }
    else if((0 == strcmp(cmd, "spd")) && (n >= 2))
    {
        track_set_speed(a);
    }
    else if((0 == strcmp(cmd, "dir")) && (n >= 4))
    {
        track_set_steer_param(a, b, c);
    }
    else if((0 == strcmp(cmd, "curve")) && (n >= 3))
    {
        track_set_curve_param(a, b);
    }
    else if((0 == strcmp(cmd, "diff")) && (n >= 2))
    {
        track_set_diff_gain(a);
    }
    else if((0 == strcmp(cmd, "thr")) && (n >= 2))
    {
        image_set_threshold_offset((int16)a);
    }
    else if(0 == strcmp(cmd, "start"))
    {
        track_start();
    }
    else if(0 == strcmp(cmd, "stop"))
    {
        track_stop();
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      从环形缓冲里取字节拼行，凑够一条就执行
// 备注信息      回车和换行都算一行结束（VOFA+ 发 "\r\n" 或 "\n" 都行）
//-------------------------------------------------------------------------------------------------------------------
static void vofa_parse_rx (void)
{
    char c;

    while(vofa_rx_tail != vofa_rx_head)
    {
        c = (char)vofa_rx_buf[vofa_rx_tail];
        vofa_rx_tail = (uint16)((vofa_rx_tail + 1) % VOFA_RX_BUF_SIZE);

        if(('\n' == c) || ('\r' == c))
        {
            if(vofa_line_len > 0)
            {
                vofa_line[vofa_line_len] = 0;
                vofa_exec(vofa_line);
                vofa_line_len = 0;
            }
        }
        else if(vofa_line_len < (VOFA_LINE_MAX - 1))
        {
            vofa_line[vofa_line_len] = c;
            vofa_line_len++;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      把当前状态填进 4 个通道
// 备注信息      改通道顺序/数量时，这里和 vofa.h 的说明要一起改，并把 VOFA_CH_NUM 改对
//-------------------------------------------------------------------------------------------------------------------
static void vofa_fill (void)
{
    vofa_data[0] = motor_get_speed(1);                  // A1
    vofa_data[1] = motor_get_speed(2);                  // A2
    vofa_data[2] = (float32)image_error;                // ERR
    vofa_data[3] = imu_get_yaw_deg();                   // YAW
}

//==================================================== 对外接口 ====================================================
//-------------------------------------------------------------------------------------------------------------------
// 函数简介      串口 + 接收中断初始化
//-------------------------------------------------------------------------------------------------------------------
void vofa_init (void)
{
    vofa_rx_head  = 0;
    vofa_rx_tail  = 0;
    vofa_line_len = 0;
    vofa_last_ms  = system_getval_ms();

    uart_init(VOFA_UART, VOFA_BAUD, VOFA_TX_PIN, VOFA_RX_PIN);
    uart_rx_interrupt(VOFA_UART, 1);                    // 打开接收中断，命令从 isr.c 进来
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      主循环调用：解析命令 + 按周期发波形
//-------------------------------------------------------------------------------------------------------------------
void vofa_update (void)
{
    uint32 now;

    vofa_parse_rx();                                    // 先处理上位机命令，延迟最大 20ms

    now = system_getval_ms();
    if((now - vofa_last_ms) < VOFA_PERIOD_MS)
    {
        return;
    }
    vofa_last_ms = now;

    vofa_fill();

    memcpy(vofa_tx_buf, vofa_data, sizeof(vofa_data));
    memcpy(&vofa_tx_buf[sizeof(vofa_data)], vofa_tail, sizeof(vofa_tail));

    uart_write_buffer(VOFA_UART, vofa_tx_buf, (uint32)(sizeof(vofa_data) + sizeof(vofa_tail)));
}
