#include "headfile.h"

uint seconds = 0;      // 本周期经过的秒数
uint milliseconds = 0; // 毫秒数,为1000时令seconds++
uint cycle = 180;      // 红绿灯运行一轮的周期