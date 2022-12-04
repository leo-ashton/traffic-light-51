#ifndef __PINS_H__
#define __PINS_H__

#include "headfile.h"

#define DIGS P2
sbit RED_EAST = P0 ^ 0; // 东西向灯
sbit YELLOW_EAST = P0 ^ 1;
sbit GREEN_EAST = P0 ^ 2;
sbit RED_B = P0 ^ 3; // 南北向灯
sbit YELLOW_B = P0 ^ 4;
sbit GREEN_B = P0 ^ 5;
sbit transfer_east = P1 ^ 4;
sbit transfer_south = P1 ^ 5;
sbit transfer_west = P1 ^ 6;
sbit transfer_north = P1 ^ 7;
sbit UP_KEY = P3 ^ 0;
sbit DOWN_KEY = P3 ^ 1;

#endif