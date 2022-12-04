#ifndef __TRAFFIC_LIGHT_H__
#define __TRAFFIC_LIGHT_H__
typedef enum
{
    EAST,
    SOUTH,
    WEST,
    NORTH
} direction;
void TrafficLight();
void Setting();
void Init7219(void);
void Write7219(unsigned char address, unsigned char dat);
void DisplayDigit(uchar val, direction dir);

#endif