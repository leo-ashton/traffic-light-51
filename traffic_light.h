#ifndef __TRAFFIC_LIGHT_H__
#define __TRAFFIC_LIGHT_H__

void TrafficLightInit();
void Init7219(void);
void Write7219(unsigned char address, unsigned char dat);
void DisplayDigit(uchar val, direction dir);
void ClockReset();
void ToggleSegs(direction dir);
void SetLedColor(direction dir, TrafficLightColor color);
void TrafficLightMain();
void EmergencyMain();
void SettingMain();
void LoadDefaultTime();

#endif
