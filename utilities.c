#include "headfile.h"
// 延时
void DelayMS(uint x)
{
    uchar i;
    while (x--)
        for (i = 0; i < 120; i++)
            ;
}

void SerialOut595(char ds_pin, char sh_pin, char st_pin, uchar series[], int len)
{
    /**
     * @brief 向74HC595输入series指定的数据
     * @param ds_pin 74HC595的串行数据输入端
     * @param sh_pin 74HC595的串行输入端的时钟信号线,每载入一位,使用一个上升沿将该位移入锁存器中
     *
     */
    // @brief: 向74HC595输入series指定的数据
    // @param: ds_pin
    uint i = 0;
    for (i = 0; i < len; i++)
    {
        ds_pin = series[i] & 0x01;
        sh_pin = 0;
        Delay100us();
        sh_pin = 1;
        sh_pin = 0; // 产生一个上升沿
    }
    st_pin = 0;
    Delay100us();
    st_pin = 1; // 产生一个上升沿
    st_pin = 0;
}

void Delay100us() //@12.000MHz
{
    unsigned char i;

    _nop_();
    i = 47;
    while (--i)
        ;
}

void Hc595SendByte(uchar dat)
{
    uchar a;

    SRCLK = 1;
    RCLK = 1;

    for (a = 0; a < 8; a++) // 发送8位数
    {
        SER = dat >> 7; // 从最高位开始发送
        dat <<= 1;

        SRCLK = 0; // 发送时序
        _nop_();
        _nop_();
        SRCLK = 1;
    }
    RCLK = 0;
    _nop_();
    _nop_();
    RCLK = 1;
}

uint BinarySeries2Uint(uchar series[], uchar len)
{
    // TODO implement this
    return 0;
}
