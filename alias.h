#ifndef __ALIAS_H__
#define __ALIAS_H__

typedef unsigned char uchar;
typedef unsigned int uint;

typedef enum func
{
    DEBUGGING,
    RUNNING,
    SETTING,
    FUNC_MAX
} func;

#define DECODE_MODE 0x09  //译码控制寄存器
#define INTENSITY 0x0A    //亮度控制寄存器
#define SCAN_LIMIT 0x0B   //扫描界限寄存器
#define SHUT_DOWN 0x0C    //关断模式寄存器
#define DISPLAY_TEST 0x0F //测试控制寄存器

#endif