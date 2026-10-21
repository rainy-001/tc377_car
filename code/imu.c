/*********************************************************************************************************************
* 文件名称          imu.c
* 功能简介          IMU660RB 陀螺仪：偏航角速度 -> 航向角
*********************************************************************************************************************/

#include "imu.h"

//==================================================== 全局变量 ====================================================
float32 imu_gyro_dps[3] = {0.0f, 0.0f, 0.0f};
float32 imu_yaw_deg     = 0.0f;

//==================================================== 内部变量 ====================================================
static float32 imu_zero[3] = {0.0f, 0.0f, 0.0f};   // 上电校零得到的三轴零偏（度/秒）
static uint32  imu_last_ms = 0;
static uint8   imu_ok      = 0;

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      IMU 初始化 + 上电校零
// 备注信息      【调用时车必须静止】否则这个错误零偏会被一直积进去，航向角全程是漂的。
//               校零分两遍：
//                 第一遍粗平均；
//                 第二遍只保留"离粗平均很近"的采样再平均一次 ——
//                 校零期间万一被人碰一下、或者桌子震一下，那几拍会被剔掉，不污染零偏。
//               初始化失败也不会死：imu_update() 直接什么都不做，屏幕上的 GX/GY/GZ 保持 0。
//-------------------------------------------------------------------------------------------------------------------
void imu_init (void)
{
    uint16 i;
    int32  sum[3]  = {0, 0, 0};
    int32  sum2[3] = {0, 0, 0};
    int16  avg[3]  = {0, 0, 0};
    uint16 n2      = 0;

    imu_ok      = 0;
    imu_yaw_deg = 0.0f;
    imu_zero[0] = 0.0f;
    imu_zero[1] = 0.0f;
    imu_zero[2] = 0.0f;
    imu_last_ms = system_getval_ms();

    if(0 != imu660rb_init())
    {
        return;                                     // 没接模块 / 通信失败
    }

    // 先静置一会儿再采样：舵机刚初始化完会抖，抖着校出来的零偏是错的
    system_delay_ms(IMU_ZERO_SETTLE_MS);

    // ---------------- 第一遍：粗平均 ----------------
    for(i = 0; i < IMU_ZERO_SAMPLES; i++)
    {
        imu660rb_get_gyro();
        sum[0] += (int32)imu660rb_gyro_x;
        sum[1] += (int32)imu660rb_gyro_y;
        sum[2] += (int32)imu660rb_gyro_z;
        system_delay_ms(IMU_ZERO_DELAY_MS);
    }

    avg[0] = (int16)(sum[0] / (int32)IMU_ZERO_SAMPLES);
    avg[1] = (int16)(sum[1] / (int32)IMU_ZERO_SAMPLES);
    avg[2] = (int16)(sum[2] / (int32)IMU_ZERO_SAMPLES);

    // ---------------- 第二遍：去掉离群的采样再平均 ----------------
    for(i = 0; i < IMU_ZERO_SAMPLES; i++)
    {
        int16 gx, gy, gz;

        imu660rb_get_gyro();
        gx = imu660rb_gyro_x;
        gy = imu660rb_gyro_y;
        gz = imu660rb_gyro_z;

        if(   (gx > (int16)(avg[0] - IMU_ZERO_TRIM)) && (gx < (int16)(avg[0] + IMU_ZERO_TRIM))
           && (gy > (int16)(avg[1] - IMU_ZERO_TRIM)) && (gy < (int16)(avg[1] + IMU_ZERO_TRIM))
           && (gz > (int16)(avg[2] - IMU_ZERO_TRIM)) && (gz < (int16)(avg[2] + IMU_ZERO_TRIM)))
        {
            sum2[0] += (int32)gx;
            sum2[1] += (int32)gy;
            sum2[2] += (int32)gz;
            n2++;
        }

        system_delay_ms(IMU_ZERO_DELAY_MS);
    }

    if(0 == n2)
    {
        n2 = 1;                                     // 一个都没留下（抖动太大），退化成 0 偏，不要除零
        sum2[0] = 0;
        sum2[1] = 0;
        sum2[2] = 0;
    }

    imu_zero[0] = imu660rb_gyro_transition((int16)(sum2[0] / (int32)n2));
    imu_zero[1] = imu660rb_gyro_transition((int16)(sum2[1] / (int32)n2));
    imu_zero[2] = imu660rb_gyro_transition((int16)(sum2[2] / (int32)n2));

    imu_ok = 1;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      周期调用：读陀螺仪 -> 扣零偏 -> 死区 -> 积分航向
// 备注信息      现在由 user/isr.c 的 cc60_pit_ch0_isr 每 20ms 调一次，采样均匀。
//               内部用【实际 dt】积分，所以万一调用周期变了也不会算错。
//-------------------------------------------------------------------------------------------------------------------
void imu_update (void)
{
    uint32  now_ms;
    float32 dt;
    float32 rate;
    uint8   i;

    if(0 == imu_ok)
    {
        return;
    }

    now_ms = system_getval_ms();
    dt     = (float32)(now_ms - imu_last_ms) * 0.001f;
    imu_last_ms = now_ms;

    if((dt <= 0.0f) || (dt > 0.2f))
    {
        return;                                     // 时间没走 / 卡了太久，这一拍丢掉，免得积出一个跳变
    }

    imu660rb_get_gyro();

    imu_gyro_dps[0] = imu660rb_gyro_transition(imu660rb_gyro_x) - imu_zero[0];
    imu_gyro_dps[1] = imu660rb_gyro_transition(imu660rb_gyro_y) - imu_zero[1];
    imu_gyro_dps[2] = imu660rb_gyro_transition(imu660rb_gyro_z) - imu_zero[2];

    for(i = 0; i < 3; i++)
    {
        if((imu_gyro_dps[i] < IMU_GYRO_DEADBAND) && (imu_gyro_dps[i] > -IMU_GYRO_DEADBAND))
        {
            imu_gyro_dps[i] = 0.0f;                 // 死区：零偏残留不参与积分，否则航向会一直漂
        }
    }

#if   (IMU_YAW_AXIS == IMU_YAW_AXIS_X)
    rate = imu_gyro_dps[0];
#elif (IMU_YAW_AXIS == IMU_YAW_AXIS_Y)
    rate = imu_gyro_dps[1];
#else
    rate = imu_gyro_dps[2];
#endif

    imu_yaw_deg += IMU_YAW_DIR * IMU_YAW_SCALE * rate * dt;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      把航向角置成指定值（比如进元素前清零，方便判"转过多少度"）
//-------------------------------------------------------------------------------------------------------------------
void imu_reset_yaw (float32 deg)
{
    imu_yaw_deg = deg;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      初始化是否成功（1 = 成功）
//-------------------------------------------------------------------------------------------------------------------
uint8 imu_is_ok (void)
{
    return imu_ok;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      读航向角（度）
//-------------------------------------------------------------------------------------------------------------------
float32 imu_get_yaw_deg (void)
{
    return imu_yaw_deg;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      读偏航角速度（度/秒，已按 IMU_YAW_DIR / IMU_YAW_SCALE 修正）
//-------------------------------------------------------------------------------------------------------------------
float32 imu_get_yaw_rate (void)
{
#if   (IMU_YAW_AXIS == IMU_YAW_AXIS_X)
    return imu_gyro_dps[0] * IMU_YAW_DIR * IMU_YAW_SCALE;
#elif (IMU_YAW_AXIS == IMU_YAW_AXIS_Y)
    return imu_gyro_dps[1] * IMU_YAW_DIR * IMU_YAW_SCALE;
#else
    return imu_gyro_dps[2] * IMU_YAW_DIR * IMU_YAW_SCALE;
#endif
}