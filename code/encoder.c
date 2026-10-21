/*********************************************************************************************************************
* 文件名称          encoder.c
* 功能简介          两路编码器读取（正交/方向计数 + 单位时间增量）
* 硬件平台          TC377TP + 逐飞开源库
*
* 接线（按《推荐IO分配.txt》）
*   编码器1：计数 P10_3   方向 P10_1    -> GPT12 的 T5 定时器
*   编码器2：计数 P20_3   方向 P20_0    -> GPT12 的 T6 定时器
*
* 读取方式
*   encoder_get_count() 读的是定时器当前计数值，读完立刻清零，
*   所以每次读到的就是"本周期增量"，天然不会溢出，也不用做差分。
*********************************************************************************************************************/

#include "encoder.h"

//==================================================== 配置 ========================================================
// 编码器使用的定时器与引脚
// 注意：T5 的计数脚只能是 P10_3 或 P21_7；T6 的计数脚只能是 P20_3 或 P10_2
//       （这是 GPT12 的硬件限制，具体可选范围见 libraries/zf_driver/zf_driver_encoder.h）
#define ENCODER_1_INDEX         (TIM5_ENCODER)
#define ENCODER_1_COUNT_PIN     (TIM5_ENCODER_CH1_P10_3)
#define ENCODER_1_DIR_PIN       (TIM5_ENCODER_CH2_P10_1)

#define ENCODER_2_INDEX         (TIM6_ENCODER)
#define ENCODER_2_COUNT_PIN     (TIM6_ENCODER_CH1_P20_3)
#define ENCODER_2_DIR_PIN       (TIM6_ENCODER_CH2_P20_0)

// ******************************* 编码器方向取反（必须实测，不能猜）*******************************
// 【判据】给一个正的目标速度，轮子【往前转】的时候，屏幕上 A1 / A2 必须是【正数】。
//         哪一路是负数，就把哪一路的 INVERT 翻一下（0 改 1，或者 1 改 0）。
//
// 【重要】两路不一定要一样！左右轮是镜像装的，两个编码器的 A/B 接线顺序也可能不同，
//         所以完全可能一路要 0、另一路要 1。以实车实测为准，不要靠"两边必须一致"来猜。
//
// 【后果】符号反了就是【正反馈】：
//             目标 = +191，编码器读回 -191
//             误差 = 191 - (-191) = 382，而且越转越负
//             -> 占空比一路顶到 +10000 -> 轮子疯转，PID 永远收敛不了
//         所以这一步必须在正式跑赛道之前确认好。
#define ENCODER_1_INVERT        (1)         // 电机1：正转时 A1 是正数就保持 1，是负数就改 0
#define ENCODER_2_INVERT        (0)         // 电机2：已实测"正转时 A2 为负"，所以由 1 翻成 0

//==================================================== 内部变量 ====================================================
// 注意：这里的类型是【库的】encoder_index_enum（TIM5_ENCODER / TIM6_ENCODER），
//       和本文件对外用的 encoder_id_enum（ENCODER_ID_1/2）是两个东西，别混
static const encoder_index_enum encoder_index_table[ENCODER_ID_COUNT] =
{
    ENCODER_1_INDEX,
    ENCODER_2_INDEX,
};

static const uint8 encoder_invert_table[ENCODER_ID_COUNT] =
{
    ENCODER_1_INVERT,
    ENCODER_2_INVERT,
};

static int16   encoder_delta[ENCODER_ID_COUNT];        // 本周期增量（脉冲/周期）
static int32   encoder_total[ENCODER_ID_COUNT];        // 累计脉冲
static int16   encoder_speed[ENCODER_ID_COUNT];        // 滤波后的速度（脉冲/周期）

//==================================================== 对外接口 ====================================================
//-------------------------------------------------------------------------------------------------------------------
// 函数简介      编码器初始化
//-------------------------------------------------------------------------------------------------------------------
void encoder_init (void)
{
    uint8 i;

    for(i = 0; i < ENCODER_ID_COUNT; i++)
    {
        encoder_delta[i] = 0;
        encoder_total[i] = 0;
        encoder_speed[i] = 0;
    }

    // T5 / T6 只支持"一路计数 + 一路方向"模式，所以用 encoder_dir_init
    encoder_dir_init(ENCODER_1_INDEX, ENCODER_1_COUNT_PIN, ENCODER_1_DIR_PIN);
    encoder_dir_init(ENCODER_2_INDEX, ENCODER_2_COUNT_PIN, ENCODER_2_DIR_PIN);

    encoder_clear_count(ENCODER_1_INDEX);
    encoder_clear_count(ENCODER_2_INDEX);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      编码器刷新（固定周期调用，比如 10ms 或 20ms 一次）
// 返回参数      void
// 备注信息      内部做三件事：读增量 -> 累加总计数 -> 一阶低通滤波得到速度
//-------------------------------------------------------------------------------------------------------------------
void encoder_update (void)
{
    uint8 i;
    int16 count;

    for(i = 0; i < ENCODER_ID_COUNT; i++)
    {
        // 读本周期增量并立刻清零
        count = encoder_get_count(encoder_index_table[i]);
        encoder_clear_count(encoder_index_table[i]);

        if(encoder_invert_table[i])
        {
            count = -count;
        }

        encoder_delta[i] = count;
        encoder_total[i] += count;

        // 一阶低通滤波：speed = speed + (count - speed) / 2
        // 让屏幕上的速度数字稳定一点，不至于每周期乱跳
        encoder_speed[i] += (int16)((count - encoder_speed[i]) / 2);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      读取本周期增量（脉冲/周期），正负表示转向
//-------------------------------------------------------------------------------------------------------------------
int16 encoder_get_delta (encoder_id_enum encoder_n)
{
    if(encoder_n >= ENCODER_ID_COUNT)
    {
        return 0;
    }

    return encoder_delta[encoder_n];
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      读取累计脉冲数（从开机或上次清零开始）
//-------------------------------------------------------------------------------------------------------------------
int32 encoder_get_total (encoder_id_enum encoder_n)
{
    if(encoder_n >= ENCODER_ID_COUNT)
    {
        return 0;
    }

    return encoder_total[encoder_n];
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      读取滤波后的速度（脉冲/周期）
//-------------------------------------------------------------------------------------------------------------------
int16 encoder_get_speed (encoder_id_enum encoder_n)
{
    if(encoder_n >= ENCODER_ID_COUNT)
    {
        return 0;
    }

    return encoder_speed[encoder_n];
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      累计脉冲清零
//-------------------------------------------------------------------------------------------------------------------
void encoder_clear_total (encoder_id_enum encoder_n)
{
    if(encoder_n >= ENCODER_ID_COUNT)
    {
        return;
    }

    encoder_total[encoder_n] = 0;
}
