/*********************************************************************************************************************
* 文件名称          display.c
* 功能简介          IPS200 Pro（2.0 寸 SPI 智能屏）显示循迹状态
* 硬件平台          TC377TP + 逐飞开源库
*
* ******************************* 屏幕接线（和 IPS200 完全一样，不用改线） *******************************
*   SCLK = P15_3   MOSI = P15_5   MISO = P15_4（屏没有此脚，库要求定义）
*   CS   = P15_2   RST  = P15_1   INT/DC = P15_0
*
* ******************************* 方向 *******************************
*   IPS200 Pro 最大分辨率 320 x 240。这里用竖屏（IPS200PRO_PORTRAIT = 240 宽 x 320 高）。
*   如果你的屏装出来字是躺着的，把下面 DISP_DIR 换成 IPS200PRO_CROSSWISE /
*   IPS200PRO_PORTRAIT_180 试一下即可。
*
* ******************************* 布局（240 宽 x 320 高，竖屏，9 行 x 3 列） *******************************
*   ┌────────────────────────────┐
*   │      摄像头图像 188x120     │  蓝=左线 绿=右线 红=中线（水平居中）
*   │      （按本帧阈值二值化）   │  这就是算法眼里的图像，中线有没有跑偏一眼就能看出来
*   ├────────────────────────────┤
*   │ ERR   8   TOP  29   T1 191 │  y=124
*   │ SDR   9   STA RUN   A1   0 │  y=140
*   │ RAT  93   FRM 106   D1 4210│  y=156
*   │ DTY 741   GMN  12   T2 191 │  y=172
*   │ THR  94   GMX 201   A2   0 │  y=188
*   │ VLD  35   PRD  20   D2 4180│  y=204
*   │ CRS 0  6  RNG L12  WID 171 │  y=220
*   │ LR 0-171  CNR  1   YAW  10 │  y=236
*   │ ODO 123   LEN 42   RR   0  │  y=252
*   └────────────────────────────┘
*
*   ERR  图像偏差（像素，正 = 赛道偏右 = 车要右转）
*   SDR  舵机打角量（相对中值的占空比偏移）
*   RAT  弯道减速比例 x100（100 = 没减速）
*   DTY  舵机实际占空比
*   THR  本帧二值化阈值
*   VLD  找到边线的有效行数
*   TOP  最远的那条有效行
*   STA  状态：WAIT / RUN / LOST / NOCAM
*   FRM  已处理帧数（正常约 50/秒）
*   GMN / GMX  图像灰度最小 / 最大值（看曝光）
*   PRD  控制周期(ms)
*   T1/A1/D1  电机1 目标速度 / 实际速度 / 实际占空比
*   T2/A2/D2  电机2 目标速度 / 实际速度 / 实际占空比
*   CRS  十字判断：判定结果(1 = 正在十字里) / 元素行数（判断就是靠这个数）
*   RNG  环岛判断【起环就看这一格】：没绕环时 "RNG L12" = 侧别 + "只有一侧伸出去"的行数，
*        行数到 IMAGE_RING_ROW_MIN(3) 才会判成环岛；"-" 后面跟的是左右里大的那个数。
*        正在绕环 "RNG LP2" = 侧别 + P + 阶段号（1..7，对应 E07 的 cir_flag1..7），卡在哪一段不动就是那一段门槛不对。
*   LR   环岛诊断 3/4：第 83 行（量路宽那一行、对应 E07 第 41 行）的左右边线位置，原样显示。
*        "-1" = 这一侧跑出画面。左环入口应该读到 "LR 0-171" 这种。
*   CNR  本帧有没有找到环岛拐点（1/0）。绕环卡在 P3/P4 就是它一直 0。
*   YAW  陀螺仪积分出来的航向角（度），右转为负 —— 环岛 6->7 那段判据就靠它
*   ODO  编码器里程（两轮累计脉冲的平均值，上电清零）—— 量距离用
*   LEN  环岛诊断 1/4：视野长度。从最底行往上，图像中心列连续多少个采样行还是白的。
*        对应 E07 的 long_road = len(39)，起环门槛 RING_LONG_ROAD_MIN（实测取 36）。
*   WID  环岛诊断 2/4：第 83 行（对应 E07 第 41 行）的路面宽度。实测直道 103 / 左环 171 / 右环 151，门槛 135。
*   RR   环岛诊断 4/4：本帧"只有右侧伸出去"的行数（RNG 那格显示的是左右里大的那个）。
*********************************************************************************************************************/

