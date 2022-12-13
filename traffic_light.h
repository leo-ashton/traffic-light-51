#ifndef __TRAFFIC_LIGHT_H__
#define __TRAFFIC_LIGHT_H__

void TrafficLightInit();
void TrafficLight();
void Setting();
void Init7219(void);
void Write7219(unsigned char address, unsigned char dat);
void DisplayDigit(uchar val, direction dir);
void DisplayDigitDemo();
void ClockReset();
void ToggleSegs(direction dir);
void SetLedColor(direction dir, TrafficLightColor color);
uchar GetReloadValue(direction dir, TrafficLightColor next_status);
uchar GetDstLed(direction dir, TrafficLightColor color);

#endif
