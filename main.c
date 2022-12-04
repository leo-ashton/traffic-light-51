#include "headfile.h"
uchar digit_codes[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8,
					   0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e}; // 数码管段码
uchar Flash_Count = 0, Operation_Type = 1;								//闪烁次数，操作类型变量

uint i = 0;
uint pre_second = 0;

//延时
void DelayMS(uint x)
{
	uchar i;
	while (x--)
		for (i = 0; i < 120; i++)
			;
}

void InterruptKey() interrupt 0
{
	if (mode < FUNC_MAX)
	{
		mode++;
	}
	else
	{
		mode = RUNNING;
	}
}

void InterruptTimer0() interrupt 1
{
	UpdateTimer0();
}

//主程序
void main()
{
	Timer0Init();
	EA = 1;
	ET0 = 1;
	EX0 = 1; // 外部中断
	Init7219();
	while (1)
	{
		if (mode == RUNNING)
		{
			TrafficLight();
		}
		if (mode == SETTING)
		{
			Setting();
		}
		if (mode == DEBUGGING)
		{
			Write7219(1, 1);
			Write7219(2, 2);
			Write7219(3, 3);
			Write7219(4, 4);
			Write7219(5, 5);
			Write7219(6, 6);
			Write7219(7, 7);
			Write7219(8, 8);
		}
	}
}
