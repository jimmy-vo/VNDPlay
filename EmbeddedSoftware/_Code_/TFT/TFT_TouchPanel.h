#include "stm32f4xx.h"
#ifndef __TOUCHPANNNEL_H
#define __TOUCHPANNNEL_H


void 		XPT2046_Initialize(void);
void 		TouchPanel_Calibrate(void);
void 		TouchPanel_Test (void);

typedef enum
{
	TouchFinger_None = 0,
	TouchFinger_Down = 1,
	TouchFinger_Up   = 3,
	TouchFinger_Hold = 2,
}TouchFinger;


TouchFinger TouchPanel_GetPoint (int16_t *x,int16_t *y);
TouchFinger TouchPanel_CheckEvent (void);
void 				TouchPanel_Initialize(void);

#endif