#include "display.h"
#include "camera.h"
#include "image.h"
#include "motor.h"
#include "servo.h"
#include "track.h"
#include "imu.h"
#include "encoder.h"
#include "ring.h"

//==================================================== 方向 ====================================================
// 竖屏 240x320。字是躺着的时候换 IPS200PRO_CROSSWISE 或 IPS200PRO_PORTRAIT_180。
#define DISP_DIR            (IPS200PRO_PORTRAIT)

//==================================================== 布局参数 ====================================================
#define DISP_IMG_X          (26)            // 图像水平居中 (240 - 188) / 2
#define DISP_IMG_Y          (0)

#define DISP_LABEL_Y0       (124)           // 标签区第一行（图像到 y=119 为止）
#define DISP_LABEL_STEP     (16)            // 行距（16 号字体正好一行）
#define DISP_LABEL_W        (76)            // 标签宽度
#define DISP_COL1_X         (0)             // 第一列（转向 / 图像）
#define DISP_COL2_X         (80)            // 第二列（状态 / 调试）
#define DISP_COL3_X         (160)           // 第三列（电机）

//==================================================== 内部变量 ====================================================
static uint32 disp_period_ms = 0;

// 组件 ID
static uint16 disp_img = 0;
static uint16 lbl_err, lbl_sdr, lbl_rat, lbl_dty, lbl_thr, lbl_vld;
static uint16 lbl_crs;                                  // 十字判断（第一列第 7 行，下面还有空位）
static uint16 lbl_top, lbl_sta, lbl_frm, lbl_gmn, lbl_gmx, lbl_prd;
static uint16 lbl_ring;                                 // 环岛判断（第二列第 7 行，下面还有空位）
static uint16 lbl_lr, lbl_cnr, lbl_yaw;                 // 左边线位置 / 拐点 / 航向
static uint16 lbl_odo;                                  // 编码器里程（量距离用）
static uint16 lbl_len, lbl_wid, lbl_rr;                 // 环岛诊断：视野长度 / 路宽 / 右侧证据行数
static uint16 lbl_t1, lbl_a1, lbl_d1, lbl_t2, lbl_a2, lbl_d2;

// 图像上叠加的边线 / 中线（draw_line 必须在 image_display 之前调用）
static ips200pro_image_line_uint8_struct left_pts[IMAGE_H];
static ips200pro_image_line_uint8_struct right_pts[IMAGE_H];
static ips200pro_image_line_uint8_struct center_pts[IMAGE_H];

