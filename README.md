# E01_gpio_demo — TC377 摄像头循迹智能车

> 目录名沿用了 E01 模板（`E01_gpio_demo`），实际内容是一辆**基于 Infineon TC377 + 逐飞开源库的摄像头循迹小车**。
> 原始 GPIO 流水灯例程还留在 `user/cpu0_main_原文件备份.c.old`，可以对照看出改造起点。
>
> 本文档由源码与实车注释整理，用来替代散落在各文件头部的说明。源码为 **GBK 编码**，用 UTF-8 工具打开会乱码。

---

## 目录

1. [项目概况](#1-项目概况)
2. [硬件与接线](#2-硬件与接线)
3. [目录结构](#3-目录结构)
4. [软件架构与时序](#4-软件架构与时序)
5. [控制算法](#5-控制算法)
6. [参数速查](#6-参数速查)
7. [屏幕读数说明](#7-屏幕读数说明)
8. [编译、烧录与调试](#8-编译烧录与调试)
9. [建议的调参顺序](#9-建议的调参顺序)
10. [已知问题与注意事项](#10-已知问题与注意事项)
11. [VOFA+ 串口调参](#11-vofa-串口调参)

---

## 1. 项目概况

| 项目 | 内容 |
|---|---|
| 主控 | Infineon TC377TP（TriCore 三核，本工程实际只用 CPU0） |
| 开发环境 | AURIX Development Studio 1.10.40，TASKING v1.1r8 编译器 |
| 基础库 | 逐飞 TC377 开源库 V3.4.5 + Infineon iLLD TC37A |
| 传感器 | 总钻风 MT9V03X 灰度摄像头（188×120）、两路方向+计数编码器、IMU660RB 陀螺仪 |
| 执行器 | 两路直流电机（PWM+DIR 驱动，速度闭环）、一路转向舵机 |
| 人机界面 | IPS200 Pro 2.0 寸 SPI 智能屏（竖屏 240×320），实时显示图像与调试量 |
| 功能 | 摄像头巡线、弯道减速、电子差速、十字通过、环岛识别与绕行、摄像头掉线保护 |

**核心思路：** 速度环跑在 20 ms 定时中断里，主循环只做图像处理与决策（约 50 帧/秒），刷屏再慢也不会打乱控制节拍。

---

## 2. 硬件与接线

| 外设 | 引脚 / 接口 | 说明 |
|---|---|---|
| 电机 1 | PWM `P21_3`（ATOM0_CH1）、DIR `P21_2` | DIR 低电平正转；电机 1 = **左后轮**（实车验证） |
| 电机 2 | PWM `P21_5`（ATOM0_CH3）、DIR `P21_4` | DIR 低电平正转 |
| 编码器 1 | 计数 `P10_3`、方向 `P10_1` | 定时器 T5，方向+计数模式 |
| 编码器 2 | 计数 `P20_3`、方向 `P20_0` | 定时器 T6 |
| 转向舵机 | PWM `P33_9` | **必须用 `ATOM1_CH1_P33_9`**，不能用 ATOM0_CH1（与电机 1 同通道） |
| 摄像头 MT9V03X | PCLK `P02_1`、VSY `P02_0`、D0~D7 `P00_0~P00_7`、配置串口 `P02_2/P02_3` | PCLK+ERU 触发 DMA 搬运 |
| IPS200 Pro | SCLK `P15_3`、MOSI `P15_5`、CS `P15_2`、RES `P15_1`、DC `P15_0`、BL `P15_4` | SPI2 |
| IMU660RB | SCLK `P20_11`、MOSI `P20_14`、MISO `P20_12`、CS `P20_13` | 硬件 SPI0 |

**不要使用的引脚**（boot 引脚，误用可能导致无法启动）：`P14_2`、`P14_3`、`P14_4`、`P14_5`、`P14_6`、`P10_5`、`P10_6`。

---

## 3. 目录结构

```
E01_gpio_demo/
├── code/                       ← 应用层（用户代码都放这里）
│   ├── camera.c/.h             摄像头采集、双缓冲防撕裂、掉线计时
│   ├── image.c/.h              二值化 → 边线 → 补线 → 偏差
│   ├── track.c/.h              方向环 + 弯道减速 + 电子差速
│   ├── motor.c/.h              双路速度闭环（前馈 + PI、斜坡、堵转）
│   ├── servo.c/.h              舵机 PWM、限位保护、标定扫描
│   ├── encoder.c/.h            编码器读取与低通测速
│   ├── imu.c/.h                IMU660RB 偏航角速度 → 航向角
│   ├── ring.c/.h               环岛 7 状态机 + 补线
│   ├── display.c/.h            IPS200 Pro 显示
│   ├── vofa.c/.h               VOFA+ JustFloat 波形 + 串口在线调参
│   └── zf_common_headfile.h    转发头（见“已知问题”第 1 条）
├── user/                       ← 入口与中断
│   ├── cpu0_main.c             初始化顺序 + 50 Hz 主循环
│   ├── isr.c                   20 ms PIT、摄像头场/DMA/串口中断
│   ├── isr_config.h            中断服务核、优先级、向量号
│   ├── cpu1_main.c / cpu2_main.c   空循环（未使用）
│   └── cpu0_main_原文件备份.c.old   改造前的 GPIO 例程
├── libraries/                  ← 逐飞库 + iLLD（不要改）
├── tools/vofa_tune.py          VOFA+ 备用上位机（Python + pyserial）
├── Lcf_Tasking_Tricore_Tc.lsl  链接脚本
├── .project / .cproject        ADS 工程文件
├── 推荐IO分配.txt / 尽量不要使用的引脚.txt
└── Debug/                      构建产物（**来自另一台机器，需重新编译**）
```

---

## 4. 软件架构与时序

### 4.1 启动顺序

```
clock_init()                 关看门狗、系统延时
cpu_wait_event_ready()       等其它核
display_init()               屏幕（SPI2）
encoder_init() + motor_init() + motor_stop_all()
servo_init()                 舵机回中
imu_init()                   静止校零：静置 300 ms + 300×2 ms 采样（约 1 s，车必须静止）
camera_setup()               摄像头（内部 200 ms 延时 + 串口协商，最多重试 3 次）
image_init() + track_init()
pit_ms_init(CCU60_CH0, 20)    最后才开定时中断
```

### 4.2 三个执行上下文

| 上下文 | 周期 | 频率 | 做什么 |
|---|---|---|---|
| `cc60_pit_ch0_isr`（优先级 50） | 20 ms | 50 Hz | `encoder_update()` → `imu_update()` → `motor_control_update()`（双路 PID + PWM） |
| 主循环 | 跟图像帧 | ≈50 Hz | `camera_update()` → `image_process()` → `track_update()` |
| 主循环（分频） | 150 ms | ≈6.7 Hz | `display_update_track()` 刷屏 |
| DMA 完成中断（优先级 70） | 每帧 | 50 Hz | 22 KB 整帧 `memcpy` 到 `camera_image`（此刻 DMA 已关，不会撕裂） |
| 场中断 VSY（优先级 62） | 每帧 | 50 Hz | 启动一帧 DMA |

> 中断优先级：DMA(70) > VSY(62) > 20 ms 控制(50)。DMA 拷帧约 0.1~0.2 ms，不会打乱控制周期。

### 4.3 数据流

```
camera_image[120][188]
        │  Otsu 阈值 + 二值化
        ▼
   image_bin[][]  ──►  逐行扫描  ──►  left/right/center_line[]
                                          │
                                          ▼
                                     image_error（像素，正 = 需右转）
                                          │
                        ring_update()（可选改写中线并重算偏差）
                                          ▼
                                    track_update()
                          ┌───────────────┴───────────────┐
                          ▼                               ▼
                  servo_set_offset()              motor_set_speed(1/2)
                          │                               │
                  舵机 PWM（50 Hz）              20 ms 中断里的速度环
```

---

## 5. 控制算法

### 5.1 摄像头与图像

- 188×120 灰度，一帧约 3.8 ms，帧率约 50 fps。
- **双缓冲**：DMA 采完立即在中断里整帧拷到 `camera_image`，图像算法只读这一份，避免“上半帧新、下半帧旧”的白条纹。
- **大津法自适应阈值**，并夹在 `110 ~ 200` 之间：防止背景亮度落在第三档时把背景误判成赛道。
- **逐行扫描**：从第 119 行往上、隔行扫（共 58 行），找左右边线；只找到一侧时按透视补出另一侧。
- **偏差 = 中线 − 图像中心**，加权平均且带**有限前瞻**：前瞻行 `y=56`，更远的行在 28 行内权重线性降到 0，再远完全不参与。这是治“把远处赛道也巡进来”的关键。
- **元素行**（十字 / 环岛）判据：① 整行无边线；② 本行比下一行反而变宽；③ 边线外侧还有成片白点；④ 左右边线同时往外挪。用上下有效行的中线插值把元素行补成连续中线，避免过十字时被判丢线停车。
- **十字**：连续若干元素行 + 仍有足够有效行 → 置十字标志，期间不算丢赛道；超过 60 帧仍不退出则按真丢赛道处理。

### 5.2 循迹（方向环 + 速度决策）

转向采用**非线性 P + 视觉 D**：

```
steer = KP·e + KP2·e·|e| + KD·(e − e_last)
```

- 小偏差时平方项几乎为 0 → 直道不蛇形；大偏差时迅速增大 → 急弯跟得上。
- D 项单独限幅 30：转向环跟图像帧跑，刷屏会导致偶发 40 ms 间隔，D 会翻倍。
- 打角硬限位由舵机侧唯一决定（`servo_set_offset()` 夹在机械限位内），这里只做 300 的软限幅。

弯道减速：

```
ratio = 1 − 0.745 × min(1, 低通偏差 / 40)     # 最低 0.255
base  = 直道速度 × ratio                      # 600 × 0.255 ≈ 153
```

- 升速斜坡 17/帧（防起步打滑），降速斜坡 82/帧（进弯要刹得快）。
- 偏差低通 0.3，避免速度忽快忽慢。

电子差速（按前轮转角几何分配，不是按偏差线性给）：

```
k = 1.35 × 轮距 / (2 × 轴距) × tan(|前轮角|)     # 上限 0.66
内轮 = base × (1 − k)      外轮 = base × (1 + k)
```

### 5.3 电机速度环

- 被控量：**脉冲 / 20 ms**；输出：0~10000 占空比。
- 开环实测拟合：`占空比 = 死区 + 2.36 × 目标速度`，电机 1 死区 386、电机 2 死区 644。
- **前馈 + PI**：前馈先给到基本正确的占空比，PI 只在 ±1500 内小修正，避开“PID 输出必须跨过死区”的来回翻。
- 输出斜坡 500 / 800，积分限幅 ±3000，积分分离阈值 40。
- 堵转保护：占空比 > 4000 且速度 < 2 连续 25 个周期（0.5 s）置堵转标志。

### 5.4 舵机

- 50 Hz（模拟舵机不能调高），1 个占空比单位 = 2 µs。
- 中值 770 = 前轮正前方；实测机械限位：小侧 590 起叫、大侧 920 起叫。
- 左右行程**不对称**：`600 ~ 770 ~ 910`，即 170 / 140，转角换算按各自行程归一化。
- `SERVO_SWEEP_ENABLE = 1` 时进入硬限位扫描模式（车不会动），用于换舵机后重新标定。

### 5.5 陀螺仪

- IMU660RB，硬件 SPI0；偏航轴 = Z，右转 YAW 变负。
- 20 ms 均匀采样，扣零偏 + 1.5 °/s 死区后积分；比例系数实测 1.0。
- 上电校零时**车必须静止**，且要放在舵机初始化之后（舵机动作会晃）。

### 5.6 环岛

- 从 E07 工程移植的 **7 状态机**：入环 → 环中 → 找拐点 → 出环特征 → 陀螺仪再转 50°/40° → 左右边线回归 → 冷却。
- 起环判据**没有照抄** E07 的“某行丢边”：实测本车相机视场较窄，右环入口右边线根本跑不出画面。改用 `image_ring_side`（逐行“单侧伸出去”的证据）+ 路宽 > 135 + 视野长度 ≥ 36。
- 环内换一套舵机参数（KP 1.6→1.9，KP2 0.035→0.042），并限速 138。
- 兜底：绕环总计时 600 帧（约 12 s）无条件复位；出环后冷却 250 帧（约 5 s），防止同一个环被重复识别。

---

## 6. 参数速查

> 改车 90% 的时间只动这张表里的宏。改完重新编译烧录即可。

### 6.1 转向 / 速度（`code/track.h`）

| 宏 | 当前值 | 含义 |
|---|---|---|
| `TRACK_STEER_KP` | `1.6f` | 线性比例 |
| `TRACK_STEER_KP2` | `0.035f` | 非线性平方项（弯道打角不够调大，直道蛇形调小） |
| `TRACK_STEER_KD` | `0.45f` | 视觉微分 |
| `TRACK_STEER_KD_MAX` | `30.0f` | D 项限幅 |
| `TRACK_STEER_MAX` | `300.0f` | 打角软限幅（比舵机行程大即可） |
| `TRACK_STEER_DIR` | `-1.0f` | 转向反了就改符号 |
| `TRACK_SPEED_STRAIGHT` | `600.0f` | 直道基础速度（脉冲/20 ms） |
| `TRACK_SPEED_RAMP` | `17.0f` | 每帧最大升速 |
| `TRACK_SPEED_RAMP_DOWN` | `82.0f` | 每帧最大降速 |
| `TRACK_CURVE_GAIN` | `0.745f` | 必须等于 `1 − MIN_RATIO` |
| `TRACK_CURVE_MIN_RATIO` | `0.255f` | 最低速度比例（600 × 0.255 ≈ 153） |
| `TRACK_CURVE_ERR_REF` | `40.0f` | 偏差到 40 像素即减到底 |
| `TRACK_CURVE_ERR_LPF` | `0.30f` | 偏差低通系数 |
| `TRACK_DIFF_GAIN` | `1.35f` | 电子差速增益（0 = 关闭） |
| `TRACK_DIFF_WHEELBASE_MM` | `145.0f` | 轴距 |
| `TRACK_DIFF_TRACK_MM` | `156.0f` | 后轮轮距 |
| `TRACK_DIFF_RATIO_MAX` | `0.66f` | 差速比例上限 |
| `TRACK_DIFF_ANGLE_MAX` | `42.0f` | 满打前轮转角（实测 R=160 mm → 42.2°） |
| `TRACK_MOTOR1_IS_LEFT` | `1` | 电机 1 = 左轮；接反了差速会把车往弯外推 |
| `TRACK_START_DELAY_MS` | `3000` | 上电停 3 s（还要等到一帧能看见赛道） |
| `TRACK_LOST_STOP_FRAMES` | `30` | 连续丢线 30 帧（约 0.6 s）停车 |

### 6.2 图像（`code/image.h`）

| 宏 | 当前值 | 含义 |
|---|---|---|
| `IMAGE_USE_OTSU` | `1` | 大津法自适应阈值 |
| `IMAGE_FIXED_THRESHOLD` | `128` | 关闭 Otsu 时的固定阈值 |
| `IMAGE_THRESHOLD_MIN / MAX` | `110 / 200` | 阈值静态下限 / 上限 |
| `IMAGE_SCAN_TOP` | `4` | 扫到第 4 行停 |
| `IMAGE_SCAN_STEP` | `2` | 隔行扫描（共 58 行） |
| `IMAGE_TRACK_MODE` | `EDGE` | 白底黑边（智能车赛道）；`LINE` = 单线循迹 |
| `IMAGE_ALLOW_SINGLE_EDGE` | `1` | 单边丢失时补另一边（弯道关键） |
| `IMAGE_LOOKAHEAD_ROW` | `56` | 前瞻行（偏差最看重这一行） |
| `IMAGE_LOOKAHEAD_FADE` | `28` | 更远的行在 28 行内渐隐到 0 |
| `IMAGE_ERROR_W_MAX` | `40` | 加权平均权重上限 |
| `IMAGE_MIN_VALID_ROWS` | `6` | 有效行少于此数判丢赛道 |
| `IMAGE_CROSS_ROW_MIN` | `3` | 十字元素行门槛 |
| `IMAGE_RING_ROW_MIN` | `3` | 环岛单侧证据行数门槛 |
| `IMAGE_CROSS_MAX_FRAMES` | `60` | 十字最多持续帧数 |

### 6.3 电机（`code/motor.c`）

| 宏 | 当前值 | 含义 |
|---|---|---|
| `MOTOR_PWM_FREQ` | `10000` | PWM 10 kHz |
| `MOTOR_KP / KI / KD` | `2.0 / 0.5 / 0.0` | PID 默认参数 |
| `MOTOR_OUT_MAX` | `10000` | 占空比满量程 |
| `MOTOR_FF_ENABLE` | `1` | 前馈 + PI（0 = 纯 PID + 死区补偿） |
| `MOTOR_FF_DUTY_PER_SPEED` | `2.36f` | 每单位速度所需占空比 |
| `MOTOR_1_FF_DEADZONE` | `386.0f` | 电机 1 静摩擦死区 |
| `MOTOR_2_FF_DEADZONE` | `644.0f` | 电机 2 静摩擦死区 |
| `MOTOR_FF_PID_MAX` | `1500.0f` | PID 修正量限幅 |
| `MOTOR_RAMP_UP / DOWN` | `500 / 800` | 每周期占空比变化上限 |
| `MOTOR_STALL_DUTY / SPEED / COUNT` | `4000 / 2 / 25` | 堵转判据 |
| `MOTOR_OPEN_LOOP_TEST` | `0` | 1 = 开环标定（车不受控） |

> 改了目标速度的量级后，`KP` 要跟着缩放：被控对象增益约 0.425 脉冲/占空比。

### 6.4 舵机（`code/servo.h`）

| 宏 | 当前值 | 含义 |
|---|---|---|
| `SERVO_PWM_PIN` | `ATOM1_CH1_P33_9` | 不能改成 ATOM0_CH1 |
| `SERVO_PWM_FREQ` | `50` | 模拟舵机 50 Hz |
| `SERVO_MID_DUTY` | `770` | 前轮正前方（实测） |
| `SERVO_LOCK_LOW / HIGH` | `660 / 880` | 实测机械限位（590 起叫 / 920 起叫，往里留余量） |
| `SERVO_MIN_DUTY / MAX_DUTY` | `600 / 910` | 实际输出行程 |
| `SERVO_TRAVEL_LOW / HIGH` | `170 / 140` | 两侧行程（不对称是正常的） |
| `SERVO_SWEEP_ENABLE` | `0` | 1 = 硬限位扫描（车不会动） |

### 6.5 环岛（`code/ring.h`）

| 宏 | 当前值 | 含义 |
|---|---|---|
| `RING_ENABLE` | `1` | 1 = 识别并真的绕进去 |
| `RING_WIDE_MIN` | `135` | 路宽门槛（实测直道 103 / 左环 171 / 右环 151） |
| `RING_LONG_ROAD_MIN` | `36` | 视野长度保险 |
| `RING_STEER_KP / KP2` | `1.9 / 0.042` | 环内转向参数 |
| `RING_SPEED_MAX` | `138.0f` | 环内限速 |
| `RING_YAW_EXIT_L / R` | `50 / 40` | 出环再转角度 |
| `RING_EXIT_TIMEOUT_FRAMES` | `75` | 出环计时兜底 |
| `RING_WATCHDOG_FRAMES` | `600` | 绕环总超时 |
| `RING_COOLDOWN_FRAMES` | `250` | 出环冷却 |

### 6.6 其它

| 宏 | 值 | 位置 |
|---|---|---|
| `ENCODER_UPDATE_MS` | `20` | `user/cpu0_main.c` |
| `DISPLAY_TRACK_MS` | `150` | `user/cpu0_main.c` |
| `CAMERA_LOST_MS` | `200` | `user/cpu0_main.c`（掉线停车） |
| `CAMERA_RETRY_MAX` | `3` | `code/camera.c` |
| `ENCODER_1_INVERT / 2_INVERT` | `1 / 0` | `code/encoder.c`（实测） |
| `IMU_YAW_AXIS` | `Z` | `code/imu.h` |
| `IMU_YAW_DIR` | `1.0f` | `code/imu.h` |
| `IMU_GYRO_DEADBAND` | `1.5f` | `code/imu.h` |
| `IMU_YAW_SCALE` | `1.0f` | `code/imu.h` |

---

## 7. 屏幕读数说明

屏幕上方是摄像头图像（按本帧阈值二值化），叠加**蓝 = 左边线、绿 = 右边线、红 = 中线**；下方是 3 列 × 9 行状态。

### 第一列

| 标签 | 含义 | 怎么用 |
|---|---|---|
| `ERR` | 图像偏差（像素） | 正 = 赛道中心在右，车应右转 |
| `SDR` | 实际加到舵机上的偏移 | 饱和时能一眼看出 |
| `RAT` | 弯道减速比例 ×100 | 直道 100，弯心约 25 |
| `DTY` | 舵机当前占空比 | 正常范围 600~910 |
| `THR` | 本帧二值化阈值 | 看大津法是否被背景带偏 |
| `VLD` | 有效边线行数 | 低于 6 判丢赛道 |
| `CRS` | 十字判断 + 元素行数 | `CRS 1 5` = 正在十字里 |
| `LR` | 第 83 行左右边线坐标 | `-1` 表示这一侧跑出画面 |
| `ODO` | 编码器里程（两轮平均） | 推车量距离用 |

### 第二列

| 标签 | 含义 |
|---|---|
| `TOP` | 最远有效行 |
| `STA` | 状态：WAIT / RUN / LOST / NOCAM |
| `FRM` | 累计处理帧数（不涨 = 摄像头没出图） |
| `GMN` / `GMX` | 图像灰度最小 / 最大值（看曝光） |
| `PRD` | 控制周期（ms） |
| `RNG` | 环岛判断：`RNG L12` 或绕环中 `RNG LP2`（P+阶段号） |
| `CNR` | 本帧是否找到环岛拐点 |
| `LEN` | 视野长度（中心列连续白点采样行数） |

### 第三列

| 标签 | 含义 |
|---|---|
| `T1` / `A1` / `D1` | 电机 1 目标 / 实际 / 占空比 |
| `T2` / `A2` / `D2` | 电机 2 目标 / 实际 / 占空比 |
| `WID` | 第 83 行路面宽度（环岛诊断） |
| `YAW` | 陀螺仪航向角（度，右转为负） |
| `RR` | 本帧“只有右侧伸出去”的行数 |

---

## 8. 编译、烧录与调试

### 8.1 导入与编译

1. 打开 AURIX Development Studio 1.10.40，工作区选 `D:\AURIX_workspace`（工作区里已有同名工程时先删掉或改名）。
2. `File → Import → General → Existing Projects into Workspace`，选择本工程目录。
3. 选中工程按 `F5`（Refresh），让 ADS 重新扫描 `code/`、`user/`。
4. `Project → Clean…`，然后 `Build`。
5. 用工程里的 Debug 配置（`.launch`）烧录。

> **必须 Clean 全量重编**：`Debug/` 里的 `.elf/.hex/.map` 是另一台机器（`C:\Users\lenovo\...`）生成的，`Debug/TASKING_...opt` 里写死了旧绝对路径，与当前源码不对应。

### 8.2 上电前检查

- 车架起来（轮子悬空），确认：
  - 用手转左后轮，屏幕 `A1` 在动 → 电机 1 = 左轮，`TRACK_MOTOR1_IS_LEFT` 保持 1；
  - 给正目标，轮子往前转时 `A1/A2` 都是正数 → 编码器方向正确；
  - 舵机左右打到底不“嗡嗡”叫 → 限位没顶死。
- 上电后有 3 s 等待（`TRACK_START_DELAY_MS`），方便看图像和阈值。
- 摄像头超过 200 ms 没有新帧会**立即停车并回中**。
- 连续 30 帧丢线会停车。

---

## 9. 建议的调参顺序

按这个顺序做，每一步只改一处，避免互相干扰：

1. **电机开环标定**：`MOTOR_OPEN_LOOP_TEST = 1`，量死区和“占空比→速度”曲线，标完改回 0。
2. **编码器符号**：确认正转时 `A1/A2` 为正（反了就是正反馈，PID 永远收敛不了）。
3. **舵机中值与限位**：`SERVO_SWEEP_ENABLE = 1` 扫硬限位，填 `SERVO_LOCK_LOW/HIGH`，再确认中值 770 时前轮朝正前。
4. **图像**：看 `THR/GMN/GMX/VLD`，调 `IMAGE_THRESHOLD_MIN`，保证赛道白、背景黑。
5. **转向**：先只加 `KP`，直道不蛇形后再加 `KD`，弯道打角不够再加 `KP2`。
6. **速度与弯道减速**：先低直道速度跑顺，再按 `MIN_RATIO = 弯道速度 / 直道速度` 提速，并保持 `CURVE_GAIN = 1 − MIN_RATIO`。
7. **电子差速**：确认 `TRACK_MOTOR1_IS_LEFT` 正确后再调 `TRACK_DIFF_GAIN`（出弯甩尾就降）。
8. **陀螺仪**：静止 30 s 看 `YAW` 是否漂；贴地转 5 圈看比例。先治漂移，再标比例。
9. **环岛**：看 `RNG/WID/LEN/CNR/LR`，确认入口能起环、`P1~P7` 能顺序推进。

---

## 10. 已知问题与注意事项

### 10.1 源码注释与当前代码存在“历史层积”

注释是按迭代过程写的，部分描述已被后续改动覆盖，**以宏的实际值为准**：

| 位置 | 过期说法 | 当前实际 |
|---|---|---|
| `code/image.h`（环岛段） | “本工程没有陀螺仪，只能用编码器距离” | 已有 `imu.c`，环岛出环用 YAW |
| `code/image.c`（元素判决段） | “绕进环岛的处理还没加，只会直着走” | `ring.c` 已实现绕环，`RING_ENABLE = 1` |
| `code/motor.c` 文件头 | “主循环里每 20 ms 调一次 `motor_control_update()`” | 已挪进 `cc60_pit_ch0_isr` |
| `code/encoder.h` | “在 main 循环里每 20 ms 调一次” | 已挪进中断 |
| `code/track.h` 弯道段 | 举例仍写 400 × 0.20 = 80 | 实际 600 × 0.255 ≈ 153 |
| `code/display.h` | “3 列 × 6 行状态数字” | 实际 3 列 × 9 行 |

### 10.2 构建产物与路径

- `Debug/`、`.metadata/` 都进了 git，且包含旧机器绝对路径（`C:\Users\lenovo\Desktop\zhe\...`）。
- 建议加 `.gitignore`：`Debug/`、`.metadata/`、`.ads/`、`*.launch`。

### 10.3 编码

- 所有 `.c/.h` 为 **GBK（CP936）**。用 VS Code 需手动选 `GBK` 编码，或先统一转 UTF-8 再提交。

### 10.4 其它

- 应用层只用了 CPU0，`cpu1_main.c` / `cpu2_main.c` 是空循环，可作为后续并行化（如图像处理卸载）的落点。
- `motor.h` 的 `MOTOR_START_DELAY_MS` 当前没有被引用（启动延时实际由 `track.h` 的 `TRACK_START_DELAY_MS` 控制），属于残留定义。
- `motor_close_loop_test_start()` 是闭环测试接口，主流程未调用。
- 工程名仍是 `E01_gpio_demo`，与内容不符；改名请用根目录的 `AURIX修改工程名称.bat`（会同时清理工程临时文件）。

---

## 11. VOFA+ 串口调参

工程里已经写好双端：下位机 `code/vofa.c` + 上位机配置（VOFA+ 或 `tools/vofa_tune.py`）。

### 11.1 连接与参数

| 项 | 值 |
|---|---|
| 串口 | UART0，TX `P14_0`、RX `P14_1`（烧录/调试 USB 口那路串口） |
| 波特率 | 460800，8N1；打不开就改成 115200，同时改 `VOFA_BAUD` |
| 协议 | **JustFloat** |
| 帧格式 | 4 × float32（小端）+ 帧尾 `00 00 80 7F`，共 20 字节 |
| 发送频率 | 50 Hz（`VOFA_PERIOD_MS = 20`） |

如果板子的串口不在 `P14_0/P14_1`，只改 `code/vofa.h` 里的 `VOFA_UART` / `VOFA_TX_PIN` / `VOFA_RX_PIN`。

### 11.2 通道

| # | 名称 | 含义 |
|---|---|---|
| 0 | A1 | 电机 1 实际速度 |
| 1 | A2 | 电机 2 实际速度 |
| 2 | ERR | 图像偏差（像素） |
| 3 | YAW | 航向角（度） |

### 11.3 命令

在 VOFA+ 的“命令”面板发送 ASCII，一行一条（勾选“发送换行”）：

| 命令 | 作用 |
|---|---|
| `p 2.0 0.5 0` | 两个电机 PID（Kp Ki Kd，会清积分） |
| `kp 2.0` | 只改两路 Kp（Ki/Kd 和积分都不动） |
| `ki 0.5` | 只改两路 Ki |
| `kd 0` | 只改两路 Kd |
| `spd 600` | 直道基础速度（脉冲/20ms） |
| `dir 1.6 0.035 0.45` | 转向 Kp / Kp2 / Kd |
| `curve 0.255 40` | 弯道最低速度比例 / 减速参考偏差（GAIN 自动 = 1 − 比例） |
| `diff 1.35` | 电子差速增益 |
| `thr 20` | 二值化阈值偏移（−60 ~ +60） |
| `start` / `stop` | 起跑 / 停车 |

参数在下位机里都做了限幅；命令不回应答（回文字会打断波形），改没改看波形。

### 11.4 VOFA+ 设置步骤

1. 打开 VOFA+，左侧“数据源”选 **串口**，选烧录口的 COM，波特率 **460800**，8N1。
2. “数据格式”选 **JustFloat**（通道数会按 20 字节自动识别成 4）。
3. 添加“波形图”控件，把 4 个通道拖进去，按上表改图例名。
4. `ERR` 是 ±140 像素、`YAW` 是 ±180 度，和速度量级不同，建议给它们单独一个波形图或独立 Y 轴。
5. 打开“命令”面板，按 11.3 建几条常用命令（`p`、`dir`、`spd`），勾选换行后一键发送。

### 11.5 建议的调参顺序（在线版）

1. `spd 300` 先慢慢跑，看 `A1/A2` 是否稳定、两轮是否接近；
2. `p 2.0 0.5 0` 起调，`A` 追不上加 Kp，抖动加 Kd；
3. `dir` 从小往大加，直道不蛇形、弯道不冲出去为准；
4. `curve 0.30 40` 先保守，再逐步降比例提速；
5. `diff 1.2` 起步，出弯甩尾就往下调；
6. 图像发灰/发白时用 `thr ±20` 微调，同时看 `ERR` 是否平顺。

### 11.6 上位机命令表与备用上位机

命令定义在 `tools/cmd.json`：每条包含 `id`、`template`、参数名/范围/默认值和说明；下位机解析在 `code/vofa.c` 的 `vofa_exec()`，两边一一对应。VOFA+ 的命令面板仍需手动添加，可以照着这份表填。

`tools/vofa_tune.py`（Python + pyserial）直接读这份 JSON：

```bash
pip install pyserial
python tools/vofa_tune.py --list
python tools/vofa_tune.py --list-cmds                      # 打印 cmd.json 里的命令
python tools/vofa_tune.py --port COM5                      # 打开控制台，50Hz 刷 4 通道
python tools/vofa_tune.py --send pid_all 2.2 0.6 0 --port COM5
python tools/vofa_tune.py --send curve 0.255 40 --dry-run  # 只拼命令不发送
```

控制台里除了直接输入原始命令，还支持：

- `:list` 列出命令
- `:send <id> [参数...]` 按模板发送（超出范围自动夹到 min/max）
- `:q` 退出

### 11.7 把命令导入 VOFA+ 命令面板

这个版本的 VOFA+ 把命令树存在：

```
%LOCALAPPDATA%\vofa+\<版本号>\context\vofa+.cmds.json
```

`vofa+.cmds.json`（工程根目录）就是按这个格式写的命令组：`vnumber = 100`（和本机 VOFA+ 的 `<版本号>` 目录一致），每条命令的 `cmd_hex` 是**空格分隔的 HEX 字节**，例如：

```
70 20 32 2E 30 20 30 2E 35 20 30 0A   =  "p 2.0 0.5 0\n"
```

`down: false` 表示普通按钮，`loop_on: false` 表示点一次只发一次（避免 100ms 重复发 PID 命令把积分一直清零）。

先**关闭 VOFA+**，然后：

```bash
python tools/vofa_import_cmds.py --dry-run   # 先看目标文件和命令数
python tools/vofa_import_cmds.py             # 备份 + 合并写入
```

脚本会自动找到 `vofa+.cmds.json`，备份成 `.bak-时间戳`，再把“TC377 调参”组合并进去；你原有的其它命令组不会被改动。重新打开 VOFA+，命令面板里就能看到。

说明：

- 命令内容已经带换行（`0A`），不需要在 VOFA+ 里另外勾“换行”；
- 13 条都是固定预设值，导入后可以在 VOFA+ 里直接改某条命令的 HEX/文本；
- 如果你的 VOFA+ 版本号不是 100（`%LOCALAPPDATA%\vofa+\` 下的目录名），把 JSON 顶部的 `vnumber` 改成对应数字再导入；
- 目录不一样也没关系，脚本会自动找最新的那份，也可以用 `--file` 指定。

### 11.8 注意

- **不要**在同一个串口上再打 `printf` / `debug_init()`，文字会把二进制帧冲散。
- 新增 `code/vofa.c` 后，ADS 里要 **F5 刷新 + Clean**，让构建系统把新文件加进 makefile。
- `UART0` 的接收中断在 `user/isr.c` 的 `uart0_rx_isr` 里，只做入缓冲，解析在主循环，不会拖慢 20 ms 控制中断。

---

*文档依据 2026-10-20 的源码整理；实车标定数据（舵机限位、电机死区、环岛路宽等）标注截至 2026-10。*
