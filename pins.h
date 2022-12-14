#ifndef __PINS_H__
#define __PINS_H__

#include "headfile.h"

sbit DIN_7219 = P2 ^ 0;
sbit LOAD_7219 = P2 ^ 1;
sbit CLK_7219 = P2 ^ 2;
sbit UP_KEY = P1 ^ 0;
sbit DOWN_KEY = P1 ^ 1;
sbit MODE_KEY = P3 ^ 2;
sbit EMERGENCY_KEY = P3 ^ 3;
sbit SWITCH_KEY = P1 ^ 2;

sbit SRCLK = P0 ^ 1;
sbit RCLK = P0 ^ 2;
sbit SER = P0 ^ 0;

#endif