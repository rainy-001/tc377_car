/*********************************************************************************************************************
* 文件名称          image.c
* 功能简介          赛道图像处理：大津法二值化 + 左右边线提取 + 中线 + 加权偏差
*********************************************************************************************************************/

#include "image.h"
#include "ring.h"

//==================================================== 全局变量 ====================================================
uint8  image_bin[IMAGE_H][IMAGE_W];
int16  image_left_line[IMAGE_H];
int16  image_right_line[IMAGE_H];
int16  image_center_line[IMAGE_H];
uint8  image_row_valid[IMAGE_H];
uint8  image_lost_l[IMAGE_H];                           // 1 = 本行左边线跑到画面外了（单边补线【之前】的原始判断）
int16  image_raw_l[IMAGE_H];                          // 补线【之前】的原始左边线（-1 = 这一行没找到）
int16  image_raw_r[IMAGE_H];                          // 环岛算曲率要用它：元素行的 image_left/right_line 是不写的
uint8  image_lost_r[IMAGE_H];                           // 环岛检测（ring.c）照 E07 的写法全靠这两个量

uint8  image_threshold       = IMAGE_FIXED_THRESHOLD;
uint8  image_otsu_threshold  = IMAGE_FIXED_THRESHOLD;
uint8  image_gray_min        = 0;
uint8  image_gray_max        = 0;
uint32 image_white_count     = 0;
uint8  image_valid_row_count = 0;
uint8  image_valid_top_row   = IMAGE_H;
int16  image_error           = 0;
int16  image_error_near      = 0;
int16  image_error_far       = 0;
uint8  image_error_far_used  = IMAGE_H;                 // 本帧真正参与偏差的最远行
uint8  image_off_track       = 1;

//==================================================== 元素行（十字 / 环岛）====================================================
uint8  image_row_gap[IMAGE_H];                          // 1 = 这一行是元素行，交给补线处理
uint8  image_gap_filled_rows = 0;                       // 本帧插值补出来的行数
uint8  image_cross_rows      = 0;                       // 本帧"两侧都伸出去"的行数（十字）
uint8  image_cross_flag      = 0;                       // 1 = 本帧判定为十字
uint16 image_cross_frames    = 0;                       // 十字已经连续持续了多少帧
uint8  image_white_rows      = 0;                       // 本帧"整行全白"的行数（没有左右信息）
uint8  image_ring_left_rows  = 0;                       // 本帧"只有左侧伸出去"的行数
uint8  image_ring_right_rows = 0;                       // 本帧"只有右侧伸出去"的行数
uint8  image_ring_side       = 0;                       // 0 = 没环岛  1 = 左环岛  2 = 右环岛

static uint16 image_cross_row_count = 0;                // 扫描中累计：两侧都伸出去
static uint16 image_white_row_count = 0;                // 扫描中累计：整行全白
static uint16 image_ring_row_l      = 0;                // 扫描中累计：只有左侧
static uint16 image_ring_row_r      = 0;                // 扫描中累计：只有右侧
static int16 image_threshold_offset = IMAGE_THRESHOLD_OFFSET;   // 运行时可改的阈值偏移（VOFA+ 的 thr 命令）

//==================================================== 追线参考 ====================================================
// 逐行扫是从最下面一行往上扫的，所以"上一有效行"就是画面里更近的那一行。
// 赛道在画面里是连续的，用它当下一个搜索种子比用图像正中靠谱得多。
// 每帧在 image_process() 里清零，从最底行重新建立。
static int16 image_scan_ref_l = -1;                     // 上一有效行的左边线
static int16 image_scan_ref_r = -1;                     // 上一有效行的右边线
static int16 image_scan_ref_w = 0;                      // 上一有效行的赛道宽度（0 = 还没有参考）

