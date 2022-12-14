#include "headfile.h"

static uchar led_status[TRAFFIC_LIGHT_MAX];

void DelayMS(uint x)
{
    uchar i;
    while (x--)
        for (i = 0; i < 120; i++)
            ;
}

bit Wait4Key(uint max_wait_time, bit key_pin, uchar *timeout_flag)
{
    /**
     * @brief 等待按键输入，等待wait_time后如无输入则返回
     * @param max_wait_time 最大返回时间
     * @param key_pin 按键所在的引脚
     * @return 如在wait_time期间按键电平发生变化，返回消抖后的按键信息；否则返回初始时候
     */
    uchar i;
    bit in_status = key_pin; // 进入程序时的引脚状态
    bit ret = in_status;
    while (max_wait_time--)
    {
        for (i = 0; i < 120; i++)
            if (key_pin == 0)
            {
                DelayMS(50); // 消抖延时
                if (key_pin != in_status)
                {
                    *timeout_flag = 0;
                    return (bit)key_pin; // 如果按键电平发生了变化，则直接返回
                }
            }
    }
    *timeout_flag = 1;
}

void Hc595SendMultiByte(unsigned short dat)
{
    /**
     * @brief 向HC595发送数据，并并行输出；dat的高8位在第二块HC595（东边的三个灯所在的那片），低8位在第一块HC595
     */
    uchar a;

    SRCLK = 1;
    RCLK = 1;

    for (a = 0; a < sizeof(dat) * 8; a++) // 发送8位数
    {
        SER = dat >> ((sizeof(dat) * 8) - 1); // 从最高位开始发送
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

unsigned short BinarySeries2ushort(uchar series[], uchar len)
{
    /**
     * @brief 将二进制序列数组转换为对应的十进制数
     * @param series 高位在前的二进制序列
     * @return 转换的十进制数
     */
    // TODO implement this
    static ushort ret;
    static uchar i;
    for (i = 0, ret = 0; i < len; i++)
    {
        ret += (series[i] << i);
    }

    return ret;
}

void SetLedBit(LEDs led, bit status)
{
    /**
     * @brief 设置某位LED的状态。这是唯一允许的操作LED的方式。
     * @param led 枚举类型变量，操作的LED位
     */
    led_status[led] = status;
    Hc595SendMultiByte(
        BinarySeries2ushort(led_status,
                            sizeof(led_status) / sizeof(led_status[0])));
}

void ToggleLedBit(LEDs led)
{
    /**
     * @brief 翻转led
     * @warning 注意输入变量是否为LEDs枚举变量
     */
    led_status[led] = !led_status[led];
    Hc595SendMultiByte(
        BinarySeries2ushort(led_status,
                            sizeof(led_status) / sizeof(led_status[0])));
}
