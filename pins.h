#ifndef __PINS_H__
#define __PINS_H__

#include "headfile.h"

sbit DIN_7219 = P2 ^ 0;
sbit LOAD_7219 = P2 ^ 1;
sbit CLK_7219 = P2 ^ 2;
sbit UP_KEY = P3 ^ 0;
sbit DOWN_KEY = P3 ^ 1;

#endif