#include "headfile.h"

void Timer0Init(void) // 1毫秒@12.000MHz
{
    // AUXR |= 0x80; //定时器时钟1T模式
    TMOD &= 0xF0; //设置定时器模式
    TL0 = 0x20;   //设置定时初始值
    TH0 = 0xD1;   //设置定时初始值
    TF0 = 0;      //清除TF0标志
    TR0 = 1;      //定时器0开始计时
}

void Timer1Init(void) // 5毫秒@12.000MHz
{
    TMOD |= 0x0F; //设置定时器模式
    TL1 = 0xA0;   //设置定时初始值
    TH1 = 0x15;   //设置定时初始值
    TF1 = 0;      //清除TF1标志
    TR1 = 1;      //定时器1开始计时
}

void UpdateTimer0()
{
    // 定时器中断服务程序
    // 重置初值
    TL0 = 0x20; //设置定时初始值
    TH0 = 0xD1; //设置定时初始值
    milliseconds++;
    if (milliseconds > 1000)
    {
        seconds++;
        milliseconds = 0;
        if (seconds > cycle)
        {
            seconds = 0;
        }
        RED_EAST = ~RED_EAST;
    }
}