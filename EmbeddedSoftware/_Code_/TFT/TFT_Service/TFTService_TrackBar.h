#include "stm32f4xx.h"
#include "TFT_TouchPanel.h"

#define TrackBar_TrackSize							4								

#define TrackBar_BorderColor						TFT_BLACK

#define TrackBar_ForeColor 							TFT_DGRAY
#define TrackBar_BackColor 							TFT_LGRAY
#define TrackBar_PathColor							(TFT_DGRAY + TFT_LGRAY) / 2 

#define TrackBar_Disable_ForeColor			TrackBar_ForeColor + 20
#define TrackBar_Disable_BackColor			TrackBar_BackColor + 20
#define TrackBar_Disable_PathColor			(TrackBar_Disable_ForeColor + TrackBar_Disable_BackColor) / 2 

typedef enum
{
	TFTService_TrackBar_Vertical, 
	TFTService_TrackBar_Horizoltal, 
}TFTService_TrackBarType;

typedef struct
{
	int16_t 						MinX;
	int16_t 						MaxX;
	int16_t 						MinY;
	int16_t 						MaxY;
	int16_t 						CurrentPos;
	int16_t 						LastPos;
	float 							MaxValue;
	float 							MinValue;
	float 							Value;
	float 							Step;
	uint8_t							IsDisable;
	TFTService_TrackBarType Type;
}TFTService_TrackBar;

uint8_t	TFTSevice_TrackBarEvent (TFTService_TrackBar *R, int16_t touchX, int16_t touchY, TouchFinger Touch);
void TFTSevice_TrackBarValueRender(TFTService_TrackBar *R, float Value);
void TFTSevice_TrackBarTouchRender(TFTService_TrackBar *R);
void TFTSevice_TrackBarRender(TFTService_TrackBar *R);
