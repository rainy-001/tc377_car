/*********************************************************************************************************************
* 文件名称          ring.c
* 功能简介          环岛状态机 —— 照 E07_encoder_demo\code\image.c 的 cir_l() / cir_R() 移植
* 备注信息          参考来源、换算依据、每个门槛的原始值都写在 ring.h 里。这里只说实现的三个要点：
*                   ① 起环靠 image.c 的 image_ring_side（逐行单侧证据），换段照 E07 用丢边标志
*                   ② 找拐点用 E07 Get_piont() 的五邻域花样，找到以后从底行锚点往拐点拉直线当中线的一侧
*                   ③ 一旦进环就改写 image_center_line[] 并【重算 image_error】——
*                      所以 track.c / display.c 里看偏差的部分一行都不用改
*********************************************************************************************************************/

#include "ring.h"
#include "imu.h"

//==================================================== 结果 ====================================================
uint8   ring_on          = 0;                           // 1 = 正在绕环
uint8   ring_state       = 0;                           // 1..7 = 阶段；0 = 不在环里
uint8   ring_side        = 0;                           // 1 = 左环  2 = 右环
uint8   ring_long_road   = 0;                           // 视野长度
int16   ring_wide        = 0;                           // 第 83 行路宽
uint8   ring_corner_ok   = 0;                           // 本帧找到拐点了吗

//==================================================== 内部变量 ====================================================
static uint16  ring_watch    = 0;                       // 整个绕环过程的兜底计时（超过 RING_WATCHDOG_FRAMES 强制复位）
static uint16  ring_cooldown   = 0;                     // 出环冷却计数（E07 的 timer_L）
static uint16  ring_exit_cnt   = 0;                     // 7 阶段计时兜底（E07 的 youchuhuandejishu）
static float32 ring_ang_ref    = 0.0f;                  // 5->6 时记下的航向（E07 的 cir_ang）
static int16   ring_corner_col = 0;                     // 本帧拐点列
static int16   ring_corner_row = 0;                     // 本帧拐点行

// E07 里 3->4 用的是 up_piont_x > 20（它图高 60），换算过来就是"拐点行号 > 本工程第 41 行"
#define RING_ROW_IN             RING_ROW(20)

