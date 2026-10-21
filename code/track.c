/*********************************************************************************************************************
* 文件名称          track.c
* 功能简介          循迹控制：非线性 P+D 转向 + 弯道减速 + 几何电子差速
*********************************************************************************************************************/

#include "track.h"
#include "image.h"
#include "camera.h"
#include "servo.h"
#include "motor.h"
#include "ring.h"

//==================================================== 内部变量 ====================================================
static track_state_enum track_state        = TRACK_STATE_WAIT;
static float32 track_base_speed            = TRACK_SPEED_STRAIGHT;   // 直道基础速度
static float32 track_cur_speed             = 0.0f;                   // 减速 + 斜坡之后的基础速度
static float32 track_last_error            = 0.0f;                   // 上一帧的偏差，算 D 用
static float32 track_error_lpf            = 0.0f;                   // 偏差低通，减速用
static int16   track_steer                = 0;                      // 本帧打角量（舵机占空比偏移）
static float32 track_curve_ratio           = 1.0f;
static float32 track_left_target           = 0.0f;
static float32 track_right_target          = 0.0f;
static uint16  track_lost_count            = 0;
static uint32  track_start_ms              = 0;

// ---- 运行时可改参数（VOFA+ 在线调参用，初值就是 track.h 里的宏）----
static float32 track_steer_kp             = TRACK_STEER_KP;
static float32 track_steer_kp2            = TRACK_STEER_KP2;
static float32 track_steer_kd             = TRACK_STEER_KD;
static float32 track_curve_gain           = TRACK_CURVE_GAIN;
static float32 track_curve_min            = TRACK_CURVE_MIN_RATIO;
static float32 track_curve_ref            = TRACK_CURVE_ERR_REF;
static float32 track_diff_gain            = TRACK_DIFF_GAIN;

//==================================================== 内部函数 ====================================================
//-------------------------------------------------------------------------------------------------------------------
// 函数简介      舵机占空比偏移 -> 前轮转角（度，右转为正）
// 参数说明      offset          相对中值的偏移（占空比单位）
// 备注信息      跟参考工程 scheduler.c 里的 steer_duty_to_angle() 一个思路：
//               偏移为负（占空比变小）算右转，两边各自按自己的行程归一化
//-------------------------------------------------------------------------------------------------------------------
static float32 track_offset_to_angle (int16 offset)
{
    if(offset <= 0)
    {
        return (float32)(-(int32)offset) * TRACK_DIFF_ANGLE_MAX
             / (float32)(SERVO_MID_DUTY - SERVO_MIN_DUTY);
    }

    return -(float32)offset * TRACK_DIFF_ANGLE_MAX
         / (float32)(SERVO_MAX_DUTY - SERVO_MID_DUTY);
}

