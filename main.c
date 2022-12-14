#include "headfile.h"

void InterruptKey() interrupt 0
{
	EA = 0;
	if (mode < FUNC_MAX)
		mode++;
	else
		mode = RUNNING;
	EA = 1;
}

void InterruptTimer0() interrupt 1
{
	EA = 0;
	UpdateTimer0();
	EA = 1;
}

void InterruptEmergency() interrupt 2
{
	EA = 0;
	if (mode == EMERGENCY)
		mode = RUNNING;
	else
		mode = EMERGENCY;
	EA = 1;
}

// 主程序
void main()
{
	Timer0Init();
	LoadDefaultTime();
	TrafficLightInit();
	EA = 1;
	ET0 = 1;
	EX0 = 1; // 外部中断
	IT0 = 1; // 下降沿触发
	EX1 = 1; // 外部中断
	IT1 = 1; // 下降沿触发
	Init7219();

	DelayMS(500);
	while (1)
	{
		if (mode == RUNNING)
		{
			TrafficLightMain();
		}
		if (mode == SETTING)
		{
			SettingMain();
		}
		if (mode == DEBUGGING)
		{
			mode = RUNNING;
		}
		if (mode == EMERGENCY)
		{
			EmergencyMain();
		}
	}
}
