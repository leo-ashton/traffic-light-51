#include "headfile.h"

static uchar led_status[TRAFFIC_LIGHT_MAX];

void DelayMS(uint x)
{
    uchar i;
    while (x--)
        for (i = 0; i < 120; i++)
            ;
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
    // last_led_status[led] = status;
}

void ToggleLedBit(LEDs led)
{
    led_status[led] = !led_status[led];
    Hc595SendMultiByte(
        BinarySeries2ushort(led_status,
                            sizeof(led_status) / sizeof(led_status[0])));
    // last_led_status[led] = led_status[led];
}