//==================================================== 对外接口 ====================================================
//-------------------------------------------------------------------------------------------------------------------
// 函数简介      循迹模块初始化：初始化舵机、把状态机复位、电机停住
//-------------------------------------------------------------------------------------------------------------------
void track_init (void)
{
    servo_init();

    track_state        = TRACK_STATE_WAIT;
    track_base_speed   = TRACK_SPEED_STRAIGHT;
    track_cur_speed    = 0.0f;
    track_last_error   = 0.0f;
    track_error_lpf    = 0.0f;
    track_steer        = 0;
    track_curve_ratio  = 1.0f;
    track_left_target  = 0.0f;
    track_right_target = 0.0f;
    track_lost_count   = 0;
    track_start_ms     = system_getval_ms();

    motor_stop_all();
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      手动启动 / 停车
//-------------------------------------------------------------------------------------------------------------------
void track_start (void)
{
    track_state        = TRACK_STATE_RUN;
    track_cur_speed    = 0.0f;
    track_last_error   = (float32)image_error;
    track_error_lpf    = 0.0f;
    track_lost_count   = 0;
}

void track_stop (void)
{
    track_state = TRACK_STATE_LOST;
    motor_stop_all();
    servo_set_duty(SERVO_MID_DUTY);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      循迹控制主函数：拿到新的一帧图像之后调用一次
// 备注信息      做四件事：判断该不该跑 -> 算打角给舵机 -> 算弯道减速后的基础速度 -> 按几何分配两轮速度
//-------------------------------------------------------------------------------------------------------------------
void track_update (void)
{
    float32 err;
    float32 abs_err;
    float32 p_term;
    float32 d_term;
    float32 steer;
    float32 ratio;
    float32 base;
    float32 angle;
    float32 k;
    float32 left;
    float32 right;
    uint8   run = 0;

    // ==================== 1. 判断现在该不该跑 ====================
    if(0 == camera_is_ok())
    {
        track_state = TRACK_STATE_NO_CAM;
    }

    switch(track_state)
    {
        case TRACK_STATE_WAIT:
            // 先停够 TRACK_START_DELAY_MS，然后等一帧"看得到赛道"的图像才起跑
            if((system_getval_ms() - track_start_ms) >= TRACK_START_DELAY_MS)
            {
                if(0 == image_off_track)
                {
                    track_state       = TRACK_STATE_RUN;
                    track_cur_speed   = 0.0f;
                    track_last_error  = (float32)image_error;
                    track_error_lpf   = 0.0f;
                    track_lost_count  = 0;
                    run               = 1;
                }
            }
            break;

        case TRACK_STATE_RUN:
            if(image_off_track)
            {
                track_lost_count++;
                if(track_lost_count >= TRACK_LOST_STOP_FRAMES)
                {
                    track_state = TRACK_STATE_LOST;
                }
            }
            else
            {
                track_lost_count = 0;
            }
            run = (TRACK_STATE_RUN == track_state) ? 1 : 0;
            break;

        default:
            run = 0;
            break;
    }

    // ==================== 2. 不跑的时候：舵机回中、电机立刻停 ====================
    if(0 == run)
    {
        servo_set_duty(SERVO_MID_DUTY);
        track_steer        = 0;
        track_cur_speed    = 0.0f;
        track_curve_ratio  = 1.0f;
        track_left_target  = 0.0f;
        track_right_target = 0.0f;

        motor_stop_all();                           // 直接停，不走 PID 斜坡
        return;
    }

    err     = (float32)image_error;
    abs_err = (err >= 0.0f) ? err : -err;

    // ==================== 3. 方向环：非线性 P + 视觉 D ====================
    // 【环岛里换一套舵机参数】照 E07 的 duoji_pid()：进了环 P 放大约 19%、D 不变、I 清零。
    //   E07 正常 P=0.32，进环 P=0.38；本工程按同样比例放大 KP 和 KP2（数值在 ring.h 里）。
    if(ring_is_active())
    {
        p_term = RING_STEER_KP  * err
               + RING_STEER_KP2 * err * abs_err;
        d_term = RING_STEER_KD  * (err - track_last_error);
    }
    else
    {
        p_term = track_steer_kp  * err
               + track_steer_kp2 * err * abs_err;    // 小偏差几乎不动 -> 直道不蛇形
        d_term = track_steer_kd  * (err - track_last_error);
    }
    track_last_error = err;

    // D 项限幅：丢帧会让差分间隔从 20ms 变成 40ms，D 会翻倍，压一下
    if(d_term >  TRACK_STEER_KD_MAX) { d_term =  TRACK_STEER_KD_MAX; }
    if(d_term < -TRACK_STEER_KD_MAX) { d_term = -TRACK_STEER_KD_MAX; }

    steer = p_term + d_term;

    if(steer >  TRACK_STEER_MAX) { steer =  TRACK_STEER_MAX; }
    if(steer < -TRACK_STEER_MAX) { steer = -TRACK_STEER_MAX; }

    servo_set_offset((int16)(TRACK_STEER_DIR * steer));

    // 屏幕上的 SDR 显示【真正加到舵机上的偏移】（已经被 SERVO_MIN/MAX_DUTY 夹过），
    // 这样 SDR 和 DTY 永远对得上，饱和了一眼就能看出来。
    track_steer = servo_offset();

    // ==================== 4. 弯道减速 ====================
    track_error_lpf += TRACK_CURVE_ERR_LPF * (abs_err - track_error_lpf);

    ratio = track_error_lpf / track_curve_ref;
    if(ratio > 1.0f) { ratio = 1.0f; }

    ratio = 1.0f - track_curve_gain * ratio;
    if(ratio < track_curve_min) { ratio = track_curve_min; }
    track_curve_ratio = ratio;

    base = track_base_speed * ratio;

    // 【环岛限速】照 E07 的 yvan_speed：绕环时把目标速度压到上限以内。
    // 环里是持续大角度，速度高了根本贴不住内圈 —— E07 也是单独给环岛一条限速。
    if(ring_is_active() && (base > RING_SPEED_MAX)) { base = RING_SPEED_MAX; }

    // 目标速度斜坡：升速用 TRACK_SPEED_RAMP（防起步打滑），
    // 降速用 TRACK_SPEED_RAMP_DOWN（进弯要刹得快，否则直道越快冲进弯里越快）
    if(base > (track_cur_speed + TRACK_SPEED_RAMP))
    {
        base = track_cur_speed + TRACK_SPEED_RAMP;
    }
    else if(base < (track_cur_speed - TRACK_SPEED_RAMP_DOWN))
    {
        base = track_cur_speed - TRACK_SPEED_RAMP_DOWN;
    }
    track_cur_speed = base;

    // ==================== 5. 电子差速：按前轮转角几何分配内外轮 ====================
    // 注意这里用的是舵机【实际输出】的偏移，而不是 PID 的中间量，
    // 所以舵机被限幅时差速也跟着被限，两者永远是一致的。
    angle = track_offset_to_angle(servo_offset());   // 右转为正

    k = track_diff_gain
      * (TRACK_DIFF_TRACK_MM / (2.0f * TRACK_DIFF_WHEELBASE_MM))
      * tanf(((angle >= 0.0f) ? angle : -angle) * 0.01745329f);

    if(k > TRACK_DIFF_RATIO_MAX) { k = TRACK_DIFF_RATIO_MAX; }

    if(angle >= 0.0f)                                // 右转：右轮是内轮，要慢
    {
        left  = base * (1.0f + k);
        right = base * (1.0f - k);
    }
    else                                             // 左转：左轮是内轮，要慢
    {
        left  = base * (1.0f - k);
        right = base * (1.0f + k);
    }

    if(left  < 0.0f) { left  = 0.0f; }
    if(right < 0.0f) { right = 0.0f; }

    track_left_target  = left;
    track_right_target = right;

    // ==================== 6. 输出到两路电机 ====================
    if(TRACK_MOTOR1_IS_LEFT)
    {
        motor_set_speed(1, left);
        motor_set_speed(2, right);
    }
    else
    {
        motor_set_speed(1, right);
        motor_set_speed(2, left);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      几个给屏幕用的查询接口
//-------------------------------------------------------------------------------------------------------------------
uint8 track_is_running (void)
{
    return (TRACK_STATE_RUN == track_state) ? 1 : 0;
}

void track_set_speed (float32 speed)
{
    if(speed < 0.0f)     { speed = 0.0f; }
    if(speed > 1000.0f)  { speed = 1000.0f; }

    track_base_speed = speed;
}

float32 track_get_speed (void)
{
    return track_base_speed;
}

int16 track_get_steer (void)
{
    return track_steer;
}

float32 track_get_curve_ratio (void)
{
    return track_curve_ratio;
}

uint8 track_get_state (void)
{
    return (uint8)track_state;
}

float32 track_get_left_target (void)
{
    return track_left_target;
}

float32 track_get_right_target (void)
{
    return track_right_target;
}

//==================================================== 在线调参（VOFA+ 用）============================================
//-------------------------------------------------------------------------------------------------------------------
// 函数简介      在线改转向参数
// 参数说明      kp / kp2 / kd   对应 TRACK_STEER_KP / TRACK_STEER_KP2 / TRACK_STEER_KD
// 备注信息      内部限幅；环岛里用的是 RING_STEER_* 那一套，不受这里影响
//-------------------------------------------------------------------------------------------------------------------
void track_set_steer_param (float32 kp, float32 kp2, float32 kd)
{
    if(kp  < 0.0f) { kp  = 0.0f; }
    if(kp  > 10.0f) { kp  = 10.0f; }
    if(kp2 < 0.0f) { kp2 = 0.0f; }
    if(kp2 > 1.0f) { kp2 = 1.0f; }
    if(kd  < 0.0f) { kd  = 0.0f; }
    if(kd  > 5.0f) { kd  = 5.0f; }

    track_steer_kp  = kp;
    track_steer_kp2 = kp2;
    track_steer_kd  = kd;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      在线改弯道减速参数
// 参数说明      min_ratio       最低速度比例（0.05 ~ 1.0）
// 参数说明      err_ref         减速参考偏差（5 ~ 200 像素）
// 备注信息      GAIN 不单独给，直接按 1 - min_ratio 算，保证两者永远对齐
//-------------------------------------------------------------------------------------------------------------------
void track_set_curve_param (float32 min_ratio, float32 err_ref)
{
    if(min_ratio < 0.05f)  { min_ratio = 0.05f; }
    if(min_ratio > 1.0f)   { min_ratio = 1.0f; }
    if(err_ref   < 5.0f)   { err_ref   = 5.0f; }
    if(err_ref   > 200.0f) { err_ref   = 200.0f; }

    track_curve_min  = min_ratio;
    track_curve_gain = 1.0f - min_ratio;
    track_curve_ref  = err_ref;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      在线改电子差速增益（0 = 关掉差速）
//-------------------------------------------------------------------------------------------------------------------
void track_set_diff_gain (float32 gain)
{
    if(gain < 0.0f) { gain = 0.0f; }
    if(gain > 3.0f) { gain = 3.0f; }

    track_diff_gain = gain;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      逐项改转向参数（旋钮用）：只动一项，其余不变
// 参数说明      term            0 = Kp，1 = Kp2，2 = Kd
// 参数说明      value           新的值（内部限幅）
//-------------------------------------------------------------------------------------------------------------------
void track_set_steer_term (uint8 term, float32 value)
{
    switch(term)
    {
        case 0:
            if(value < 0.0f) { value = 0.0f; }
            if(value > 10.0f) { value = 10.0f; }
            track_steer_kp = value;
            break;
        case 1:
            if(value < 0.0f) { value = 0.0f; }
            if(value > 1.0f) { value = 1.0f; }
            track_steer_kp2 = value;
            break;
        case 2:
            if(value < 0.0f) { value = 0.0f; }
            if(value > 5.0f) { value = 5.0f; }
            track_steer_kd = value;
            break;
        default:
            break;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      逐项改弯道参数（旋钮用）：只动一项，其余不变
// 参数说明      term            0 = 最低速度比例，1 = 减速参考偏差
// 备注信息      改比例时 GAIN 自动按 1 - 比例 重算，和 track_set_curve_param 一致
//-------------------------------------------------------------------------------------------------------------------
void track_set_curve_term (uint8 term, float32 value)
{
    if(0 == term)
    {
        if(value < 0.05f) { value = 0.05f; }
        if(value > 1.0f)  { value = 1.0f; }

        track_curve_min  = value;
        track_curve_gain = 1.0f - value;
    }
    else if(1 == term)
    {
        if(value < 5.0f)   { value = 5.0f; }
        if(value > 200.0f) { value = 200.0f; }

        track_curve_ref = value;
    }
}
