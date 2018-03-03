/* Includes ------------------------------------------------------------------*/
#include "TFT_TouchPanel.h"
#include "TFT_XPT2046.h"
#include "TFT_SSD1289.h"
#include "TFTService_Text.h"
#include "TFTService_Draw.h"
#include "RGB.h"
#include "Voltage_FB.h"

#define THRESHOLD 2
static TouchFinger TouchPanel_LastEvent;

void TouchPanel_Initialize(void)
{
	XPT2046_Initialize();
}


int16_t LastPointX, LastPointY;
void TouchPanel_Filter (int16_t *x, int16_t *y)
{
	int32_t avrX, avrY;
	if (Read_XPT2046 (&avrX, &avrY))
	{				
		if (TFT_Width<TFT_Height) //Portrait
		{
			*x = (int16_t) ((avrY - 2100)*0.0671f + TFT_Width/2);
			*y = (int16_t) ((-avrX + 2160)*0.0881f + TFT_Height/2);
		}
		else
			{
				*x = (int16_t) ((avrX - 2160)*0.0881f + TFT_Width/2);
				*y = (int16_t) ((avrY - 2100)*0.0671f + TFT_Height/2);
			}
			
		if ((*x > TFT_Width)||(*x < 0)) 	*x = LastPointX;
		
		if ((*y > TFT_Height)||(*y < 0)) 	*y = LastPointY;	
	}
	else
	{
		*x = LastPointX;
		*y = LastPointY;	
	}
}


TouchFinger TouchPanel_CheckEvent (void)
{
	if(XPT2046_IRQRead())	
	{
		switch (TouchPanel_LastEvent)
		{
			case TouchFinger_None:	RGB_Flash(RGB_WHITE);		
															return (TouchPanel_LastEvent = TouchFinger_Down);
			case TouchFinger_Down:	return (TouchPanel_LastEvent = TouchFinger_Hold);
			case TouchFinger_Hold:	return (TouchPanel_LastEvent = TouchFinger_Hold);
			case TouchFinger_Up:		return (TouchPanel_LastEvent = TouchFinger_Down);													
		}		
	}
	else
	{
		switch (TouchPanel_LastEvent)
		{
			case TouchFinger_Hold:	RGB_Flash(RGB_WHITE);
															return (TouchPanel_LastEvent = TouchFinger_Up);
			case TouchFinger_None:	return (TouchPanel_LastEvent = TouchFinger_None);
			case TouchFinger_Down:	return (TouchPanel_LastEvent = TouchFinger_Up);
			case TouchFinger_Up:		return (TouchPanel_LastEvent = TouchFinger_None);													
		}
	}
	return TouchFinger_Up;
}

TouchFinger TouchPanel_GetPoint (int16_t *x, int16_t *y)
{
	TouchFinger state =  TouchPanel_CheckEvent();
	switch (state)
	{
		case TouchFinger_Down:				
					TouchPanel_Filter(x, y);					
					if ((*x == LastPointX) && (*y == LastPointY))
						return (TouchPanel_LastEvent = TouchFinger_None);								
					break;						
		case TouchFinger_Hold:				
					TouchPanel_Filter(x, y);		
					break;
		default:	
					break;
	}		
	LastPointX = *x;
	LastPointY = *y;
	return state;						
}

