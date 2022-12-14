#include "headfile.h"

#define DONT_DISPLAY 255 // DisplayDigit函数的val为该值时，两位均不显示
#define SEG_OFF 15       // Write7219向某位数码管写入该值时，该位数码管不显示

static uchar last_display_val[DIRECTION_MAX];
digs_addr code ONES_BIT[] = {EAST_ONES, SOUTH_ONES, WEST_ONES, NORTH_ONES};
digs_addr code TENS_BIT[] = {EAST_TENS, SOUTH_TENS, WEST_TENS, NORTH_TENS};
uchar remain_time[DIRECTION_MAX];
uchar light_time[DIRECTION_MAX][3]; // 存放四个方向的三色灯的设置时间
uchar current_color[DIRECTION_MAX];
TrafficLightColor next_status;

void TrafficLightInit()
{
    /**
     * @brief 初始化交通灯的值
     * @todo 注意修改初始时间
     */
    uchar i;
    for (i = 0; i < TRAFFIC_LIGHT_MAX; i++)
    {
        SetLedBit(i, LED_OFF); // 关掉所有灯
    }
    // 初始状态
    light_time[EAST][RED] = 60, light_time[EAST][YELLOW] = 5, light_time[EAST][GREEN] = 15;
    light_time[SOUTH][RED] = 60, light_time[SOUTH][YELLOW] = 5, light_time[SOUTH][GREEN] = 15;
    light_time[WEST][RED] = 60, light_time[WEST][YELLOW] = 5, light_time[WEST][GREEN] = 15;
    light_time[NORTH][RED] = 60, light_time[NORTH][YELLOW] = 5, light_time[NORTH][GREEN] = 15;

    remain_time[EAST] = 15;
    remain_time[SOUTH] = 20;
    remain_time[WEST] = 40;
    remain_time[NORTH] = 60;

    SetLedColor(EAST, GREEN);
    SetLedColor(SOUTH, RED);
    SetLedColor(WEST, RED);
    SetLedColor(NORTH, RED);
}

void Write7219(unsigned char address, unsigned char dat)
{
    unsigned char i;
    LOAD_7219 = 0; // 拉低片选线，选中器件
    // 发送地址
    for (i = 0; i < 8; i++) // 移位循环8次
    {
        CLK_7219 = 0;                     // 清零时钟总线
        DIN_7219 = (bit)(address & 0x80); // 每次取高字节
        address <<= 1;                    // 左移一位
        CLK_7219 = 1;                     // 时钟上升沿，发送地址
    }
    // 发送数据
    for (i = 0; i < 8; i++)
    {
        CLK_7219 = 0;
        DIN_7219 = (bit)(dat & 0x80);
        dat <<= 1;
        CLK_7219 = 1; // 时钟上升沿，发送数据
    }
    LOAD_7219 = 1; // 发送结束，上升沿锁存数据
}

// MAX7219初始化，设置MAX7219内部的控制寄存器
void Init7219(void)
{
    Write7219(SHUT_DOWN, 0x01);    // 开启正常工作模式（0xX1）
    Write7219(DISPLAY_TEST, 0x00); // 选择工作模式（0xX0）
    Write7219(DECODE_MODE, 0xff);  // 选用全译码模式
    Write7219(SCAN_LIMIT, 0x07);   // 8只LED全用
    Write7219(INTENSITY, 0x04);    // 设置初始亮度
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

    Write7219(ONES_BIT[dir], ones);
    Write7219(TENS_BIT[dir], tens);

    if (val == DONT_DISPLAY)
        last_display_val[dir] = last_display_val[dir]; // 不保存空白位置
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
    direction dir;
    if (new_second_flag)
    {
        for (dir = 0; dir < DIRECTION_MAX; dir++)
        {
            remain_time[dir]--;       // 各方向倒计时更新
            if (remain_time[dir] < 5) // 判断是否需要闪烁
            {
                if (current_color[dir] == YELLOW)
                    ToggleLedBit(GetDstLed(dir, YELLOW));
            }
            if (remain_time[dir] <= 0)
            {
                // 装载下一个状态
                next_status = (current_color[dir] + 1 >= TRAFFIC_LIGHT_STATUS_MAX) ? (RED) : (current_color[dir] + 1);
                // remain_time[dir] = GetReloadValue(next_status, dir);
                remain_time[dir] = light_time[dir][next_status];
                SetLedColor(dir, next_status);
            }

            // 数码管显示剩余秒数
            DisplayDigit(remain_time[dir], dir);
        }
        new_second_flag = 0; // 清除标志位
    }
    else
        return;
}

void Setting()
{
    // EA = 0;
    direction dir = NORTH;
    uchar time_out_flag = 0;
    TrafficLightColor color = RED;
    for (dir = EAST; dir < DIRECTION_MAX; dir++)
        SetLedColor(dir, RED); // 全部置为红灯
    for (dir = EAST; dir < DIRECTION_MAX; dir++)
    {
        while (mode == SETTING)
        {
            if (new_second_flag)
            {
                ToggleSegs(dir);
                new_second_flag = 0;
            }
            if (UP_KEY == 0)
            {
            }
        }
    }
    ClockReset();       // 重置时钟
    TrafficLightInit(); // 重新初始化
    mode = RUNNING;
    // EA = 1;
}

void ClockReset()
{
    // @brief 重置定时器0以及毫秒、秒计数器
    TR0 = 0;    // 定时器0开始计时
    ET0 = 0;    // 关闭定时器0中断
    TL0 = 0x20; // 设置定时初始值
    TH0 = 0xD1; // 设置定时初始值
    milliseconds = 0;
    seconds = 0;
    ET0 = 1;
    TR0 = 1; // 定时器0开始计时
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

void SetLedColor(direction dir, TrafficLightColor color)
{
    /**
     * @brief 设置dir方向的灯色
     * @ref 参见TrafficLight枚举
     * @warning 在高频调用该函数时会使输出电平出现暂态，影响稳定性
     */
    SetLedBit(GetDstLed(dir, current_color[dir]), LED_OFF);
    SetLedBit(GetDstLed(dir, color), LED_ON);
    current_color[dir] = color;
}

uchar GetDstLed(direction dir, TrafficLightColor color)
{
    /**
     * @brief 获取dir方向color颜色的LED对应的LEDs枚举变量
     */
    uchar dst_led = 0;
    switch (dir)
    {
    case NORTH:
        dst_led = 0;
        break;
    case WEST:
        dst_led = 3;
        break;
    case SOUTH:
        dst_led = 6;
        break;
    case EAST:
        dst_led = 9;
        break;
    default:
        break;
    }
    switch (color)
    {
    case GREEN:
        break;
    case YELLOW:
        dst_led += 1;
        break;
    case RED:
        dst_led += 2;
        break;
    default:
        break;
    }
    return dst_led;
}