//==================================================== 内部函数 ====================================================
//-------------------------------------------------------------------------------------------------------------------
// 函数简介      大津法（最大类间方差）算二值化阈值
// 返回参数      阈值 0~255
// 备注信息      直方图放在 static 里，不要占用户栈（本工程用户栈只有 2KB）
//               隔一个像素采一次样，直方图形状几乎不变，时间省一半
//-------------------------------------------------------------------------------------------------------------------
static uint8 image_otsu (void)
{
    static uint32 hist[256];

    const uint8 *p    = &camera_image[0][0];
    uint32 size       = (uint32)IMAGE_W * IMAGE_H;
    uint32 i;
    uint32 total      = 0;
    uint32 wb         = 0;
    uint32 wf;
    float32 sum       = 0.0f;
    float32 sumb      = 0.0f;
    float32 mb, mf, var, var_max = 0.0f;
    uint8  th         = IMAGE_FIXED_THRESHOLD;

    memset(hist, 0, sizeof(hist));

    for(i = 0; i < size; i += 2)
    {
        hist[p[i]]++;
        total++;
    }

    for(i = 0; i < 256; i++)
    {
        sum += (float32)i * (float32)hist[i];
    }

    for(i = 0; i < 256; i++)
    {
        wb += hist[i];
        if(0 == wb)
        {
            continue;
        }

        wf = total - wb;
        if(0 == wf)
        {
            break;
        }

        sumb += (float32)i * (float32)hist[i];
        mb    = sumb / (float32)wb;
        mf    = (sum - sumb) / (float32)wf;
        var   = (float32)wb * (float32)wf * (mb - mf) * (mb - mf);

        if(var > var_max)
        {
            var_max = var;
            th      = (uint8)i;
        }
    }

    return th;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      二值化，顺手统计白点数和灰度极值
// 参数说明      th              阈值，>= th 记白（赛道），< th 记黑（边线）
//-------------------------------------------------------------------------------------------------------------------
static void image_binarize (uint8 th)
{
    uint32 x, y;
    uint32 white = 0;
    uint8  gmin  = 255;
    uint8  gmax  = 0;

    for(y = 0; y < IMAGE_H; y++)
    {
        const uint8 *src = camera_image[y];
        uint8       *dst = image_bin[y];

        for(x = 0; x < IMAGE_W; x++)
        {
            uint8 g = src[x];

            if(g < gmin) { gmin = g; }
            if(g > gmax) { gmax = g; }

            if(g >= th)
            {
                dst[x] = 255;
                white++;
            }
            else
            {
                dst[x] = 0;
            }
        }
    }

    image_white_count = white;
    image_gray_min    = gmin;
    image_gray_max    = gmax;
}

#if (IMAGE_ALLOW_SINGLE_EDGE) && (IMAGE_TRACK_MODE == IMAGE_MODE_EDGE)
//-------------------------------------------------------------------------------------------------------------------
// 函数简介      按透视关系估算某一行赛道的半宽
// 备注信息      只有在 IMAGE_ALLOW_SINGLE_EDGE = 1 时才参与编译
//-------------------------------------------------------------------------------------------------------------------
static int16 image_half_w (uint8 y)
{
    int32 span = (int32)IMAGE_SCAN_BOTTOM - (int32)IMAGE_SCAN_TOP;
    int32 d;

    if(span < 1)
    {
        span = 1;
    }

    if(y > IMAGE_SCAN_BOTTOM) { y = IMAGE_SCAN_BOTTOM; }
    if(y < IMAGE_SCAN_TOP)    { y = IMAGE_SCAN_TOP;    }

    d = (int32)IMAGE_SCAN_BOTTOM - (int32)y;            // 0 = 最近的一行，span = 最远的一行

    return (int16)((int32)IMAGE_HALF_W_NEAR
                 + ((int32)IMAGE_HALF_W_FAR - (int32)IMAGE_HALF_W_NEAR) * d / span);
}
#endif

#if ((IMAGE_CROSS_ENABLE) || (IMAGE_GAP_FILL_ENABLE)) && (IMAGE_TRACK_MODE == IMAGE_MODE_EDGE)
//-------------------------------------------------------------------------------------------------------------------
// 函数简介      给这一行分类：普通 / 十字 / 左环岛 / 右环岛
// 参数说明      row             本行二值图
//               l, r            本行找到的左右边线
//               width           本行宽度 r - l
// 返回参数      0 = 普通行   1 = 十字   2 = 左侧伸出去（左环岛）   3 = 右侧伸出去（右环岛）
// 备注信息      【十字和环岛唯一的区别就是"哪一侧伸出去"】
//                 十字：横向赛道【两侧同时】伸出去（它是两条胳膊）。
//                 环岛入口：只有【靠环的那一侧】伸出去，另一侧还是主赛道原来的边线。
//               所以每一侧单独判，命中的特征越多越确定：
//                 ② 这一侧的边线比【下面一行】同一侧边线往外跑了 IMAGE_ELEMENT_OUT_MIN 像素。
//                    逐行从下往上扫，正常透视下边线只会往中间收，往外跑一定不正常。
//                    拿下面一行当基准、自己跟自己比，不依赖任何固定的赛道宽度模型，最灵。
//                 ③ 这一侧边线【外侧】还有成片白点（IMAGE_ELEMENT_WHITE_MIN）——
//                    国一参考工程 Cirque_or_Cross() 就是这个判据（它那边左 >70、右 >120）。
//                    正常赛道边线外侧是背景（黑的），元素入口那里外侧还是白的。
//                    这条抓的是"两端还在画面里、边线被切成短短一小段"的行，② 抓不到那种。
//                 ④ 左右【都】往外挪、合起来宽了 IMAGE_ELEMENT_WIDE_MIN 以上 -> 两侧一起判十字。
//                    透视上"越远越窄"，两边同时往外挪在正常赛道上不可能；
//                    门槛比 ② 低得多，补的是"两边各宽十几像素、② 都够不到 25"的那个洞。
//-------------------------------------------------------------------------------------------------------------------
static uint8 image_row_class (const uint8 *row, int16 l, int16 r, int16 width)
{
    int16 i;
    int16 white_l = 0;
    int16 white_r = 0;
    uint8 side_l  = 0;
    uint8 side_r  = 0;

    // ② 单侧：这一侧的边线比【下面一行】同一侧往外跑了 IMAGE_ELEMENT_OUT_MIN 像素。
    //    逐行从下往上扫，正常透视下边线只会往中间收，往外跑一定不正常。
    //    拿下面一行当基准、自己跟自己比，不依赖任何固定的赛道宽度模型，最灵。
    if((image_scan_ref_l >= 0) && (l >= 0) && (l < (int16)(image_scan_ref_l - IMAGE_ELEMENT_OUT_MIN)))
    {
        side_l = 1;
    }
    if((image_scan_ref_r >= 0) && (r < IMAGE_W) && (r > (int16)(image_scan_ref_r + IMAGE_ELEMENT_OUT_MIN)))
    {
        side_r = 1;
    }

    // ④ 双侧对称变宽：左右边线【都】往外挪了，而且合起来宽了 IMAGE_ELEMENT_WIDE_MIN 以上。
    //    透视关系决定：越往上（越远）赛道只能越窄 —— 左边线往右收、右边线往左收。
    //    所以"两边同时往外挪"在几何上不可能出现在正常赛道上，只有横向白区连进来才会。
    //    ② 的门槛是每边 25 像素，而十字的横向赛道常常只是【两边各宽十几像素】，
    //    这时 ② 一条都不命中，这些行就带着过宽的中线被当成有效行用了 ——
    //    扫描会顺着它继续往上追，这就是"把较远的赛道也纳入巡线范围"。
    //    这条门槛低得多（左右合起来才 IMAGE_ELEMENT_WIDE_MIN），专门补这个洞。
    //    注意判据是"两边都往外"：弯道里两条边线是【朝同一个方向】平移的（一边进一边出），
    //    所以弯道不会命中这一条。
    if((image_scan_ref_w > 0) && (image_scan_ref_l >= 0) && (image_scan_ref_r >= 0)
    && (width > (int16)(image_scan_ref_w + IMAGE_ELEMENT_WIDE_MIN)))
    {
        int16 dl = (int16)(image_scan_ref_l - l);       // 左边线往外挪了多少（正 = 往外）
        int16 dr = (int16)(r - image_scan_ref_r);       // 右边线往外挪了多少（正 = 往外）

        if((dl > 0) && (dr > 0))
        {
            side_l = 1;
            side_r = 1;
        }
    }

    if((l >= 0) && (r < IMAGE_W))
    {
        for(i = (int16)(l - 1); (i >= 0) && (white_l < IMAGE_ELEMENT_WHITE_MIN); i--)
        {
            if(row[i] >= 128) { white_l++; }
        }
        for(i = (int16)(r + 1); (i < IMAGE_W) && (white_r < IMAGE_ELEMENT_WHITE_MIN); i++)
        {
            if(row[i] >= 128) { white_r++; }
        }

        if(white_l >= IMAGE_ELEMENT_WHITE_MIN) { side_l = 1; }   // ③ 左边线外侧还有成片白
        if(white_r >= IMAGE_ELEMENT_WHITE_MIN) { side_r = 1; }   // ③ 右边线外侧还有成片白
    }

    if(side_l && side_r) { return 1; }                  // 两侧都伸出去 -> 十字
    if(side_l)           { return 2; }                  // 只有左侧     -> 左环岛
    if(side_r)           { return 3; }                  // 只有右侧     -> 右环岛

    return 0;
}
#endif

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      扫一行，找左右边线
// 参数说明      y               行号
// 备注信息      结果直接写进 image_left_line / image_right_line / image_center_line / image_row_valid
//-------------------------------------------------------------------------------------------------------------------
static void image_scan_row (int16 y)
{
    const uint8 *row = image_bin[y];
    int16 l          = -1;
    int16 r          = -1;
    int16 cx         = IMAGE_CENTER_X;
    int16 i;
    int16 width;

    image_row_valid[y] = 0;

#if (IMAGE_TRACK_MODE == IMAGE_MODE_LINE)
    // ---------------- 白底黑线：中心附近那条黑线就是赛道 ----------------
    if(row[cx] >= 128)                                  // 中心是白的，先往两边找最近的黑点
    {
        for(i = cx; i >= 0; i--)
        {
            if(row[i] < 128) { cx = i; break; }
        }

        if(i < 0)
        {
            for(i = cx; i < IMAGE_W; i++)
            {
                if(row[i] < 128) { cx = i; break; }
            }

            if(i >= IMAGE_W)                            // 整行都是白的，这行没有线
            {
                return;
            }
        }
    }

    // 现在 cx 一定落在黑线上，往两边扩到线的边界（也就是第一个白点）
    for(l = cx; l >= 0;       l--) { if(row[l] >= 128) { l++; break; } }
    for(r = cx; r < IMAGE_W;  r++) { if(row[r] >= 128) { r--; break; } }

    if(l < 0)        { l = 0; }
    if(r >= IMAGE_W) { r = IMAGE_W - 1; }

    width = r - l;
    if((width < IMAGE_LINE_W_MIN) || (width > IMAGE_LINE_W_MAX))
    {
        return;
    }
#else
    // ---------------- 白底黑边（智能车赛道）：找赛道两侧的黑线 ----------------
#if (IMAGE_CHASE_SEED)
    // 追线种子：优先从"上一有效行的中线"开始找，而不是每行都从图像正中开始。
    // 弯道里赛道跑到画面一边时，图像正中已经是画面外侧的背景了，从正中找必然找偏。
    if(image_scan_ref_w > 0)
    {
        cx = (int16)((image_scan_ref_l + image_scan_ref_r) / 2);
    }
    else
    {
        cx = IMAGE_CENTER_X;
    }

    if(cx < 0)           { cx = 0; }
    if(cx > IMAGE_W - 1) { cx = IMAGE_W - 1; }
#else
    cx = IMAGE_CENTER_X;
#endif

    if(row[cx] < 128)                                   // 种子上是黑的（十字 / 压边线 / 赛道偏在一侧）
    {
        // 左右【对称】地往外找最近的白点。
        // 原来的写法只往右找：赛道偏在画面左侧时整行会被丢掉，
        // 于是左右弯的可用行数不一样 —— 这是"一边转得动、一边转不动"的直接原因之一。
        int16 found = -1;

        for(i = 1; i < IMAGE_W; i++)
        {
            if(((cx - i) >= 0)      && (row[cx - i] >= 128)) { found = (int16)(cx - i); break; }
            if(((cx + i) < IMAGE_W) && (row[cx + i] >= 128)) { found = (int16)(cx + i); break; }
        }

        if(found < 0)                                   // 整行全黑
        {
            return;
        }

        cx = found;
    }

    for(l = cx; l >= 0;      l--) { if(row[l] < 128) { break; } }    // 往左第一条黑线
    for(r = cx; r < IMAGE_W; r++) { if(row[r] < 128) { break; } }    // 往右第一条黑线

    // ---- 逐行丢边标志（必须在下面"单边补线"【之前】记，补过之后就看不出来了）----
    //   环岛检测（ring.c）照 E07 的写法全靠这个量：lost = 这一侧的边线跑到画面外去了。
    //   E07 那边判的是 left_point[y] < 5 / right_point[y] > 75，本质一样。
    image_lost_l[y] = (l < 0)        ? 1 : 0;
    image_lost_r[y] = (r >= IMAGE_W) ? 1 : 0;
    image_raw_l[y] = l;
    image_raw_r[y] = r;

    if((l < 0) && (r >= IMAGE_W))                       // 两边都没找到：这一行一条黑线都没有
    {
        // 整行全白。它【没有左右信息】，所以不能当场断定是十字还是环岛：
        //   十字的横向赛道比画面还宽时会这样，环岛入口白区特别大时也会这样。
        // 所以这里单独记一笔，等整帧扫完和其他证据一起判（见 image_process）。
        // 标记照打，好让 image_fill_gaps() 把它补成一条连续的中线。
        image_row_gap[y] = 1;
        if(image_white_row_count < 0xFFFF) { image_white_row_count++; }
        return;
    }

    if((l < 0) || (r >= IMAGE_W))                       // 只找到一边
    {
#if (IMAGE_ALLOW_SINGLE_EDGE) && (IMAGE_TRACK_MODE == IMAGE_MODE_EDGE)
        // 用"下面最近一条有效行的赛道宽度"把丢掉的那一边补出来。
        // 比固定透视模型准得多：宽度是跟着实际赛道走的，不是猜的。
        // 最底下几行还没有参考值，这时才退回按透视估算。
        int16 w = image_scan_ref_w;

        if(w <= 0)
        {
            w = 2 * image_half_w((uint8)y);
        }

        if(l < 0)                                       // 左边线跑出画面 -> 往左补
        {
            l = (int16)(r - w);
            if(l < 0) { l = 0; }
        }
        else                                            // 右边线跑出画面 -> 往右补
        {
            r = (int16)(l + w);
            if(r > IMAGE_W - 1) { r = IMAGE_W - 1; }
        }
#else
        return;                                         // 只找到一边，这行作废
#endif
    }

    width = r - l;
    if((width < IMAGE_WIDTH_MIN) || (width > IMAGE_WIDTH_MAX))
    {
        return;
    }

#if ((IMAGE_ELEMENT_ENABLE) || (IMAGE_GAP_FILL_ENABLE)) && (IMAGE_TRACK_MODE == IMAGE_MODE_EDGE)
    // 分类：0 普通 / 1 十字 / 2 左环岛 / 3 右环岛。命中的按"元素行"处理。
    // 【这里必须 return，绝不能把这一行存下来】：
    //   ① 元素行那一百多像素的边线一旦被当成有效行参与偏差，中线就跑到元素正中间去了，
    //      车会跟着它偏（"跟着较远的赛道走"就是这么来的）；
    //   ② 追线参考 image_scan_ref_l/r/w 也会被这个错误边线污染，后面所有单边补线全被带偏。
    // 标成 gap 之后交给 image_fill_gaps()，用上下真正有效行的中线补成一条连续的线。
    {
        uint8 cls = image_row_class(row, l, r, width);

        if(0 != cls)
        {
            image_row_gap[y] = 1;

            if(1 == cls)                                        // 十字：两侧都伸出去
            {
                if(image_cross_row_count < 0xFFFF) { image_cross_row_count++; }
            }
            else if(2 == cls)                                   // 左环岛：只有左侧伸出去
            {
                if(image_ring_row_l < 0xFFFF) { image_ring_row_l++; }
            }
            else                                                // 右环岛：只有右侧伸出去
            {
                if(image_ring_row_r < 0xFFFF) { image_ring_row_r++; }
            }

            return;
        }
    }
#endif
#endif

    image_left_line[y]   = l;
    image_right_line[y]  = r;
    image_center_line[y] = (int16)((l + r) / 2);
    image_row_valid[y]   = 1;

#if (IMAGE_TRACK_MODE == IMAGE_MODE_EDGE)
    // 记下来给"更高（更远）的那一行"追线用
    image_scan_ref_l = l;
    image_scan_ref_r = r;
    image_scan_ref_w = width;
#endif
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介      十字补线：整行找不到边线的行，用上下有效行的中线插值补出来
// 备注信息      思路照国一参考工程的 RouteFilter()：某一行左右边线都丢掉时，
//               找它上下最近的、两边都正常的行，用这两条中线连一条直线，把中间补起来。
//
//               十字路口内部正是这种情况：边线全部消失，但前后都还看得见自己的赛道。
//               不补的话这些行会全部作废，有效行数掉到 IMAGE_MIN_VALID_ROWS 以下，
//               状态机就判成丢赛道 -> 停车，车自然过不了十字。
//
//               上下都有有效行  -> 线性插值（最稳，不会漂）
//               只有下面有（十字一直延伸到画面顶） -> 用下面两行的斜率外推
//               注意锚点只用【本帧原生扫出来的有效行】，不用刚刚补出来的行，
//               这样整段补线都落在同一条直线上，不会一段一段累积误差。
//-------------------------------------------------------------------------------------------------------------------
static void image_fill_gaps (void)
{
    int16 y;

    image_gap_filled_rows = 0;

    for(y = IMAGE_SCAN_BOTTOM; y >= IMAGE_SCAN_TOP; y -= IMAGE_SCAN_STEP)
    {
        int16 j;
        int16 yb = -1;                                  // 下面最近的一条原生有效行
        int16 ya = -1;                                  // 上面最近的一条原生有效行

        if(0 == image_row_gap[y]) { continue; }         // 这行不是"整行无边线"，不用补

        for(j = (int16)(y - IMAGE_SCAN_STEP); j >= IMAGE_SCAN_TOP; j -= IMAGE_SCAN_STEP)
        {
            if(image_row_valid[j] && (0 == image_row_gap[j])) { yb = j; break; }
        }
        for(j = (int16)(y + IMAGE_SCAN_STEP); j <= IMAGE_SCAN_BOTTOM; j += IMAGE_SCAN_STEP)
        {
            if(image_row_valid[j] && (0 == image_row_gap[j])) { ya = j; break; }
        }

        if((yb >= 0) && (ya >= 0))
        {
            int32 cb = (int32)image_center_line[yb];
            int32 ca = (int32)image_center_line[ya];
            int32 dy = (int32)ya - (int32)yb;

            image_center_line[y] = (int16)(cb + (ca - cb) * ((int32)y - (int32)yb) / dy);
        }
        else if(yb >= 0)
        {
            int16 yb2 = (int16)(yb - IMAGE_SCAN_STEP);

            if((yb2 >= IMAGE_SCAN_TOP) && image_row_valid[yb2] && (0 == image_row_gap[yb2]))
            {
                int32 dc = (int32)image_center_line[yb] - (int32)image_center_line[yb2];
                int32 dy = (int32)yb - (int32)yb2;

                image_center_line[y] = (int16)((int32)image_center_line[yb]
                                     + dc * ((int32)y - (int32)yb) / dy);
            }
            else
            {
                image_center_line[y] = image_center_line[yb];   // 只够一行参考，就正着往前走
            }
        }
        else
        {
            continue;                                   // 上下都没有有效行，补不了
        }

        if(image_center_line[y] < 0)           { image_center_line[y] = 0; }
        if(image_center_line[y] > IMAGE_W - 1) { image_center_line[y] = IMAGE_W - 1; }

        // 左右边线只是为了屏幕上画图好看，按最近一次参考宽度摆在中线两侧（偏差只用中线）
        if(image_scan_ref_w > 0)
        {
            image_left_line[y]  = (int16)(image_center_line[y] - image_scan_ref_w / 2);
            image_right_line[y] = (int16)(image_center_line[y] + image_scan_ref_w / 2);
        }
        else
        {
            image_left_line[y]  = image_center_line[y];
            image_right_line[y] = image_center_line[y];
        }

        if(image_left_line[y]  < 0)            { image_left_line[y]  = 0; }
        if(image_right_line[y] > IMAGE_W - 1)  { image_right_line[y] = IMAGE_W - 1; }

        image_row_valid[y] = 1;
        image_gap_filled_rows++;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      按有效行算综合偏差
// 备注信息      WEIGHTED  模式：权重 = min(离底部的距离, IMAGE_ERROR_W_MAX)，越远看的越重 = 自带前瞻。
//                          但【必须配合有限前瞻】：比前瞻行更远的行权重渐隐到 0，再远完全不参与偏差。
//                          否则画面最上面几十行（远处的赛道）权重和中间一样大，车会被它们拽偏 ——
//                          这就是"把较远的赛道也纳入巡线范围"。详见 image.h 里那段说明。
//               LOOKAHEAD 模式：只看 IMAGE_LOOKAHEAD_ROW 上下几行
//               两种模式都同时给出 near（最近有效行）和 far（最远有效行）的偏差，方便在屏幕上对着调
//-------------------------------------------------------------------------------------------------------------------
void image_calc_error (void)
{
    int16 y;
    int16 dev;
    int16 err_near  = 0;
    int16 err_far   = 0;
    int32 w_sum     = 0;
    int32 e_sum     = 0;
    uint8 near_flag = 0;
    uint8 valid     = 0;
    uint8 top       = IMAGE_H;
    uint8 far_used  = IMAGE_H;                          // 本帧真正参与偏差的最远行

    for(y = IMAGE_SCAN_BOTTOM; y >= IMAGE_SCAN_TOP; y -= IMAGE_SCAN_STEP)
    {
        if(0 == image_row_valid[y])
        {
            continue;
        }

        dev = (int16)(image_center_line[y] - (IMAGE_CENTER_X + IMAGE_CENTER_OFFSET));

        if(0 == near_flag)
        {
            err_near  = dev;
            near_flag = 1;
        }
        err_far = dev;

        valid++;
        if((uint8)y < top)
        {
            top = (uint8)y;
        }

#if (IMAGE_ERROR_MODE == IMAGE_ERROR_MODE_LOOKAHEAD)
        if((y <= (IMAGE_LOOKAHEAD_ROW + IMAGE_LOOKAHEAD_SPAN))
        && (y >= (IMAGE_LOOKAHEAD_ROW - IMAGE_LOOKAHEAD_SPAN)))
        {
            e_sum += dev;
            w_sum += 1;

            if((uint8)y < far_used) { far_used = (uint8)y; }
        }
#else
        {
            int32 w = (int32)IMAGE_SCAN_BOTTOM - (int32)y;

            if(w > IMAGE_ERROR_W_MAX) { w = IMAGE_ERROR_W_MAX; }
            if(w < 1)                 { w = 1; }

            // ---------------- 有限前瞻：比前瞻行更远的行，权重渐隐，再远就完全不参与偏差 ----------------
            // 这一段就是"不把较远的赛道纳入巡线范围"。前面几步（near / far / top / valid）
            // 已经在上面算完了，所以这里 continue 只是跳过这一行对偏差的贡献，不影响其它统计。
            if(y < IMAGE_LOOKAHEAD_ROW)
            {
                int32 d = (int32)IMAGE_LOOKAHEAD_ROW - (int32)y;    // 比前瞻行远了多少行

                if(d >= (int32)IMAGE_LOOKAHEAD_FADE)
                {
                    continue;                                       // 太远了：这一行不参与偏差
                }

                // 线性渐隐：贴着前瞻行还是满权重，越往外越轻
                w = w * ((int32)IMAGE_LOOKAHEAD_FADE - d) / (int32)IMAGE_LOOKAHEAD_FADE;
            }

            e_sum += w * (int32)dev;
            w_sum += w;

            if((uint8)y < far_used) { far_used = (uint8)y; }
        }
#endif
    }

    image_valid_row_count = valid;
    image_valid_top_row   = top;
    image_error_near      = err_near;
    image_error_far       = err_far;
    image_error_far_used  = far_used;

    if(w_sum > 0)
    {
        image_error = (int16)(e_sum / w_sum);
    }
#if (IMAGE_ERROR_MODE == IMAGE_ERROR_MODE_LOOKAHEAD)
    else
    {
        // 前瞻行那一段一行有效行都没有（急弯里很常见），退回到 near / far 的平均
        image_error = (int16)((err_near + err_far) / 2);
    }
#else
    else
    {
        // 极端情况：所有有效行都在有限前瞻的可见范围之外。
        // 退回最近那一条有效行的偏差。【绝不能把偏差清成 0】——
        // 那样舵机会瞬间回正，车会直着一头冲出赛道。
        image_error = err_near;
    }
#endif

    if(image_error >  IMAGE_ERROR_LIMIT) { image_error =  IMAGE_ERROR_LIMIT; }
    if(image_error < -IMAGE_ERROR_LIMIT) { image_error = -IMAGE_ERROR_LIMIT; }

    image_off_track = (valid < IMAGE_MIN_VALID_ROWS) ? 1 : 0;
}

//==================================================== 对外接口 ====================================================
//-------------------------------------------------------------------------------------------------------------------
// 函数简介      图像模块初始化
//-------------------------------------------------------------------------------------------------------------------
void image_init (void)
{
    uint16 i;

    memset(image_bin,        0, sizeof(image_bin));
    memset(image_row_valid,  0, sizeof(image_row_valid));

    for(i = 0; i < IMAGE_H; i++)
    {
        image_left_line[i]   = -1;
        image_right_line[i]  = -1;
        image_center_line[i] = IMAGE_CENTER_X;
    }

    image_threshold       = IMAGE_FIXED_THRESHOLD;
    image_otsu_threshold  = IMAGE_FIXED_THRESHOLD;
    image_gray_min        = 0;
    image_gray_max        = 0;
    image_white_count     = 0;
    image_valid_row_count = 0;
    image_valid_top_row   = IMAGE_H;
    image_error           = 0;
    image_error_near      = 0;
    image_error_far       = 0;
    image_error_far_used  = IMAGE_H;
    image_off_track       = 1;

    memset(image_row_gap,    0, sizeof(image_row_gap));
    image_gap_filled_rows = 0;
    image_cross_rows      = 0;
    image_cross_flag      = 0;
    image_cross_frames    = 0;
    image_white_rows      = 0;
    image_ring_left_rows  = 0;
    image_ring_right_rows = 0;
    image_ring_side       = 0;

    image_cross_row_count = 0;
    image_white_row_count = 0;
    image_ring_row_l      = 0;
    image_ring_row_r      = 0;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      处理一帧图像（camera_update() 返回 1 之后调用一次）
// 备注信息      入口是 camera_image，出口是 image_error
//-------------------------------------------------------------------------------------------------------------------
void image_process (void)
{
    int16 y;
    int16 th;

    // ---------------------- 1. 算阈值 ----------------------
#if (IMAGE_USE_OTSU)
    image_otsu_threshold = image_otsu();
#else
    image_otsu_threshold = IMAGE_FIXED_THRESHOLD;
#endif

    th = (int16)image_otsu_threshold + image_threshold_offset;
    if(th < IMAGE_THRESHOLD_MIN) { th = IMAGE_THRESHOLD_MIN; }
    if(th > IMAGE_THRESHOLD_MAX) { th = IMAGE_THRESHOLD_MAX; }
    image_threshold = (uint8)th;

    // ---------------------- 2. 二值化 ----------------------
    image_binarize(image_threshold);

    // ---------------------- 3. 逐行找边线 ----------------------
    // 先把所有行标记成无效，避免上一帧的残留值被当成这一帧的结果
    memset(image_row_valid, 0, sizeof(image_row_valid));

#if (IMAGE_GAP_FILL_ENABLE) || (IMAGE_ELEMENT_ENABLE) || (IMAGE_CROSS_ENABLE)
    memset(image_row_gap, 0, sizeof(image_row_gap));    // 元素行标记每帧重新统计
    image_cross_row_count = 0;
    image_white_row_count = 0;
    image_ring_row_l      = 0;
    image_ring_row_r      = 0;
#endif
    image_gap_filled_rows = 0;

    // 追线参考清零：从最下面一行重新建立起这条连续曲线
    image_scan_ref_l = -1;
    image_scan_ref_r = -1;
    image_scan_ref_w = 0;

    for(y = IMAGE_SCAN_BOTTOM; y >= IMAGE_SCAN_TOP; y -= IMAGE_SCAN_STEP)
    {
        image_scan_row(y);
    }

    // ---------------------- 4. 十字补线 ----------------------
#if (IMAGE_GAP_FILL_ENABLE)
    // 整行没有边线的行（十字中间就是），用上下有效行的中线插值补出来。
    // 补完有效行数就保住了，车能直着穿过十字，而不是被判成丢赛道停车。
    image_fill_gaps();
#endif

    // ---------------------- 5. 算偏差 ----------------------
    image_calc_error();

    // ---------------------- 6. 元素判决：十字 / 环岛 ----------------------
#if (IMAGE_ELEMENT_ENABLE) || (IMAGE_CROSS_ENABLE)
    image_white_rows      = (uint8)((image_white_row_count > 255) ? 255 : image_white_row_count);
    image_ring_left_rows  = (uint8)((image_ring_row_l      > 255) ? 255 : image_ring_row_l);
    image_ring_right_rows = (uint8)((image_ring_row_r      > 255) ? 255 : image_ring_row_r);

    // ---- 先判环岛：有足够的"只有一侧伸出去"的行，就按环岛算 ----
    // 单侧证据比"整行全白"更具体，所以优先采信。
    if((image_ring_left_rows >= IMAGE_RING_ROW_MIN) && (image_ring_left_rows >= image_ring_right_rows))
    {
        image_ring_side = 1;                            // 左环岛
    }
    else if(image_ring_right_rows >= IMAGE_RING_ROW_MIN)
    {
        image_ring_side = 2;                            // 右环岛
    }
    else
    {
        image_ring_side = 0;
    }

    // ---- 再判十字 ----
    // "整行全白"的行没有左右信息，把它并进十字的行数里 —— 但要满足一个前提：
    // 没有任何单侧证据。否则那些全白行其实是环岛入口的一部分，不能算成十字。
    if(0 == image_ring_side)
    {
        uint16 n = (uint16)(image_cross_row_count + image_white_row_count);

        image_cross_rows = (uint8)((n > 255) ? 255 : n);
    }
    else
    {
        image_cross_rows = (uint8)((image_cross_row_count > 255) ? 255 : image_cross_row_count);
    }

    // 判定要同时满足两条：
    //   ① 出现元素行 —— 十字/环岛内部的特征；
    //   ② 同时还看得见自己的赛道 —— 否则整幅发白（曝光过度/冲出赛道）会被误判成十字。
    if((image_cross_rows >= IMAGE_CROSS_ROW_MIN) && (image_valid_row_count >= IMAGE_CROSS_VALID_MIN))
    {
        if(image_cross_frames < 0xFFFF) { image_cross_frames++; }
    }
    else
    {
        image_cross_frames = 0;                         // 不连续了，重新计数
    }

    // 持续超过 IMAGE_CROSS_MAX_FRAMES 还没出去，那多半不是十字
    // （比如曝光过度整幅发白、或者真的冲出赛道了），这时必须回到"丢赛道"的正常处理，
    // 否则车会一直假装在十字里、闭着眼往前冲。
    image_cross_flag = ((image_cross_frames > 0) && (image_cross_frames <= IMAGE_CROSS_MAX_FRAMES)) ? 1 : 0;

    if(image_cross_flag)
    {
        // 十字不是丢赛道。国一参考工程里十字标志位的主要用途也是这个：
        // 别让别的逻辑被十字的异常特征带偏（它那边是禁掉直道加速判断）。
        // 注意这里【不】把 image_error 清 0 —— 补出来的中线本身就是一条连续的线，
        // 它给出的偏差是有意义的（延续进十字之前的朝向），比强行清 0 更好。
        image_off_track = 0;
    }

    if(0 != image_ring_side)
    {
        // 环岛入口那几行同样不是丢赛道，先别让它把状态机带进停车。
        // 【注意】"绕进环岛"的处理还没加 —— 现在它和十字一样只会直着走。
        image_off_track = 0;
    }
#else
    image_white_rows      = 0;
    image_ring_left_rows  = 0;
    image_ring_right_rows = 0;
    image_ring_side       = 0;
    image_cross_rows      = 0;
    image_cross_flag      = 0;
    image_cross_frames    = 0;
#endif

    // ---------------------- 7. 环岛状态机（已关闭）----------------------
    // 【为什么放最后】它要用本帧最终的 image_lost_l/r、边线、路宽这些量来判环岛；
    // 判上之后它会【改写 image_center_line[] 并重算 image_error】——
    // 放在最后，后面 track.c / display.c 看到的就已经是环岛目标线了，那两个文件不用操心。
    //
    // 【当前关闭环岛】不调用 ring_update()：车不会绕进环，环岛入口按直道补线通过。
    //   要恢复：把 RING_ENABLE 改回 1，并取消下面这一行的注释。
    // ring_update();
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介      在线改二值化阈值偏移（VOFA+ 的 thr 命令）
// 参数说明      offset          -60 ~ +60，正数阈值更高（更容易把背景判黑）
//-------------------------------------------------------------------------------------------------------------------
void image_set_threshold_offset (int16 offset)
{
    if(offset >  60) { offset =  60; }
    if(offset < -60) { offset = -60; }

    image_threshold_offset = offset;
}
