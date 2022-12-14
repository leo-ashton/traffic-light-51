#include "headfile.h"

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

// 主程序
void main()
{
	Timer0Init();
	TrafficLightInit();
	EA = 1;
	ET0 = 1;
	EX0 = 1; // 外部中断
	Init7219();

	// DisplayDigit(1, EAST);
	// DisplayDigit(10, SOUTH);
	// DisplayDigit(50, WEST);
	// DisplayDigit(99, NORTH);

	// SetLedColor(SOUTH, GREEN);
	// SetLedColor(NORTH, RED);
	// SetLedColor(WEST, RED);
	// SetLedColor(EAST, YELLOW);

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
			TrafficLight();
		}
	}
}