#if (RING_ENABLE)
//-------------------------------------------------------------------------------------------------------------------
// 函数简介      视野长度：从最底行往上，中心列连续多少【采样行】还是白的（照 E07 len()）
// 备注信息      E07: long_road = len(39)。它的 39 列在 80 宽里就是中间，对应本工程的图像中心列。
//-------------------------------------------------------------------------------------------------------------------
static uint8 ring_measure_long_road (void)
{
    int16 y;
    uint8 n = 0;

    for(y = IMAGE_SCAN_BOTTOM; y >= IMAGE_SCAN_TOP; y -= IMAGE_SCAN_STEP)
    {
        if(image_bin[y][IMAGE_CENTER_X] < 128) { break; }
        n++;
    }

    return n;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      第 RING_R_WIDE 行（对应 E07 第 41 行）的路面宽度
//-------------------------------------------------------------------------------------------------------------------
static int16 ring_measure_wide (void)
{
    int16 l = image_raw_l[RING_R_WIDE];
    int16 r = image_raw_r[RING_R_WIDE];

    // 【不再要求这一行有效】环岛入口那几行正好会被元素分类器标成元素行、
    // image_row_valid 是 0，原来直接返回 0 -> 路宽永远是 0 -> 环岛【永远认不出来】。
    // 照 E07 wild_celiang(41) 的语义：它那套点数组总是有值，边线跑出画面就夹到图像边缘，
    // 所以环岛入口量出来是"图像边缘到另一侧"的宽度（偏大，正是我们要的那个"变宽"特征）。
    if(l < 0)        { l = 0; }
    if(r >= IMAGE_W) { r = IMAGE_W - 1; }
    if(r <= l)       { return 0; }

    return (int16)(r - l);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      找环岛拐点（照 E07 Get_piont / Get_piont_R 的五邻域花样）
// 参数说明      site    0 = 中拐点（入环那段用）  1 = 上拐点（进环/出环那段用）
//               side    1 = 左环  2 = 右环
// 返回参数      1 = 找到了（结果写在 ring_corner_col / ring_corner_row）
// 备注信息      全部条件照抄 E07，只把邻域偏移按分辨率放大：
//                 E07 的 4 像素  -> RING_CORNER_DX (9)
//                 E07 的 3/4 行  -> RING_CORNER_DY (8)
//                 E07 的 i+1     -> 本工程 y+2（行号是 2 倍关系）
//                 E07 的 i+3     -> 本工程 y+6（用于"再往下几行丢没丢线"）
//               含义（以左环中拐点为例）：这一列是黑的，但右边是白、左边是黑（一条竖直黑线），
//               而且上下 8 行都是白，并且左边线在这一行最靠右（鼓出来）—— 就是环岛内圈最突出的那个尖角。
//-------------------------------------------------------------------------------------------------------------------
static uint8 ring_find_corner (uint8 site, uint8 side)
{
    int16 y;
    int16 col_lo;
    int16 col_hi;

    ring_corner_ok = 0;

    if(1 == side) { col_lo = RING_CORNER_COL_LO;   col_hi = RING_CORNER_COL_HI;   }
    else          { col_lo = RING_CORNER_R_COL_LO; col_hi = RING_CORNER_R_COL_HI; }

    for(y = RING_R_BOT; y >= (IMAGE_SCAN_TOP + RING_CORNER_DY); y -= IMAGE_SCAN_STEP)
    {
        int16 x;

        if((0 == image_row_valid[y]) || (0 == image_row_valid[y + RING_CORNER_DY])
        || (0 == image_row_valid[y - RING_CORNER_DY]) || (0 == image_row_valid[y + 2]))
        {
            continue;
        }

        for(x = col_lo; x <= col_hi; x++)
        {
            const uint8 *r0 = image_bin[y];
            uint8 hit = 0;

            if(r0[x] >= 128) { continue; }              // 必须是黑的起点

            if(0 == site)                               // ---------------- 中拐点 ----------------
            {
                if(1 == side)
                {
                    hit = ((r0[x + RING_CORNER_DX] >= 128) && (r0[x - RING_CORNER_DX] < 128)
                        && (image_bin[y + RING_CORNER_DY][x] >= 128)
                        && (image_bin[y - RING_CORNER_DY][x] >= 128)
                        && (image_left_line[y] >= image_left_line[y + RING_CORNER_DY])
                        && (image_left_line[y] >= image_left_line[y - RING_CORNER_DY])) ? 1 : 0;
                }
                else
                {
                    hit = ((r0[x + RING_CORNER_DX] < 128) && (r0[x - RING_CORNER_DX] >= 128)
                        && (image_bin[y + RING_CORNER_DY][x] >= 128)
                        && (image_bin[y - RING_CORNER_DY][x] >= 128)
                        && (image_right_line[y] <= image_right_line[y + RING_CORNER_DY])
                        && (image_right_line[y] <= image_right_line[y - RING_CORNER_DY])) ? 1 : 0;
                }
            }
            else                                        // ---------------- 上拐点 ----------------
            {
                if(1 == side)
                {
                    hit = ((r0[x + RING_CORNER_DX] >= 128) && (image_bin[y + 2][x - RING_CORNER_DX] >= 128)
                        && (image_bin[y - RING_CORNER_DY][x] < 128) && (image_bin[y + 2][x] >= 128)
                        && (1 == image_lost_l[y + 6])) ? 1 : 0;
                }
                else
                {
                    hit = ((r0[x - RING_CORNER_DX] >= 128) && (image_bin[y + 2][x + RING_CORNER_DX] >= 128)
                        && (image_bin[y - RING_CORNER_DY][x] < 128) && (image_bin[y + 2][x] >= 128)
                        && (1 == image_lost_r[y + 6])) ? 1 : 0;
                }
            }

            if(hit)
            {
                ring_corner_col = x;
                ring_corner_row = y;
                ring_corner_ok  = 1;
                return 1;
            }
        }
    }

    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      补线：从底行锚点往拐点拉一条直线当作某一侧边线，再和另一侧真实边线取中点
// 参数说明      fill_left   1 = 补左边线（用右边线当中点的一侧）  0 = 补右边线
//               col, row    拐点（列, 行）
// 备注信息      照 E07 buxian_l() / buxian_r()：它是
//                 buxian_k = (y - left_point[55]) / (x - 55);      // x 是行、y 是列
//                 left_bu_point[i] = buxian_k * (i - 55) + left_point[55];
//               本工程不另外维护 left_bu_point[]，直接把结果写进 image_center_line[]。
//               【哪一行本来无效就跳过哪一行】—— 无效行的边线可能是上一帧的残留，用它取中点会出错。
//-------------------------------------------------------------------------------------------------------------------
static void ring_fill_center (uint8 fill_left, int16 col, int16 row)
{
    int16   y;
    int16   anchor;
    float32 k;

    if(row == RING_R_BOT) { return; }                   // 和锚点同一行，斜率算不出来

    anchor = fill_left ? image_left_line[RING_R_BOT] : image_right_line[RING_R_BOT];
    if((anchor < 0) || (anchor >= IMAGE_W)) { return; }

    k = (float32)(col - anchor) / (float32)(row - RING_R_BOT);

    for(y = IMAGE_SCAN_BOTTOM; y >= IMAGE_SCAN_TOP; y -= IMAGE_SCAN_STEP)
    {
        float32 bu;
        int16   other;
        int16   c;

        if(0 == image_row_valid[y]) { continue; }

        bu = k * (float32)(y - RING_R_BOT) + (float32)anchor;
        if(bu < 0.0f)        { bu = 0.0f; }
        if(bu > (IMAGE_W - 1)) { bu = (float32)(IMAGE_W - 1); }

        if(fill_left)
        {
            other = image_right_line[y];
            if((0 == image_row_valid[y]) || (other >= IMAGE_W)) { continue; }
            c = (int16)(((int16)bu + other) / 2);
        }
        else
        {
            other = image_left_line[y];
            if(other < 0) { continue; }
            c = (int16)((other + (int16)bu) / 2);
        }

        if(c < 0)           { c = 0; }
        if(c > IMAGE_W - 1) { c = IMAGE_W - 1; }

        image_center_line[y] = c;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      状态 6（出环）用的"虚拟拐点"：照 E07 buxian_r(60-len(35), 35)
// 备注信息      E07 的出环不是找真实拐点，而是拿【视野长度】反推一个点：
//                 行 = 60 - len(35)（图高减视野长度），列 = 固定列（左环 35、右环 56，都在 80 宽里）
//               本工程换算：行 = IMAGE_H - 2*视野长度（视野长度是采样行数，乘 2 变图像行），
//                           列 = RING_C6_COL_L / RING_C6_COL_R（按 2.35 倍换算过的固定列）
//-------------------------------------------------------------------------------------------------------------------
static void ring_fill_state6 (uint8 side)
{
    int16 row = (int16)(IMAGE_H - 2 * (int16)ring_long_road);

    if(row > RING_R_BOT) { row = RING_R_BOT; }
    if(row < IMAGE_SCAN_TOP) { row = IMAGE_SCAN_TOP; }

    if(1 == side) { ring_fill_center(0, RING_C6_COL_L, row); }      // 左环：补右边线
    else          { ring_fill_center(1, RING_C6_COL_R, row); }      // 右环：补左边线
}
#endif

//==================================================== 对外接口 ====================================================
//-------------------------------------------------------------------------------------------------------------------
// 函数简介      环岛状态机，每帧调用一次（由 image_process() 最后自动调用）
//-------------------------------------------------------------------------------------------------------------------
void ring_update (void)
{
#if (RING_ENABLE)
    // ---------------- 0. 冷却计数 ----------------
    if(ring_cooldown > 0) { ring_cooldown--; }

    // ---------------- 1. 诊断量 ----------------
    ring_long_road = ring_measure_long_road();
    ring_wide      = ring_measure_wide();

    // ---------------- 2. 起环检测 ----------------
    // 【不再用 E07 那套丢边行号】原因写在 ring.h 文件头：实测右环入口右边线根本跑不出画面。
    //   主力判据是 image.c 每帧算好的 image_ring_side —— 一帧里有 IMAGE_RING_ROW_MIN 行
    //   "只有一侧伸出去"（逐行拿自己跟下面一行比，模型无关）。
    //   再用两条本工程实测的量交叉确认：
    //     ring_wide > RING_WIDE_MIN   环岛入口路面必然变宽（实测直道 103 / 左环 171 / 右环 151）
    //     ring_long_road >= ...       视野太短不起环（刚开机 / 贴着急弯）
    //   image_ring_side != 0 时 image.c 已经顺手把 image_off_track 清 0 了，这里不用再判。
    if((0 == ring_on) && (0 == ring_cooldown) && (0 != image_ring_side)
    && (ring_long_road >= RING_LONG_ROAD_MIN) && (ring_wide > RING_WIDE_MIN))
    {
        ring_side  = image_ring_side;
        ring_on    = 1;
        ring_state = 1;
        ring_watch = 0;
    }

    // ---------------- 3. 状态迁移 + 补线 ----------------
    if(0 != ring_on)
    {
        const uint8 *lost_ring  = (1 == ring_side) ? image_lost_l : image_lost_r;
        const uint8 *lost_other = (1 == ring_side) ? image_lost_r : image_lost_l;
        const int16 *pt_other   = (1 == ring_side) ? image_right_line : image_left_line;
        float32      yaw        = imu_get_yaw_deg();

        if(ring_state < 0xFF) { ring_exit_cnt++; }
        if(ring_watch < 0xFFFF) { ring_watch++; }

        if(1 == ring_state)
        {
            // 1 -> 2：环这一侧的边线又回来了（车已经进环）
            if((0 == lost_ring[RING_R_LO]) && (0 == lost_ring[RING_R_HI]))
            {
                ring_state = 2;
            }
        }

        if(2 == ring_state)
        {
            // 2 -> 3：环这一侧又丢线了（走到环岛上半部分）
            if((1 == lost_ring[RING_R_LO]) && (1 == lost_ring[RING_R_HI]))
            {
                ring_state = 3;
            }
        }

        if(3 == ring_state)
        {
            // 3 -> 4：找到上拐点（拐点行号 > 第 41 行，也就是拐点比较靠下）
            if(ring_find_corner(1, ring_side) && (ring_corner_row > RING_ROW_IN))
            {
                ring_state = 4;
            }
        }

        if(4 == ring_state)
        {
            // 4 -> 5：拐点更靠下（> 第 81 行），或者干脆找不到拐点了 —— 说明已经在环里
            if((0 == ring_corner_ok) || (ring_corner_row > RING_R_LO))
            {
                ring_state = 5;
            }
        }

        if(5 == ring_state)
        {
            // 5 -> 6：出环特征（照 E07：左环看 right_point[37] < right_point[35]，右环反向）
            int16 a = pt_other[RING_R_CUR_A];
            int16 b = pt_other[RING_R_CUR_B];

            if((0 != image_row_valid[RING_R_CUR_A]) && (0 != image_row_valid[RING_R_CUR_B])
            && (a >= 0) && (a < IMAGE_W) && (b >= 0) && (b < IMAGE_W))
            {
                if(((1 == ring_side) && (a < b)) || ((2 == ring_side) && (a > b)))
                {
                    ring_state  = 6;
                    ring_ang_ref = yaw;                     // 记下航向（E07: cir_ang = angle）
                }
            }
        }

        if(6 == ring_state)
        {
            // 6 -> 7：转过固定角度（左环 +50 度、右环 -40 度）
            if(((1 == ring_side) && ((yaw - ring_ang_ref) > RING_YAW_EXIT_L))
            || ((2 == ring_side) && ((ring_ang_ref - yaw) > RING_YAW_EXIT_R)))
            {
                ring_state = 7;
                ring_exit_cnt = 0;
            }
        }

        if(7 == ring_state)
        {
            // 7 -> 0：左右边线都回来了 = 出环完成；或者计时兜底（E07 右环就是这么兜的）
            if(((0 == lost_ring[RING_R_CTRL]) && (0 == lost_other[RING_R_CTRL]))
            || (ring_exit_cnt >= RING_EXIT_TIMEOUT_FRAMES))
            {
                ring_on       = 0;
                ring_state    = 0;
                ring_side     = 0;
                ring_exit_cnt = 0;
                ring_watch    = 0;
                ring_cooldown = RING_COOLDOWN_FRAMES;       // 冷却，防止同一个环被重复识别
            }
        }

        // ---------------- 3.5 兜底计时 ----------------
        //   起环判据是"逐行单侧证据"，万一是假特征起的环，状态机会一直卡在里面
        //   （ring_on 挂着不放，后面的真环岛就进不来了）。超过 RING_WATCHDOG_FRAMES
        //   无条件复位，并给足冷却。正常绕环在 300 帧上下（E07 右环的出环兜底就是 300）。
        if((0 != ring_on) && (ring_watch >= RING_WATCHDOG_FRAMES))
        {
            ring_on       = 0;
            ring_state    = 0;
            ring_side     = 0;
            ring_watch    = 0;
            ring_cooldown = RING_COOLDOWN_FRAMES;
        }

        // ---------------- 4. 按阶段改写中线（照 E07 的补线执行块）----------------
        //   1/2/7 补【环这一侧】的边线    3/4/6 补【另一侧】的边线    5 不补

        if(0 != ring_on)
        {
            uint8 fill_ring_side = ((1 == ring_state) || (2 == ring_state) || (7 == ring_state)) ? 1 : 0;

            if((3 == ring_state) || (4 == ring_state) || (7 == ring_state))
            {
                if(ring_find_corner(1, ring_side))
                {
                    ring_fill_center(fill_ring_side ? (1 == ring_side) : (2 == ring_side),
                                     ring_corner_col, ring_corner_row);
                }
            }
            else if((1 == ring_state) || (2 == ring_state))
            {
                if(ring_find_corner(0, ring_side))
                {
                    ring_fill_center((1 == ring_side) ? 1 : 0, ring_corner_col, ring_corner_row);
                }
            }
            else if(6 == ring_state)
            {
                ring_fill_state6(ring_side);
            }

            image_calc_error();                             // 中线改了，偏差必须重算
            image_off_track = 0;                            // 环岛里不算丢赛道
        }
    }
#else
    ring_long_road = 0;
    ring_wide      = 0;
    ring_corner_ok = 0;
#endif
}

uint8 ring_is_active (void)
{
    return ring_on;
}

uint8 ring_get_state (void)
{
    return ring_state;
}
