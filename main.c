#include "headfile.h"
uchar digit_codes[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8,
					   0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e}; // 数码管段码
uchar Flash_Count = 0, Operation_Type = 1;								//闪烁次数，操作类型变量

uint i = 0;
uint pre_second = 0;
typedef enum func
{
	RUNNING,
	SETTING
} func;

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
}

void InterruptTimer0() interrupt 1
{
	UpdateTimer0();
}

void Traffic_Light()
{
	//交通灯切换
	switch (Operation_Type)
	{
	case 1: //东西向绿灯与南北向红灯亮
		RED_EAST = 1;
		YELLOW_EAST = 1;
		GREEN_EAST = 0;
		RED_B = 0;
		YELLOW_B = 1;
		GREEN_B = 1;
		DelayMS(2000);
		Operation_Type = 2;
		break;
	case 2: //东西向黄灯闪烁，绿灯关闭
		DelayMS(300);
		YELLOW_EAST = ~YELLOW_EAST;
		GREEN_EAST = 1;
		if (++Flash_Count != 10)
			return; //闪烁5次
		Flash_Count = 0;
		Operation_Type = 3;
		break;
	case 3: //东西向红灯，南北向绿灯亮
		RED_EAST = 1;
		GREEN_B = 0;
		DelayMS(2000);
		Operation_Type = 4;
		break;

	case 4: //南北向黄灯闪烁5 次
		DelayMS(300);
		YELLOW_B = ~YELLOW_B;
		GREEN_B = 1;
		if (++Flash_Count != 10)
			Operation_Type = 1;
		break;
	}
}
//主程序
void main()
{
	Timer0Init();
	EA = 1;
	ET0 = 1;
	EX0 = 1; // 外部中断
	DIGS = 1;
	while (1)
	{

	}
}
