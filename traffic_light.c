#include "headfile.h"

#define DONT_DISPLAY 255 // DisplayDigit函数的val为该值时，两位均不显示
#define SEG_OFF 15       // Write7219向某位数码管写入该值时，该位数码管不显示

// static uchar last_east, last_south, last_west, last_north;
static uchar last_display_val[4];

typedef enum digs
{
    EAST_TENS = 1,
    EAST_ONES,
    SOUTH_TENS,
    SOUTH_ONES,
    WEST_TENS,
    WEST_ONES,
    NORTH_TENS,
    NORTH_ONES,
} digs;

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
    // @brief 在dir方向展示val,并将本次显示的值保存在last_变量中
    // @note  这是唯一允许调用数码管显示的方式
    static uchar ones, tens;

    if (val == DONT_DISPLAY)
    {
        // 两位均不显示
        ones = SEG_OFF;
        tens = SEG_OFF;
    }
    else
    {
        ones = val % 10;
        tens = (val / 10 % 10) == 0 ? SEG_OFF : (val / 10 % 10); // 如果十位为0，则不显示
    }

    if (dir == EAST)
    {
        Write7219(EAST_ONES, ones);
        Write7219(EAST_TENS, tens);
    }
    if (dir == SOUTH)
    {
        Write7219(SOUTH_ONES, ones);
        Write7219(SOUTH_TENS, tens);
    }
    if (dir == WEST)
    {
        Write7219(WEST_ONES, ones);
        Write7219(WEST_TENS, tens);
    }
    if (dir == NORTH)
    {
        Write7219(NORTH_ONES, ones);
        Write7219(NORTH_TENS, tens);
    }
    if (val == DONT_DISPLAY)
        last_display_val[dir] = last_display_val[dir];
    else
        last_display_val[dir] = val;
}

void DisplayDigitDemo()
{
    Write7219(1, 1); // 15为全灭
    Write7219(2, 2);
    Write7219(3, 3);
    Write7219(4, 4);
    Write7219(5, 5);
    Write7219(6, 6);
    Write7219(7, 7);
    Write7219(8, 8);
    // Write7219(1, 15); // 15为全灭
}

void TrafficLight()
{
}

void Setting()
{
    direction dir = NORTH;
    while (mode == SETTING)
    {
        ToggleSegs(dir);
        DelayMS(1000);
    }
    ClockReset();
}

void ClockReset()
{
    // @brief 重置定时器0以及毫秒、秒计数器
    TR0 = 0;    //定时器0开始计时
    ET0 = 0;    // 关闭定时器0中断
    TL0 = 0x20; //设置定时初始值
    TH0 = 0xD1; //设置定时初始值
    milliseconds = 0;
    seconds = 0;
    ET0 = 1;
    TR0 = 1; //定时器0开始计时
}

void ToggleSegs(direction dir)
{
    // @note: 向方向写入DONT_DISPLAY可以使数码管熄灭
    static uchar ON[4] = {1, 1, 1, 1}; // 4个方向的开关状态
    if (ON[dir] == 1)
    {
        DisplayDigit(DONT_DISPLAY, dir);
        ON[dir] = 0;
    }
    else
    {
        DisplayDigit(last_display_val[dir], dir);
        ON[dir] = 1;
    }
}
