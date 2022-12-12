#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include "headfile.h"
void DelayMS(uint x);
void Delay100us();
void Hc595SendByte(uchar dat);
uint BinarySeries2Uint(uchar series[], uchar len);

#endif