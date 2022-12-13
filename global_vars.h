#ifndef __GLOBAL_VARS_H__
#define __GLOBAL_VARS_H__

extern uchar mode;        // 运行模式
extern uint seconds;      // 本周期经过的秒数
extern uint milliseconds; // 毫秒数,为1000时令seconds++
extern uint cycle;        // 红绿灯运行一轮的周期
extern uchar remain_time[4];

#endif
