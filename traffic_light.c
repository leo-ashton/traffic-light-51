#include "headfile.h"

void Write7219(unsigned char address, unsigned char dat)
{
    unsigned char i;
    LOAD_7219 = 0; //拉低片选线，选中器件
    //发送地址
    for (i = 0; i < 8; i++) //移位循环8次
    {
        CLK_7219 = 0;                     //清零时钟总线
        DIN_7219 = (bit)(address & 0x80); //每次取高字节
        address <<= 1;                    //左移一位
        CLK_7219 = 1;                     //时钟上升沿，发送地址
    }
    //发送数据
    for (i = 0; i < 8; i++)
    {
        CLK_7219 = 0;
        DIN_7219 = (bit)(dat & 0x80);
        dat <<= 1;
        CLK_7219 = 1; //时钟上升沿，发送数据
    }
    LOAD_7219 = 1; //发送结束，上升沿锁存数据
}

// MAX7219初始化，设置MAX7219内部的控制寄存器
void Init7219(void)
{
    Write7219(SHUT_DOWN, 0x01);    //开启正常工作模式（0xX1）
    Write7219(DISPLAY_TEST, 0x00); //选择工作模式（0xX0）
    Write7219(DECODE_MODE, 0xff);  //选用全译码模式
    Write7219(SCAN_LIMIT, 0x07);   // 8只LED全用
    Write7219(INTENSITY, 0x04);    //设置初始亮度
}

void DisplayDigit(uchar val, direction dir)
{
    if (dir == EAST)
    {

    }
    if (dir == SOUTH)
    {
    }
    if (dir == WEST)
    {
    }
    if (dir == NORTH)
    {
    }
}
