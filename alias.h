#ifndef __ALIAS_H__
#define __ALIAS_H__

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;

typedef enum func
{
    DEBUGGING,
    RUNNING,
    SETTING,
    FUNC_MAX
} func;

typedef enum TrafficLight
{
    NORTH_GREEN,
    NORTH_YELLOW,
    NORTH_RED,
    WEST_GREEN,
    WEST_YELLOW,
    WEST_RED,
    SOUTH_GREEN,
    SOUTH_YELLOW,
    SOUTH_RED,
    EAST_GREEN,
    EAST_YELLOW,
    EAST_RED,
    TRAFFIC_LIGHT_MAX
} LEDs;

typedef enum LED_status
{
    LED_ON,
    LED_OFF
}

#define DECODE_MODE 0x09  // 译码控制寄存器
#define INTENSITY 0x0A    // 亮度控制寄存器
#define SCAN_LIMIT 0x0B   // 扫描界限寄存器
#define SHUT_DOWN 0x0C    // 关断模式寄存器
#define DISPLAY_TEST 0x0F // 测试控制寄存器

#endif