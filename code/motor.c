/*********************************************************************************************************************
* 文件名称          motor.c
* 功能简介          两路电机速度闭环控制（PWM + DIR 驱动，编码器测速，PID 调速）
* 硬件平台          TC377TP + 逐飞开源库
*
* ------------------------------ 接线 ------------------------------
*   电机1：P21_3 = PWM(调速)    P21_2 = DIR(方向)    编码器1：P10_3(计数) + P10_1(方向)
*   电机2：P21_5 = PWM(调速)    P21_4 = DIR(方向)    编码器2：P20_3(计数) + P20_0(方向)
*
* ------------------------------ 速度单位 ------------------------------
*   速度单位 = 脉冲 / 控制周期
*   控制周期 = MOTOR_CONTROL_PERIOD_MS（当前 20ms，和编码器采样周期一致）
*   屏幕上的 DELTA 就是这个速度，所以调参时眼睛看屏幕就知道控制得好不好
*
* ------------------------------ 控制流程 ------------------------------
*   1. 编码器定时中断（CCU60_CH0，20ms）里调 encoder_update() 采样
*   2. 主循环里每 20ms 调一次 motor_control_update()：
*          读实际速度 -> PID -> 输出限幅 -> 加减速斜坡 -> 方向+死区补偿 -> 写 PWM -> 堵转检测
*   3. 屏幕每 100ms 刷新一次显示目标速度 / 实际速度 / 占空比
*
* ------------------------------ 调参 ------------------------------
*   OK  命令格式：p <PID参数名> <值>，一行一个字符，所以必须一次发一个字符
*   所以参数名用单字符：p = Kp，i = Ki，d = Kd
*   例：先发 'p'，再发数字……不方便，所以本工程改为【编译期改参数 + 屏幕观察】
*   参数在下面"PID 默认参数"一节，改完重新烧录即可
*********************************************************************************************************************/

#include "motor.h"
#include "encoder.h"

//==================================================== 控制参数 ====================================================
#define MOTOR_CONTROL_PERIOD_MS (20)            // 闭环控制周期，必须和 ENCODER_UPDATE_MS 一致
#define MOTOR_PWM_FREQ          (10000)         // PWM 频率 10kHz（GTM 20MHz -> 周期 2000 tick）

// ------------------------------ PID 默认参数 ------------------------------
// 速度单位是"脉冲/20ms"，输出是 0~10000 的占空比
// 调参顺序：先把 Ki、Kd 设 0，只加 Kp 到速度能跟上且不抖；再加 Ki 消稳态误差；Kd 一般 0~1
// !! 重要 !! 目标速度必须和"电机实际能力"匹配，不能凭感觉乱设
// 实测数据：占空比约 3000（30%）时，ENC/ACT 约 310 脉冲/20ms
// 所以目标先设 200（约 20% 占空比），跑起来看 DUTY 落在 2000~6000 之间就合适
//   DUTY 顶到 10000 还追不上目标 -> 目标太大，往下降
//   DUTY 很小（不到 1500）就到目标 -> 目标太小，往上升
// 判断标准：稳定后 DUTY 停在中间某个值，ACT 贴着 TGT
#define MOTOR_1_TARGET_SPEED    (600.0f)        // 电机1 目标速度（脉冲/20ms）
#define MOTOR_2_TARGET_SPEED    (600.0f)        // 电机2 目标速度

// PID 参数的量级要跟着目标速度走：
// 目标从 40 改成 200（放大 5 倍），比例系数必须相应缩小，否则一点误差就顶到满占空比直接振荡
#define MOTOR_KP_DEFAULT        (2.0f)          // 被控对象增益 0.425 脉冲/占空比 -> 环路增益约 0.85
#define MOTOR_KI_DEFAULT        (0.5f)          // 有前馈兜底，积分只需要很小的修正量
#define MOTOR_KD_DEFAULT        (0.0f)          // 微分系数

