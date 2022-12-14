#include "headfile.h"

void Timer0Init(void) // 1毫秒@12.000MHz
{
    TMOD &= 0xF0; // 设置定时器模式
    TL0 = 0x20;   // 设置定时初始值
    TH0 = 0xD1;   // 设置定时初始值
    TF0 = 0;      // 清除TF0标志
    TR0 = 1;      // 定时器0开始计时
}

void UpdateTimer0()
{
    // 定时器中断服务程序
    // 重置初值
    TL0 = 0x20; // 设置定时初始值
    TH0 = 0xD1; // 设置定时初始值
    milliseconds++;
    if (milliseconds > 300)
    {
        seconds++;
        new_second_flag = 1;
        new_second_flag_setting = 1;
        new_second_flag_emergency = 1;
        milliseconds = 0;
        if (seconds > cycle)
        {
            seconds = 0;
        }
    }
}