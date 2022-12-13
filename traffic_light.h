#ifndef __TRAFFIC_LIGHT_H__
#define __TRAFFIC_LIGHT_H__

typedef enum direction
{
    EAST,
    SOUTH,
    WEST,
    NORTH
} direction;

typedef struct seg
{
    uchar on;
    uchar ones;
    uchar tens;
} seg;

void TrafficLightInit();
void TrafficLight();
void Setting();
void Init7219(void);
void Write7219(unsigned char address, unsigned char dat);
void DisplayDigit(uchar val, direction dir);
void DisplayDigitDemo();
void ClockReset();
void ToggleSegs(direction dir);
void DisplayLight(uchar light_status[]);

#endif