#define MOTOR_OUT_MAX           (10000.0f)      // PID 输出上限（占空比满量程）
#define MOTOR_OUT_MIN           (-10000.0f)     // PID 输出下限
#define MOTOR_INTEGRAL_MAX      (3000.0f)       // 积分项单独限幅，抗积分饱和
#define MOTOR_INTEGRAL_MIN      (-3000.0f)
#define MOTOR_INTEGRAL_SEPARATE (40.0f)         // 积分分离阈值：误差大于此值时先不积分，避免起步超调

// ------------------------------ 加减速斜坡 ------------------------------
// 限制每个控制周期占空比的变化量，保护驱动和齿轮、抑制电流冲击
#define MOTOR_RAMP_UP           (500)           // 每周期最大上升量（10000 满量程，20ms 周期 -> 约 0.4s 到满）
#define MOTOR_RAMP_DOWN         (800)           // 每周期最大下降量

// ------------------------------ 死区补偿 ------------------------------
// 占空比太小时电机转不动，直接给一个最小有效值
// 10kHz 下周期 2000 tick，1000 = 10% = 2 个 tick，确保真的输出
#define MOTOR_DUTY_MIN          (300)           // 死区补偿：只有 MOTOR_FF_ENABLE = 0 时才用得到

//==================================================== 前馈 + PI（参考示例工程 scheduler.c）====================
// 开环实测数据（占空比 -> 速度，单位 脉冲/20ms）：
//     电机1:  D=1000 -> 271    D=2000 -> 690    D=3000 -> 1085   D=5000 -> 1973
//     电机2:  D=1000 -> 167    D=2000 -> 566    D=3000 ->  971   D=5000 -> 1853
// 最小二乘拟合：
//     电机1:  速度 = 0.425 * (D - 386)      死区 386
//     电机2:  速度 = 0.422 * (D - 644)      死区 644
// 两边斜率几乎一样，但死区差了 260 —— 这就是低速时两个轮子快慢不一致的根源。
//
// 反过来用就是前馈：  占空比 = 死区 + 目标速度 / 斜率 = 死区 + 目标速度 * 2.36
//     目标 200 时： 电机1 = 386 + 472 =  858（实测需要  856）
//                   电机2 = 644 + 472 = 1116（实测需要 1118）
// 前馈只跟目标有关、跟实测无关，所以第一拍就给到基本正确的占空比，
// PID 只在 ±MOTOR_FF_PID_MAX 里做小修正 —— 彻底避开"PID 输出必须跨过死区"这个来回翻的根源。
//
// 注意：开了前馈之后 motor_duty_limit() 里的【死区补偿会自动关掉】，
//       因为前馈已经把死区算进去了，再叠一次就重复了。
#define MOTOR_FF_ENABLE         (1)             // 1 = 前馈 + PI；0 = 退回纯 PID
#define MOTOR_FF_PID_MAX        (1500.0f)       // PID 修正量限幅（占空比）
#define MOTOR_FF_DUTY_PER_SPEED (2.36f)         // 每 1 单位目标速度需要的占空比 = 1/0.424
#define MOTOR_1_FF_DEADZONE     (386.0f)        // 电机1 静摩擦死区（实测）
#define MOTOR_2_FF_DEADZONE     (644.0f)        // 电机2 静摩擦死区（实测）

//==================================================== 开环标定模式 ====================================================
// 1 = 绕开 PID，让两路电机一起跑一个缓慢上下扫的占空比，专门用来量电机曲线。
//     屏幕上 D1/D2 会一档一档往上跳（见 MOTOR_OL_TABLE），每档停 2 秒，A1/A2 跟着变。
//     标定完【一定要改回 0】，否则车不会受控。
// 要量三个数：
//     ① 占空比涨到多少，轮子【刚开始】动（这就是静摩擦死区）
//     ② 几个 (占空比, 速度) 点，比如 D=1000/2000/3000 时 A 是多少
//     ③ 占空比给到很大时 A 最大能到多少（电机最高速，决定目标速度上限）
#define MOTOR_OPEN_LOOP_TEST    (0)             // 标定已完成，改回 0（1 = 开环标定，车不受控）
#define MOTOR_OL_HOLD_CYCLE     (100)           // 每一档占空比停多少个控制周期（100 x 20ms = 2 秒）
// 开环标定的占空比档位（每档停 2 秒，方便抄 A1/A2）
#define MOTOR_OL_TABLE  { 0.0f, 400.0f, 600.0f, 800.0f, 1000.0f, 1200.0f, 1500.0f, 2000.0f, 2500.0f, 3000.0f, 4000.0f, 5000.0f }

