#include "headfile.h"

uchar mode = DEBUGGING;
ushort seconds = 0;      // 本周期经过的秒数
ushort milliseconds = 0; // 毫秒数,为1000时令seconds++
ushort cycle = 180;      // 红绿灯运行一轮的周期
bit new_second_flag = 0;
bit new_second_flag_setting = 0;
