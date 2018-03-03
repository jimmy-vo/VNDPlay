#include "stm32f4xx.h"
#include "TFT_TouchPanel.h"

typedef struct
{
	uint16_t 			ForeColor;
	uint16_t 			BackColor;
	char 					*Text;  
}Button_Appearance;

typedef struct
{
	uint16_t 						MinX;
	uint16_t 						MaxX;
	uint16_t 						MinY;
	uint16_t 						MaxY;
	uint8_t							IsPressed;
	uint8_t							IsDisable;
	Button_Appearance		Normal;
	Button_Appearance		Selected;
	Button_Appearance		Disable;
}TFTService_Button;

uint8_t			TFTSevice_RegionEvent (TFTService_Button *R, int16_t touchX, int16_t touchY, TouchFinger State);
void TFTSevice_Button_16x24(TFTService_Button *R);
void TFTSevice_Button_08x16(TFTService_Button *R);