// ------------------------------ 目标速度 ------------------------------
// 上电启动延时 MOTOR_START_DELAY_MS 已经移到 motor.h（主程序也要用）

// ------------------------------ 堵转保护 ------------------------------
#define MOTOR_STALL_DUTY        (4000)          // 占空比超过此值才判断堵转
#define MOTOR_STALL_SPEED       (2)             // 速度绝对值小于此值认为"没转起来"
#define MOTOR_STALL_COUNT       (25)            // 连续这么多个周期（25*20ms=0.5s）判定堵转

//==================================================== 引脚定义 ====================================================
#define MOTOR_1_PWM_PIN         (ATOM0_CH1_P21_3)           // 电机1 PWM = P21_3
#define MOTOR_1_DIR_PIN         ((gpio_pin_enum)P21_2)      // 电机1 DIR = P21_2
#define MOTOR_2_PWM_PIN         (ATOM0_CH3_P21_5)           // 电机2 PWM = P21_5
#define MOTOR_2_DIR_PIN         ((gpio_pin_enum)P21_4)      // 电机2 DIR = P21_4

#define MOTOR_1_DIR_LEVEL       (1)             // 电机1"正转电平"（此处配合 MOTOR_1_INVERT=1 使用：
                                                                // 取反后正转实际输出低电平，和你板子的 DIR 低电平正转匹配）
#define MOTOR_2_DIR_LEVEL       (1)             // 电机2 同上

#define MOTOR_1_INVERT          (1)             // 电机1 取反：你的驱动板 DIR 是【低电平正转】，所以这里必须是 1
#define MOTOR_2_INVERT          (1)             // 电机2 取反：同上

//==================================================== 内部数据结构 ================================================
typedef struct
{
    // ---- 硬件 ----
    pwm_channel_enum    pwm_pin;
    gpio_pin_enum       dir_pin;
    uint8               dir_level;
    uint8               invert;
    encoder_id_enum     encoder_id;

    // ---- PID 参数（每路独立，方便单独调）----
    float32             kp;
    float32             ki;
    float32             kd;
    float32             ff_deadzone;        // 这一路的静摩擦死区（前馈用）

    // ---- PID 状态 ----
    float32             target;             // 目标速度（脉冲/周期）
    float32             actual;             // 实际速度（脉冲/周期，已滤波）
    float32             integral;           // 积分累加项
    float32             last_error;         // 上次误差，用于微分
    float32             output;             // PID 输出（占空比，未限幅前）
    float32             duty;               // 实际加在 PWM 上的占空比（经斜坡限制）

    // ---- 堵转 ----
    uint16              stall_count;
    uint8               stall;
}motor_obj_struct;

static motor_obj_struct motor_list[2];

