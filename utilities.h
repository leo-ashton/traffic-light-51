#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include "headfile.h"

#define HC595_CNT 2

void DelayMS(uint x);
void Hc595SendMultiByte(unsigned short dat);
ushort BinarySeries2ushort(uchar series[], uchar len);
void SetLedBit(LEDs led, bit status);

#endif