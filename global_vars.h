#ifndef __GLOBAL_VARS_H__
#define __GLOBAL_VARS_H__

extern uchar mode;          // 运行模式
extern ushort seconds;      // 本周期经过的秒数
extern ushort milliseconds; // 毫秒数,为1000时令seconds++
extern ushort cycle;        // 红绿灯运行一轮的周期
extern uchar remain_time[4];
extern bit new_second_flag;
extern bit new_second_flag_setting;

#endif
