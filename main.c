#include "headfile.h"
uchar digit_codes[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8,
					   0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e}; // 数码管段码
uchar Flash_Count = 0, Operation_Type = 1;								//闪烁次数，操作类型变量

uint i = 0;
uint pre_second = 0;

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
	DisplayDigit(1, EAST);
	DisplayDigit(10, SOUTH);
	DisplayDigit(50, WEST);
	DisplayDigit(99, NORTH);
	DelayMS(500);
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
			// DisplayDigitDemo();
			ToggleSegs(NORTH);
			ToggleSegs(SOUTH);
			ToggleSegs(EAST);
			ToggleSegs(WEST);
			DelayMS(1000);
		}
	}
}
