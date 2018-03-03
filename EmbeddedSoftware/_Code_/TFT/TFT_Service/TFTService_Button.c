#include "TFTService_Button.h"
#include "TFTService_Text.h"
#include "TFT_SSD1289.h"
#include "TFTService_Draw.h"

uint8_t	TFTSevice_RegionEvent (TFTService_Button *R, int16_t touchX, int16_t touchY, TouchFinger Touch)
{
	if (R->IsDisable) return 0;
	else
		switch (Touch)
		{
			case TouchFinger_None : return 0;
			case TouchFinger_Up	:
									if (R->IsPressed != 0)
									{
										R->IsPressed = 0;
										return 1;
									}		
									return 0;
			default: 					
									if (((touchX >= R->MinX)&&(touchX <= R->MaxX)&&(touchY >= R->MinY)&&(touchY <= R->MaxY)) != R->IsPressed)			
									{
										R->IsPressed = !R->IsPressed ;
										return 1;
									}
									return 0;
		}
}

void TFTSevice_Button_16x24(TFTService_Button *R)
{		
	if (R->IsDisable) 
	{		
		TFT_Text_Background	(R->MinX, 	R->MinY, 		ASCII_16x24,	R->Disable.ForeColor, R->Disable.BackColor,"%s",R->Disable.Text);
	}
	else if (R->IsPressed) 
	{		
		TFT_Text_Background	(R->MinX, 	R->MinY, 		ASCII_16x24,	R->Selected.ForeColor, R->Selected.BackColor,"%s",R->Selected.Text);
		TFT_Text_Transparent(R->MinX, 	R->MinY+1, 	ASCII_16x24,	R->Selected.ForeColor,"%s",R->Selected.Text);
		TFT_Text_Transparent(R->MinX-1, R->MinY, 		ASCII_16x24,	R->Selected.ForeColor,"%s",R->Selected.Text);
		TFT_Text_Transparent(R->MinX+1, R->MinY, 		ASCII_16x24,	R->Selected.ForeColor,"%s",R->Selected.Text);
	}
	else 
	{		
		TFT_Text_Background	(R->MinX, 	R->MinY, ASCII_16x24,	R->Normal.ForeColor, R->Normal.BackColor,"%s",R->Normal.Text);
		TFT_Text_Transparent(R->MinX-1, R->MinY, ASCII_16x24,	R->Normal.ForeColor,"%s",R->Normal.Text);
		TFT_Text_Transparent(R->MinX+1, R->MinY, ASCII_16x24,	R->Normal.ForeColor,"%s",R->Normal.Text);
	}	
//	TFT_DrawFullRect (TFT_WHITE, R->MinX, R->MinY, R->MaxX - R->MinX, R->MaxY - R->MinY);
}


void TFTSevice_Button_08x16(TFTService_Button *R)
{		
	if (R->IsDisable) 
	{		
		TFT_Text_Background	(R->MinX, 	R->MinY, 		ASCII_08x16,	R->Disable.ForeColor, R->Disable.BackColor,"%s",R->Disable.Text);
	}	
	else if (R->IsPressed) 
	{		
		TFT_Text_Background	(R->MinX, 	R->MinY, 		ASCII_08x16,	R->Selected.ForeColor, R->Selected.BackColor,"%s",R->Selected.Text);
		TFT_Text_Transparent(R->MinX+1, R->MinY, 		ASCII_08x16,	R->Selected.ForeColor,"%s",R->Selected.Text);
	}
	else 
	{		
		TFT_Text_Background	(R->MinX, 	R->MinY, ASCII_08x16,	R->Normal.ForeColor, R->Normal.BackColor,"%s",R->Normal.Text);
	}	
}