//==================================================== 内部函数 ====================================================
//-------------------------------------------------------------------------------------------------------------------
// 函数简介      占空比限幅 + 死区补偿
//-------------------------------------------------------------------------------------------------------------------
static float32 motor_duty_limit (float32 duty)
{
    if(duty > MOTOR_OUT_MAX)
    {
        duty = MOTOR_OUT_MAX;
    }
    else if(duty < MOTOR_OUT_MIN)
    {
        duty = MOTOR_OUT_MIN;
    }

#if (0 == MOTOR_FF_ENABLE)
    // 死区补偿：有输出但不足以让电机起转时，直接给最小有效占空比。
    // 注意这个补偿是个"阶跃"：输出 ±50 会被抬成 ±MOTOR_DUTY_MIN。
    // 死区比 MOTOR_DUTY_MIN 大的时候没问题，比它小的时候就会来回翻，
    // 所以开了前馈就把它关掉。
    if((duty > 0.0f) && (duty < (float32)MOTOR_DUTY_MIN))
    {
        duty = (float32)MOTOR_DUTY_MIN;
    }
    else if((duty < 0.0f) && (duty > -(float32)MOTOR_DUTY_MIN))
    {
        duty = -(float32)MOTOR_DUTY_MIN;
    }
#endif

    return duty;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      设置方向和占空比
// 参数说明      motor_obj       电机对象
// 参数说明      duty            占空比，正数正转、负数反转、0 停车
// 备注信息      方向引脚先动作、PWM 后更新，避免换向瞬间的反向电流冲击
//-------------------------------------------------------------------------------------------------------------------
static void motor_output_set (motor_obj_struct *motor_obj, float32 duty)
{
    uint32 duty_value;

    if(duty > 0.0f)
    {
        // 正转：DIR 给"正转电平"（把接线取反也算进去）
        if(motor_obj->invert)
        {
            gpio_set_level(motor_obj->dir_pin, (0 == motor_obj->dir_level) ? 1 : 0);
        }
        else
        {
            gpio_set_level(motor_obj->dir_pin, motor_obj->dir_level);
        }
    }
    else if(duty < 0.0f)
    {
        // 反转：DIR 给相反电平
        if(motor_obj->invert)
        {
            gpio_set_level(motor_obj->dir_pin, motor_obj->dir_level);
        }
        else
        {
            gpio_set_level(motor_obj->dir_pin, (0 == motor_obj->dir_level) ? 1 : 0);
        }
    }
    // duty == 0：不动方向引脚，只把 PWM 给 0

    duty_value = (uint32)((duty >= 0.0f) ? (duty + 0.5f) : (-duty + 0.5f));
    if(duty_value > (uint32)MOTOR_OUT_MAX)
    {
        duty_value = (uint32)MOTOR_OUT_MAX;
    }

    pwm_set_duty(motor_obj->pwm_pin, duty_value);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      堵转检测
// 备注信息      占空比给得够大，但编码器连续多个周期几乎没增量 -> 判定堵转
//-------------------------------------------------------------------------------------------------------------------
static void motor_stall_check (motor_obj_struct *motor_obj)
{
    float32 duty_abs = (motor_obj->duty >= 0.0f) ? motor_obj->duty : -motor_obj->duty;
    float32 speed_abs = (motor_obj->actual >= 0.0f) ? motor_obj->actual : -motor_obj->actual;

    if((duty_abs > (float32)MOTOR_STALL_DUTY) && (speed_abs < (float32)MOTOR_STALL_SPEED))
    {
        if(motor_obj->stall_count < 0xFFFF)
        {
            motor_obj->stall_count++;
        }

        if(motor_obj->stall_count >= MOTOR_STALL_COUNT)
        {
            motor_obj->stall = 1;
        }
    }
    else
    {
        motor_obj->stall_count = 0;
        motor_obj->stall       = 0;
    }
}

//==================================================== 对外接口 ====================================================
//-------------------------------------------------------------------------------------------------------------------
// 函数简介      闭环初始化：PWM + DIR + PID 参数
// 备注信息      编码器由 encoder_init() 初始化，本函数不重复初始化
//-------------------------------------------------------------------------------------------------------------------
void motor_init (void)
{
    // ------------------------------- 电机1 -------------------------------
    motor_list[0].pwm_pin   = MOTOR_1_PWM_PIN;
    motor_list[0].dir_pin   = MOTOR_1_DIR_PIN;
    motor_list[0].dir_level = MOTOR_1_DIR_LEVEL;
    motor_list[0].invert    = MOTOR_1_INVERT;
    motor_list[0].encoder_id = ENCODER_ID_1;
    motor_list[0].kp        = MOTOR_KP_DEFAULT;
    motor_list[0].ki        = MOTOR_KI_DEFAULT;
    motor_list[0].kd        = MOTOR_KD_DEFAULT;
    motor_list[0].ff_deadzone = MOTOR_1_FF_DEADZONE;

    // ------------------------------- 电机2 -------------------------------
    motor_list[1].pwm_pin   = MOTOR_2_PWM_PIN;
    motor_list[1].dir_pin   = MOTOR_2_DIR_PIN;
    motor_list[1].dir_level = MOTOR_2_DIR_LEVEL;
    motor_list[1].invert    = MOTOR_2_INVERT;
    motor_list[1].encoder_id = ENCODER_ID_2;
    motor_list[1].kp        = MOTOR_KP_DEFAULT;
    motor_list[1].ki        = MOTOR_KI_DEFAULT;
    motor_list[1].kd        = MOTOR_KD_DEFAULT;
    motor_list[1].ff_deadzone = MOTOR_2_FF_DEADZONE;

    // ------------------------------- 公共初始化 -------------------------------
    {
        uint8 i;

        for(i = 0; i < 2; i++)
        {
            motor_list[i].target     = 0.0f;
            motor_list[i].actual     = 0.0f;
            motor_list[i].integral   = 0.0f;
            motor_list[i].last_error = 0.0f;
            motor_list[i].output     = 0.0f;
            motor_list[i].duty       = 0.0f;
            motor_list[i].stall_count = 0;
            motor_list[i].stall      = 0;

            // 方向引脚先输出"反转"电平，保证上电电机不转
            gpio_init(motor_list[i].dir_pin,
                      GPO,
                      (0 == motor_list[i].dir_level) ? 1 : 0,
                      GPO_PUSH_PULL);

            // PWM 先用 0 占空比初始化
            pwm_init(motor_list[i].pwm_pin, MOTOR_PWM_FREQ, 0);
            pwm_set_duty(motor_list[i].pwm_pin, 0);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      设置目标速度（单位：脉冲/控制周期）
// 参数说明      motor_no        1 或 2
// 参数说明      speed           目标速度，正数正转、负数反转、0 停
// 使用示例      motor_set_speed(1, 40.0f);      // 电机1 目标 40 脉冲/20ms
// 备注信息      目标设为 0 时顺便清零积分，避免下次起动时输出跳变
//-------------------------------------------------------------------------------------------------------------------
void motor_set_speed (uint8 motor_no, float32 speed)
{
    motor_obj_struct *motor_obj;
    uint32 primask;

#if (MOTOR_OPEN_LOOP_TEST)
    return;                                 // 开环标定模式：忽略上层的一切速度指令
#endif


    if((motor_no < 1) || (motor_no > 2))
    {
        return;
    }

    motor_obj = &motor_list[motor_no - 1];

    // 速度环现在跑在 CCU60_CH0 定时中断里（见 user/isr.c），
    // 主循环改目标值必须关中断，否则可能改到一半被中断打断，PID 会拿到半新半旧的目标值
    primask = interrupt_global_disable();

    motor_obj->target = speed;

    if(0.0f == speed)
    {
        motor_obj->integral   = 0.0f;
        motor_obj->last_error = 0.0f;
    }

    interrupt_global_enable(primask);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      读取实际速度（脉冲/控制周期）
//-------------------------------------------------------------------------------------------------------------------
float32 motor_get_speed (uint8 motor_no)
{
    if((motor_no < 1) || (motor_no > 2))
    {
        return 0.0f;
    }

    return motor_list[motor_no - 1].actual;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      读取实际占空比 0~10000
//-------------------------------------------------------------------------------------------------------------------
float32 motor_get_duty (uint8 motor_no)
{
    if((motor_no < 1) || (motor_no > 2))
    {
        return 0.0f;
    }

    return motor_list[motor_no - 1].duty;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      读取目标速度
//-------------------------------------------------------------------------------------------------------------------
float32 motor_get_target (uint8 motor_no)
{
    if((motor_no < 1) || (motor_no > 2))
    {
        return 0.0f;
    }

    return motor_list[motor_no - 1].target;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      读取堵转标志：1 = 堵转
//-------------------------------------------------------------------------------------------------------------------
uint8 motor_get_stall (uint8 motor_no)
{
    if((motor_no < 1) || (motor_no > 2))
    {
        return 0;
    }

    return motor_list[motor_no - 1].stall;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      在线修改某路电机的 PID 参数
// 参数说明      motor_no        1 或 2
// 参数说明      kp, ki, kd      新的 PID 参数
// 备注信息      改参数会同时清零积分，避免参数切换瞬间输出跳变
//-------------------------------------------------------------------------------------------------------------------
void motor_set_pid (uint8 motor_no, float32 kp, float32 ki, float32 kd)
{
    if((motor_no < 1) || (motor_no > 2))
    {
        return;
    }

    motor_list[motor_no - 1].kp       = kp;
    motor_list[motor_no - 1].ki       = ki;
    motor_list[motor_no - 1].kd       = kd;
    motor_list[motor_no - 1].integral = 0.0f;
    motor_list[motor_no - 1].last_error = 0.0f;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      读取某路电机的 PID 参数
//-------------------------------------------------------------------------------------------------------------------
void motor_get_pid (uint8 motor_no, float32 *kp, float32 *ki, float32 *kd)
{
    if((motor_no < 1) || (motor_no > 2))
    {
        return;
    }

    if(NULL != kp) { *kp = motor_list[motor_no - 1].kp; }
    if(NULL != ki) { *ki = motor_list[motor_no - 1].ki; }
    if(NULL != kd) { *kd = motor_list[motor_no - 1].kd; }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      所有电机立即停止（PWM 置 0，目标清零，积分清零）
//-------------------------------------------------------------------------------------------------------------------
void motor_stop_all (void)
{
    uint8 i;
    uint32 primask;

#if (MOTOR_OPEN_LOOP_TEST)
    return;                                 // 开环标定模式：不要来抢 PWM
#endif


    // 速度环在中断里跑，这里也要关中断，免得停到一半被中断写回一个非零占空比
    primask = interrupt_global_disable();

    for(i = 0; i < 2; i++)
    {
        motor_list[i].target     = 0.0f;
        motor_list[i].integral   = 0.0f;
        motor_list[i].last_error = 0.0f;
        motor_list[i].output     = 0.0f;
        motor_list[i].duty       = 0.0f;
        motor_list[i].stall      = 0;
        motor_list[i].stall_count = 0;

        motor_output_set(&motor_list[i], 0.0f);
    }

    interrupt_global_enable(primask);
}



//-------------------------------------------------------------------------------------------------------------------
// 函数简介      速度闭环控制节拍（每 MOTOR_CONTROL_PERIOD_MS 毫秒调用一次）
// 返回参数      void
// 备注信息      流程：读速度 -> PID -> 限幅 -> 斜坡 -> 输出 -> 堵转检测
//               不使用浮点除法以外的复杂运算，20ms 周期下开销很低
//-------------------------------------------------------------------------------------------------------------------
void motor_control_update (void)
{
    uint8   i;
    float32 error;
    float32 p_out;
    float32 i_out;
    float32 d_out;
    float32 output;
    float32 feedforward;
    float32 pid_out;
    float32 target_duty;

#if (MOTOR_OPEN_LOOP_TEST)
    // ---------------- 开环标定：完全绕开 PID，按档位给固定占空比，每档停 2 秒 ----------------
    // D1/D2 会一档一档往上跳：0 -> 400 -> 600 -> 800 -> 1000 -> 1200 -> 1500 -> 2000
    //                       -> 2500 -> 3000 -> 4000 -> 5000 -> 回到 0，循环
    // 每档停 2 秒，A1/A2 会稳定下来，从容抄数。
    {
        static const float32 ol_table[] = MOTOR_OL_TABLE;
        static uint16 ol_index = 0;
        static uint16 ol_hold  = 0;
        float32 ol_duty = ol_table[ol_index];

        ol_hold++;
        if(ol_hold >= (uint16)MOTOR_OL_HOLD_CYCLE)
        {
            ol_hold = 0;
            ol_index++;
            if(ol_index >= (uint16)(sizeof(ol_table) / sizeof(ol_table[0])))
            {
                ol_index = 0;
            }
        }

        for(i = 0; i < 2; i++)
        {
            motor_list[i].actual = (float32)encoder_get_speed(motor_list[i].encoder_id);
            motor_list[i].duty   = ol_duty;
            motor_output_set(&motor_list[i], ol_duty);
        }
        return;
    }
#endif

    for(i = 0; i < 2; i++)
    {
        motor_obj_struct *motor_obj = &motor_list[i];

        // ---------------------- 1. 读实际速度（编码器定时中断已经采好）---------------------
        motor_obj->actual = (float32)encoder_get_speed(motor_obj->encoder_id);

        // ---------------------- 2. 前馈：按目标速度直接算出该给多少占空比 ----------------------
        // 实测 速度 = 斜率 x (占空比 - 死区)，所以反过来 占空比 = 死区 + 速度 / 斜率。
        // 前馈只跟目标有关、跟实测无关，第一拍就能给到接近正确的值，
        // PID 只需要在小范围里修正 —— 不会出现"小输出被死区放大成反转"的问题。
        if(motor_obj->target > 0.0f)
        {
            feedforward = motor_obj->ff_deadzone + motor_obj->target * MOTOR_FF_DUTY_PER_SPEED;
        }
        else if(motor_obj->target < 0.0f)
        {
            feedforward = -(motor_obj->ff_deadzone + (-motor_obj->target) * MOTOR_FF_DUTY_PER_SPEED);
        }
        else
        {
            feedforward = 0.0f;
        }

        // ---------------------- 3. PID：只在前馈基础上做小修正 ----------------------
        error = motor_obj->target - motor_obj->actual;

        // 比例项
        p_out = motor_obj->kp * error;

        // 积分项（带积分分离）
        if((error < MOTOR_INTEGRAL_SEPARATE) && (error > -MOTOR_INTEGRAL_SEPARATE))
        {
            motor_obj->integral += error;

            if(motor_obj->integral > MOTOR_INTEGRAL_MAX)
            {
                motor_obj->integral = MOTOR_INTEGRAL_MAX;
            }
            else if(motor_obj->integral < MOTOR_INTEGRAL_MIN)
            {
                motor_obj->integral = MOTOR_INTEGRAL_MIN;
            }
        }
        i_out = motor_obj->ki * motor_obj->integral;

        // 微分项
        d_out = motor_obj->kd * (error - motor_obj->last_error);
        motor_obj->last_error = error;

        pid_out = p_out + i_out + d_out;

        // 修正量限幅 + 抗积分饱和
        if(pid_out > MOTOR_FF_PID_MAX)
        {
            pid_out = MOTOR_FF_PID_MAX;
            if(error > 0.0f) { motor_obj->integral -= error; }
        }
        else if(pid_out < -MOTOR_FF_PID_MAX)
        {
            pid_out = -MOTOR_FF_PID_MAX;
            if(error < 0.0f) { motor_obj->integral -= error; }
        }

        // ---------------------- 4. 前馈 + 修正 = 最终输出 ----------------------
        output = feedforward + pid_out;

        if(output > MOTOR_OUT_MAX) { output = MOTOR_OUT_MAX; }
        if(output < MOTOR_OUT_MIN) { output = MOTOR_OUT_MIN; }

        motor_obj->output = output;

        // ---------------------- 3. 加减速斜坡 + 死区补偿 ---------------------
        target_duty = motor_duty_limit(output);

        if(target_duty > motor_obj->duty)
        {
            motor_obj->duty += (float32)MOTOR_RAMP_UP;
            if(motor_obj->duty > target_duty)
            {
                motor_obj->duty = target_duty;
            }
        }
        else if(target_duty < motor_obj->duty)
        {
            motor_obj->duty -= (float32)MOTOR_RAMP_DOWN;
            if(motor_obj->duty < target_duty)
            {
                motor_obj->duty = target_duty;
            }
        }

        motor_obj->duty = motor_duty_limit(motor_obj->duty);

        // ---------------------- 4. 写 PWM + 方向 ---------------------
        motor_output_set(motor_obj, motor_obj->duty);

        // ---------------------- 5. 堵转检测 ---------------------
        motor_stall_check(motor_obj);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      闭环测试：上电延时一会儿后给两个电机固定目标速度
// 参数说明      void
// 返回参数      void
// 备注信息      由 user/cpu0_main.c 调用；想改目标速度见本文件顶部的 MOTOR_x_TARGET_SPEED
//-------------------------------------------------------------------------------------------------------------------
void motor_close_loop_test_start (void)
{
    motor_set_speed(1, MOTOR_1_TARGET_SPEED);
    motor_set_speed(2, MOTOR_2_TARGET_SPEED);
}