//==================================================== 内部函数 ====================================================
//-------------------------------------------------------------------------------------------------------------------
// 函数简介      状态机 -> 屏幕上的字符串
//-------------------------------------------------------------------------------------------------------------------
static const char *display_state_text (void)
{
    switch(track_get_state())
    {
        case TRACK_STATE_RUN:    return "RUN";
        case TRACK_STATE_LOST:   return "LOST";
        case TRACK_STATE_NO_CAM: return "NOCAM";
        default:                 return "WAIT";
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      创建一个 16 号字体的文本标签
//-------------------------------------------------------------------------------------------------------------------
static uint16 display_new_label (int16 x, int16 y, uint16 width)
{
    uint16 id = ips200pro_label_create(x, y, width, DISP_LABEL_STEP);
    if(0 != id)
    {
        ips200pro_set_font(id, FONT_SIZE_16);
    }
    return id;
}

//==================================================== 对外接口 ====================================================
//-------------------------------------------------------------------------------------------------------------------
// 函数简介      屏幕初始化
// 备注信息      顺序：init（建页面）-> 方向 -> 背光 -> 创建图像组件和所有标签
//-------------------------------------------------------------------------------------------------------------------
void display_init (void)
{
    // 1. 初始化屏幕（SPI + 复位 + 编码/字体/优化），并创建一个页面（空标题、无标题栏）
    ips200pro_init("", IPS200PRO_TITLE_TOP, 0);

    // 2. 方向、背光
    ips200pro_set_direction(DISP_DIR);
    ips200pro_set_backlight(100);

    // 3. 图像组件：顶部水平居中 188x120，正好放下总钻风的一帧
    disp_img = ips200pro_image_create(DISP_IMG_X, DISP_IMG_Y, IMAGE_W, IMAGE_H);

    // 4. 下方 3 列 x 6 行状态标签
    // 第一列：转向 / 图像
    lbl_err = display_new_label(DISP_COL1_X, DISP_LABEL_Y0 + DISP_LABEL_STEP * 0, DISP_LABEL_W);
    lbl_sdr = display_new_label(DISP_COL1_X, DISP_LABEL_Y0 + DISP_LABEL_STEP * 1, DISP_LABEL_W);
    lbl_rat = display_new_label(DISP_COL1_X, DISP_LABEL_Y0 + DISP_LABEL_STEP * 2, DISP_LABEL_W);
    lbl_dty = display_new_label(DISP_COL1_X, DISP_LABEL_Y0 + DISP_LABEL_STEP * 3, DISP_LABEL_W);
    lbl_thr = display_new_label(DISP_COL1_X, DISP_LABEL_Y0 + DISP_LABEL_STEP * 4, DISP_LABEL_W);
    lbl_vld = display_new_label(DISP_COL1_X, DISP_LABEL_Y0 + DISP_LABEL_STEP * 5, DISP_LABEL_W);
    lbl_crs = display_new_label(DISP_COL1_X, DISP_LABEL_Y0 + DISP_LABEL_STEP * 6, DISP_LABEL_W);

    // 第二列：状态 / 调试
    lbl_top = display_new_label(DISP_COL2_X, DISP_LABEL_Y0 + DISP_LABEL_STEP * 0, DISP_LABEL_W);
    lbl_sta = display_new_label(DISP_COL2_X, DISP_LABEL_Y0 + DISP_LABEL_STEP * 1, DISP_LABEL_W);
    lbl_frm = display_new_label(DISP_COL2_X, DISP_LABEL_Y0 + DISP_LABEL_STEP * 2, DISP_LABEL_W);
    lbl_gmn = display_new_label(DISP_COL2_X, DISP_LABEL_Y0 + DISP_LABEL_STEP * 3, DISP_LABEL_W);
    lbl_gmx = display_new_label(DISP_COL2_X, DISP_LABEL_Y0 + DISP_LABEL_STEP * 4, DISP_LABEL_W);
    lbl_prd = display_new_label(DISP_COL2_X, DISP_LABEL_Y0 + DISP_LABEL_STEP * 5, DISP_LABEL_W);
    lbl_ring = display_new_label(DISP_COL2_X, DISP_LABEL_Y0 + DISP_LABEL_STEP * 6, DISP_LABEL_W);

    // 陀螺仪读数：把原来 6 行下面剩下的空位用完（第一列第 8 行 + 第二列第 8 行 + 第三列第 7、8 行）
    lbl_lr = display_new_label(DISP_COL1_X, DISP_LABEL_Y0 + DISP_LABEL_STEP * 7, DISP_LABEL_W);
    lbl_cnr = display_new_label(DISP_COL2_X, DISP_LABEL_Y0 + DISP_LABEL_STEP * 7, DISP_LABEL_W);
    lbl_wid = display_new_label(DISP_COL3_X, DISP_LABEL_Y0 + DISP_LABEL_STEP * 6, DISP_LABEL_W);
    lbl_yaw = display_new_label(DISP_COL3_X, DISP_LABEL_Y0 + DISP_LABEL_STEP * 7, DISP_LABEL_W);
    lbl_odo = display_new_label(DISP_COL1_X, DISP_LABEL_Y0 + DISP_LABEL_STEP * 8, DISP_LABEL_W);

    // 环岛诊断四件套（原来的 WHT/FAR 是查"远处赛道"用的，那件事已经做完了）
    lbl_len = display_new_label(DISP_COL2_X, DISP_LABEL_Y0 + DISP_LABEL_STEP * 8, DISP_LABEL_W);
    lbl_rr = display_new_label(DISP_COL3_X, DISP_LABEL_Y0 + DISP_LABEL_STEP * 8, DISP_LABEL_W);

    // 第三列：电机
    lbl_t1 = display_new_label(DISP_COL3_X, DISP_LABEL_Y0 + DISP_LABEL_STEP * 0, DISP_LABEL_W);
    lbl_a1 = display_new_label(DISP_COL3_X, DISP_LABEL_Y0 + DISP_LABEL_STEP * 1, DISP_LABEL_W);
    lbl_d1 = display_new_label(DISP_COL3_X, DISP_LABEL_Y0 + DISP_LABEL_STEP * 2, DISP_LABEL_W);
    lbl_t2 = display_new_label(DISP_COL3_X, DISP_LABEL_Y0 + DISP_LABEL_STEP * 3, DISP_LABEL_W);
    lbl_a2 = display_new_label(DISP_COL3_X, DISP_LABEL_Y0 + DISP_LABEL_STEP * 4, DISP_LABEL_W);
    lbl_d2 = display_new_label(DISP_COL3_X, DISP_LABEL_Y0 + DISP_LABEL_STEP * 5, DISP_LABEL_W);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      显示控制周期（右下角那格，方便一眼看到速度环节拍）
//-------------------------------------------------------------------------------------------------------------------
void display_show_period (uint32 period_ms)
{
    disp_period_ms = period_ms;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      循迹模式显示
// 备注信息      直接更新图像组件和标签内容，不整屏清屏，所以不会闪
//-------------------------------------------------------------------------------------------------------------------
void display_update_track (void)
{
    uint16 y;
    uint16 nl = 0;
    uint16 nr = 0;
    uint16 nc = 0;

    // ------------------------------- 图像 + 边线叠加 -------------------------------
    // 先收集每条有效行的 左/右/中线 坐标，叠到图像上（蓝=左、绿=右、红=中）。
    // 注意：draw_line 必须在 image_display 之前调用才会生效。
    for(y = 0; y < IMAGE_H; y++)
    {
        if(0 == image_row_valid[y])
        {
            continue;
        }

        left_pts[nl].x   = (uint8)image_left_line[y];
        left_pts[nl].y   = (uint8)y;
        nl++;

        right_pts[nr].x  = (uint8)image_right_line[y];
        right_pts[nr].y  = (uint8)y;
        nr++;

        center_pts[nc].x = (uint8)image_center_line[y];
        center_pts[nc].y = (uint8)y;
        nc++;
    }

    ips200pro_image_draw_line(disp_img, 1, left_pts,   nl, IMAGE_LINE_TYPE_UINT8, RGB565_BLUE);
    ips200pro_image_draw_line(disp_img, 2, right_pts,  nr, IMAGE_LINE_TYPE_UINT8, RGB565_GREEN);
    ips200pro_image_draw_line(disp_img, 3, center_pts, nc, IMAGE_LINE_TYPE_UINT8, RGB565_RED);

    // 灰度图像 + 本帧阈值二值化，屏幕上看的就是算法眼里的图像
    ips200pro_image_display(disp_img, (const void *)camera_image,
                            IMAGE_W, IMAGE_H, IMAGE_GRAYSCALE, image_threshold);

    // ------------------------------- 第一列：转向 / 图像 -------------------------------
    ips200pro_label_printf(lbl_err, "ERR %d", (int)image_error);
    ips200pro_label_printf(lbl_sdr, "SDR %d", (int)track_get_steer());
    ips200pro_label_printf(lbl_rat, "RAT %d", (int)(track_get_curve_ratio() * 100.0f));
    ips200pro_label_printf(lbl_dty, "DTY %d", (int)servo_get_duty());
    ips200pro_label_printf(lbl_thr, "THR %d", (int)image_threshold);
    ips200pro_label_printf(lbl_vld, "VLD %d", (int)image_valid_row_count);
    // CRS = 十字判断：第一个数是判定结果（1 = 正在十字里），第二个是"整行无边线"的行数
    ips200pro_label_printf(lbl_crs, "CRS %d %d", (int)image_cross_flag, (int)image_cross_rows);

    // ------------------------------- 陀螺仪 + 环岛诊断 ------------------------------
    // 陀螺仪已经全部实测确认（偏航轴 Z），GZ 那格让给 WID；这里只留 YAW 给出环判据用。
    // LR = 第 83 行（量路宽的那一行）的左右边线位置，原样显示（-1 = 这一侧跑出画面）。
    //   左环入口应该读到 "0-171" 这种；右环入口看右边那条是不是接近 187。
    //   起环判据已经不靠它了（改用 RNG 那格显示的 image_ring_side），留着是起不动时查原因用的。
    ips200pro_label_printf(lbl_lr, "LR %d-%d",
        (int)image_raw_l[RING_R_WIDE], (int)image_raw_r[RING_R_WIDE]);
    // CNR = 本帧有没有找到环岛拐点（1/0）。绕环时卡在 P3/P4 不动，就是它一直是 0。
    ips200pro_label_printf(lbl_cnr, "CNR %d", (int)ring_corner_ok);
    // WID = 第 83 行（对应 E07 第 41 行）的路面宽度（环岛诊断 2/4），实测直道 103 / 左环 171 / 右环 151，门槛取 135。
    ips200pro_label_printf(lbl_wid, "WID %d", (int)ring_wide);
    ips200pro_label_printf(lbl_yaw, "YAW %d", (int)imu_get_yaw_deg());

    // ODO = 编码器里程：两轮累计脉冲的平均值（上电清零）。
    // 量距离就用它：把车沿某条路径推一遍，读到的就是这个距离对应的脉冲数。
    ips200pro_label_printf(lbl_odo, "ODO %d",
        (int)((encoder_get_total(ENCODER_ID_1) + encoder_get_total(ENCODER_ID_2)) / 2));

    // LEN = 视野长度（环岛诊断 1/4）：从最底行往上，图像中心列连续多少个【采样行】还是白的。
    //   对应 E07 的 long_road = len(39)，起环门槛 RING_LONG_ROAD_MIN（实测取 36）。
    //   标定：推车到环岛入口，看这个数在"该起环"的位置是多少，把门槛填对。
    ips200pro_label_printf(lbl_len, "LEN %d", (int)ring_long_road);

    // RR = 本帧"只有右侧伸出去"的行数。RNG 那格只显示左右里大的那个，
    //   这里补上右边那个数 —— RNG 起不来的时候才知道是哪一侧的证据不够。
    ips200pro_label_printf(lbl_rr, "RR %d", (int)image_ring_right_rows);

    // ------------------------------- 第二列：状态 / 调试 -------------------------------
    ips200pro_label_printf(lbl_top, "TOP %d", (int)image_valid_top_row);
    ips200pro_label_printf(lbl_sta, "STA %s", display_state_text());
    ips200pro_label_printf(lbl_frm, "FRM %u", (unsigned)camera_get_frame());
    ips200pro_label_printf(lbl_gmn, "GMN %d", (int)image_gray_min);
    ips200pro_label_printf(lbl_gmx, "GMX %d", (int)image_gray_max);
    ips200pro_label_printf(lbl_prd, "PRD %u", (unsigned)disp_period_ms);
    // RING = 环岛判断：
    //   没在绕环  "RNG L12"   L/R/- 是判定结果，后面是"只有一侧伸出去"的行数
    //   正在绕环  "RNG LP2"   L/R 后面跟 P + 阶段号（1..7，对应 E07 的 cir_flag1..7）
    //   现场标定环岛就看这个 P：卡在哪一段不动，就说明那一段的判据门槛没标定对。
    {
        const char *rside;
        int16       rnum;

        if(1 == image_ring_side)      { rside = "L"; rnum = (int16)image_ring_left_rows;  }
        else if(2 == image_ring_side) { rside = "R"; rnum = (int16)image_ring_right_rows; }
        else
        {
            rside = "-";
            rnum  = (image_ring_left_rows > image_ring_right_rows)
                  ? (int16)image_ring_left_rows : (int16)image_ring_right_rows;
        }

        if(0 != ring_get_state())
        {
            ips200pro_label_printf(lbl_ring, "RNG %cP%d", rside[0], (int)ring_get_state());
        }
        else
        {
            ips200pro_label_printf(lbl_ring, "RNG %s%d", rside, (int)rnum);
        }
    }

    // ------------------------------- 第三列：电机 -------------------------------
    ips200pro_label_printf(lbl_t1, "T1 %d", (int)motor_get_target(1));
    ips200pro_label_printf(lbl_a1, "A1 %d", (int)motor_get_speed(1));
    ips200pro_label_printf(lbl_d1, "D1 %d", (int)motor_get_duty(1));
    ips200pro_label_printf(lbl_t2, "T2 %d", (int)motor_get_target(2));
    ips200pro_label_printf(lbl_a2, "A2 %d", (int)motor_get_speed(2));
    ips200pro_label_printf(lbl_d2, "D2 %d", (int)motor_get_duty(2));
}
