#include "headfile.h"
//延时
void DelayMS(uint x)
{
    uchar i;
    while (x--)
        for (i = 0; i < 120; i++)
            ;
}