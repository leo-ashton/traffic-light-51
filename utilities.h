#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include "headfile.h"

#define HC595_CNT 2

void DelayMS(uint x);
void Hc595SendMultiByte(ushort dat);
ushort BinarySeries2ushort(uchar series[], uchar len);
void SetLedBit(LEDs led, bit status);
void ToggleLedBit(LEDs led);
bit Wait4Key(uint max_wait_time, bit key_pin, uchar *timeout_flag);

#endif
