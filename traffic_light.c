#include "headfile.h"

#define DONT_DISPLAY 255 // DisplayDigit函数的val为该值时，两位均不显示
#define SEG_OFF 15       // Write7219向某位数码管写入该值时，该位数码管不显示

static uchar last_display_val[DIRECTION_MAX];
digs_addr code ONES_BIT[] = {EAST_ONES, SOUTH_ONES, WEST_ONES, NORTH_ONES};
digs_addr code TENS_BIT[] = {EAST_TENS, SOUTH_TENS, WEST_TENS, NORTH_TENS};
LEDs code LED_MAP[DIRECTION_MAX][3] = {{EAST_RED, EAST_GREEN, EAST_YELLOW},
                                       {SOUTH_RED, SOUTH_GREEN, SOUTH_YELLOW},
                                       {WEST_RED, WEST_GREEN, WEST_YELLOW},
                                       {NORTH_RED, NORTH_GREEN, NORTH_YELLOW}};
uchar remain_time[DIRECTION_MAX];
uchar light_time[DIRECTION_MAX][3]; // 存放四个方向的三色灯的设置时间,第一维度为dir,第二维度为color
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

    remain_time[EAST] = light_time[EAST][GREEN];
    remain_time[SOUTH] = remain_time[EAST] + light_time[EAST][YELLOW];
    remain_time[WEST] = remain_time[SOUTH] + light_time[SOUTH][GREEN] + light_time[SOUTH][YELLOW];
    remain_time[NORTH] = remain_time[WEST] + light_time[WEST][GREEN] + light_time[WEST][YELLOW];

    // remain_time[EAST] = 15;
    // remain_time[SOUTH] = 20;
    // remain_time[WEST] = 40;
    // remain_time[NORTH] = 60;

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

void TrafficLightMain()
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
                    ToggleLedBit(LED_MAP[dir][YELLOW]);
                // ToggleLedBit(GetDstLed(dir, YELLOW));
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

void SettingMain()
{
    direction dir = EAST;
    uchar time_out_flag = 0;
    TrafficLightColor color = RED;

    for (dir = EAST; dir < DIRECTION_MAX; dir++)
    {
        SetLedColor(dir, RED); // 全部置为红灯
    }

    while (mode == SETTING)
    {
        for (dir = EAST; dir < DIRECTION_MAX && mode == SETTING; dir++)
        {
            for (color = RED; color < TRAFFIC_LIGHT_STATUS_MAX && mode == SETTING; color++)
            {
                while (mode == SETTING)
                {
                    if (SWITCH_KEY == 0)
                    {
                        DelayMS(100);
                        if (SWITCH_KEY == 0)
                            break;
                    }
                    if (new_second_flag_setting)
                    {
                        DisplayDigit(light_time[dir][color], dir);
                        ToggleSegs(dir);
                        new_second_flag_setting = 0;
                    }
                    if (UP_KEY == 0)
                    {
                        DelayMS(100);
                        if (UP_KEY == 0)
                            light_time[dir][color]++;
                    }
                    if (DOWN_KEY == 0)
                    {
                        DelayMS(100);
                        if (DOWN_KEY == 0)
                            light_time[dir][color]--;
                    }
                }
            }
            DisplayDigit(light_time[dir][RED], dir);
        }
    }
    ClockReset();       // 重置时钟
    TrafficLightInit(); // 重新初始化
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
    SetLedBit(LED_MAP[dir][current_color[dir]], LED_OFF);
    SetLedBit(LED_MAP[dir][color], LED_ON);
    // SetLedBit(GetDstLed(dir, current_color[dir]), LED_OFF);
    // SetLedBit(GetDstLed(dir, color), LED_ON);
    current_color[dir] = color;
}

void EmergencyMain()
{
    direction dir;
    new_second_flag_emergency = 0;
    for (dir = EAST; dir < DIRECTION_MAX; dir++)
        SetLedColor(dir, YELLOW);
    while (mode == EMERGENCY)
    {
        if (new_second_flag_emergency)
        {
            for (dir = EAST; dir < DIRECTION_MAX; dir++)
            {
                DisplayDigit(DONT_DISPLAY, dir);
                ToggleLedBit(LED_MAP[dir][YELLOW]);
            }
            new_second_flag_emergency = 0;
        }
    }
    TrafficLightInit(); // 恢复正常运行模式
}

void LoadDefaultTime()
{
    // 加载初始状态各路口时间
    light_time[EAST][RED] = 60, light_time[EAST][YELLOW] = 5, light_time[EAST][GREEN] = 15;
    light_time[SOUTH][RED] = 60, light_time[SOUTH][YELLOW] = 5, light_time[SOUTH][GREEN] = 15;
    light_time[WEST][RED] = 60, light_time[WEST][YELLOW] = 5, light_time[WEST][GREEN] = 15;
    light_time[NORTH][RED] = 60, light_time[NORTH][YELLOW] = 5, light_time[NORTH][GREEN] = 15;
}