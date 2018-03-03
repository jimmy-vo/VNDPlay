#include "TFTService_TrackBar.h"
#include "TFTService_Text.h"
#include "TFT_SSD1289.h"
#include "TFTService_Draw.h"
#include <stdio.h>
#include <stdlib.h>

uint8_t	TFTSevice_TrackBarEvent (TFTService_TrackBar *R, int16_t touchX, int16_t touchY, TouchFinger Touch)
{	
	if (R->IsDisable) return 0;
	else
		switch (Touch)
		{
			case TouchFinger_None : 	return 0;
			case TouchFinger_Up		:		return 0;
			default: 					
					if ((touchX >= R->MinX)&&(touchX <= R->MaxX)&&(touchY >= R->MinY)&&(touchY <= R->MaxY))			
					{
						if (R->Type ==  TFTService_TrackBar_Vertical) 
						{
							if (touchY == R->LastPos) return 0;
							else
							{
								R->CurrentPos = touchY;
								if 			(R->CurrentPos >= R->MaxY - TrackBar_TrackSize/2)	
								{
									R->CurrentPos = R->MaxY - TrackBar_TrackSize/2;
									R->Value = R->MinValue;
								}
								else if (R->CurrentPos <= R->MinY + TrackBar_TrackSize/2)	
								{
									R->CurrentPos = R->MinY + TrackBar_TrackSize/2;
									R->Value = R->MaxValue;
								}
								else		
								{
									R->Value = (R->MaxValue - (R->MaxValue - R->MinValue) * (touchY - R->MinY - TrackBar_TrackSize/2)/ (R->MaxY - R->MinY - TrackBar_TrackSize) );	
									R->Value = R->MinValue + R->Step * ((int16_t)((R->Value - R->MinValue)/R->Step));		
								}	
							}
						}
						else 
						{
							if (touchX == R->LastPos) return 0;
							else
							{
								R->CurrentPos = touchX;
								if 			(R->CurrentPos >= R->MaxX - TrackBar_TrackSize/2)	
								{
									R->CurrentPos = R->MaxX - TrackBar_TrackSize/2;
									R->Value = R->MaxValue;
								}
								else if (R->CurrentPos <= R->MinX + TrackBar_TrackSize/2)
								{
									R->Value = R->MinValue;
									R->CurrentPos = R->MinX + TrackBar_TrackSize/2;
								}
								else			
								{
									R->Value = (R->MaxValue - R->MinValue) * (touchX - R->MinX - TrackBar_TrackSize/2)/ (R->MaxX - R->MinX - TrackBar_TrackSize) + R->MinValue;
									R->Value = R->MinValue + R->Step * ((int16_t)((R->Value - R->MinValue)/R->Step));	
								}						
							}
						}	
						TFTSevice_TrackBarTouchRender (R);
						return 1;
					}
			return 0;
		}
}

void TFTSevice_TrackBarValueRender(TFTService_TrackBar *R, float Value)
{		
	if 			(Value >= R->MaxValue ) R->Value = R->MaxValue;
	else if (Value <= R->MinValue ) R->Value = R->MinValue;
	else 		R->Value = Value;
	
	if (R->Type ==  TFTService_TrackBar_Vertical) 
		R->CurrentPos =  (R->MaxValue - Value) * (R->MaxY - R->MinY - TrackBar_TrackSize) / (R->MaxValue - R->MinValue) + R->MinY + TrackBar_TrackSize/2;
	else
		R->CurrentPos =  (Value - R->MinValue) * (R->MaxX - R->MinX - TrackBar_TrackSize) / (R->MaxValue - R->MinValue) + R->MinX + TrackBar_TrackSize/2;
	
	if (R->CurrentPos != R->LastPos)	
		TFTSevice_TrackBarTouchRender (R);
}

void TFTSevice_TrackBarTouchRender(TFTService_TrackBar *R)
{		
	if (R->Type == TFTService_TrackBar_Vertical) 
	{
		TFT_DrawFullRect (TrackBar_BackColor 	, R->MinX, R->LastPos - TrackBar_TrackSize/2 , R->MaxX - R->MinX , TrackBar_TrackSize);
		TFT_DrawFullRect (TrackBar_ForeColor 	, R->MinX, R->CurrentPos - TrackBar_TrackSize/2 , R->MaxX - R->MinX , TrackBar_TrackSize);
	}
	else
	{
		TFT_DrawFullRect (TrackBar_BackColor 	,  R->LastPos - TrackBar_TrackSize/2 , 		R->MinY, TrackBar_TrackSize, R->MaxY - R->MinY );
		TFT_DrawFullRect (TrackBar_ForeColor 	,  R->CurrentPos - TrackBar_TrackSize/2 , R->MinY, TrackBar_TrackSize, R->MaxY - R->MinY );	
	}
	R->LastPos = R->CurrentPos;
}

void TFTSevice_TrackBarRender(TFTService_TrackBar *R)
{
	TFT_DrawRect 			(TrackBar_BorderColor , R->MinX-1, 		R->MinY-1, R->MaxX - R->MinX + 2, R->MaxY - R->MinY + 2 );
	TFT_DrawFullRect 	(TrackBar_BackColor 	, R->MinX, 	R->MinY, R->MaxX - R->MinX, R->MaxY - R->MinY);
	
	if (R->Type ==  TFTService_TrackBar_Vertical) 
		R->LastPos =  R->MinY + TrackBar_TrackSize/2;
	else
		R->LastPos = 	R->MinX + TrackBar_TrackSize/2;
	
	TFTSevice_TrackBarValueRender(R, (R->MaxValue + R->MinValue)/2);
}
