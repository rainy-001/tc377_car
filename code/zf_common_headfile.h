/*********************************************************************************************************************
* 文件名称          zf_common_headfile.h （本工程自己的转发头，不是逐飞库原文件）
* 功能简介          把 "zf_common_headfile.h" 明确指到【本工程】libraries 下的那一份
*
* ******************************* 为什么需要这个文件 *******************************
*   本工程的 include 路径里混进了另一份工程
*   （Seekfree_TC377_Opensource_Library\...）的一百多条目录，而且全部排在
*   本工程 libraries\ 的前面。后果是：
*
*     1. code\ 下的文件写 #include "zf_common_headfile.h" 时，
*        命中的是【示例工程】那份头文件（5289 字节，比本工程的多 586 字节）；
*     2. 那份头文件会把示例工程的整套应用头文件拉进每一个编译单元：
*        tc377_board.h / motor.h / servo.h / encoder.h / image.h /
*        scheduler.h / menu.h / vision_*.h / vofa.h / imu.h / voltage.h ...
*     3. 其中 motor.h / encoder.h / image.h / servo.h 和本工程【同名】，
*        连 include guard 都一样（_motor_h_ / _encoder_h_ / _image_h_ / _servo_h_），
*        谁先被包含谁就赢，于是本工程的头文件被整个跳过 —— 编译报
*        "implicit declaration of function motor_stop_all"、"undeclared identifier
*        SERVO_MID_DUTY" 这些莫名其妙的错误。
*
*   code\ 下的 .c 文件靠 C 语言"先找当前文件所在目录"的规则侥幸拿到自己的头文件，
*   user\ 下的 .c 文件没有这个便利，所以只有 user\ 下报错。
*
* ******************************* 这个文件怎么起作用 *******************************
*   把这个文件放在 code\ 目录下，code\ 里再写 #include "zf_common_headfile.h" 时，
*   编译器第一步就是找 code\ 目录，于是百分之百命中这个转发头，
*   再由它显式地包含本工程 libraries\zf_common\ 下的那一份。
*   这样就和 include 路径的顺序彻底无关了。
*
*   user\ 目录下还有一个同样的转发头（指向 code\ 这一个），作用相同。
*********************************************************************************************************************/

#ifndef _zf_common_headfile_forward_h_
#define _zf_common_headfile_forward_h_

#include "../libraries/zf_common/zf_common_headfile.h"

#endif