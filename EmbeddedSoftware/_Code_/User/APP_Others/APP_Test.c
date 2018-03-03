#include "App_Test.h"
#include "TFTService_Text.h"
#include "TFTService_Draw.h"
#include "TFTService_Button.h"
#include "TFT_SSD1289.h"
#include "Input.h"
#include "Voltage_FB.h"

void Test_Window (void)
{	
	TouchFinger event; 
	int16_t  down=0, up=0;
	int16_t	JLX=0, JLY=0, JRX=0, JRY=0;
	int16_t touchX=0, touchY=0; 
  TFT_Clear(TFT_BLACK);
  while(!ButtonA_Joystick() && !ButtonB_Joystick())
		{			
			event = TouchPanel_GetPoint(&touchX,&touchY);
			TFT_Text_Background(0,0, ASCII_08x16,  TFT_WHITE, TFT_BLACK,"x: %3d\ny: %3d", touchX, touchY);	
			switch (event)
			{
				case TouchFinger_Down:	
																TFT_DrawRect(TFT_PURPLE, touchX-6, touchY-6, 12, 12);	
																TFT_Text_Background (100,0, ASCII_08x16,  TFT_PURPLE, TFT_BLACK, "Down: %5d", down++);		
																break;
				case TouchFinger_Up:		
																TFT_DrawRect(TFT_BLUE, touchX-3, touchY-3, 6, 6);
																TFT_Text_Background (100,16, ASCII_08x16,  TFT_BLUE, TFT_BLACK,  "Up  : %5d", up++);		
																break;			
				case TouchFinger_Hold:	
																TFT_SetPixelSafe (touchX, touchY, TFT_OLIVE) ;	
																TFT_Text_Background (230,0, ASCII_08x16,  TFT_OLIVE, TFT_BLACK, "Hold");		
																break;
				case TouchFinger_None:	
																TFT_SetPixelSafe (TFT_GREEN, touchY, TFT_OLIVE) ;	
																TFT_Text_Background (230,0, ASCII_08x16,  TFT_GREEN, TFT_BLACK, "None");		
																break;
			}	
			// volt fb
			TFT_Text_Background (230,16, ASCII_08x16,		TFT_RED, TFT_BLACK, "VDD: %2.1f V", Get_Voltage());	
			
			//joystick value
			TFT_Text_Background (0,48, 	ASCII_08x16,	TFT_CYAN, TFT_BLACK, 		"Lx: %4d", 	JLX = Joystick_AX());		
			TFT_Text_Background (80,48, 	ASCII_08x16,	TFT_CYAN, TFT_BLACK, 	"Ly: %4d", JLY = Joystick_AY());	
			TFT_Text_Background (160,48,	ASCII_08x16,	TFT_CYAN, TFT_BLACK, 	"Rx: %4d", 	JRX = Joystick_BX());	
			TFT_Text_Background (240,48, 	ASCII_08x16,	TFT_CYAN, TFT_BLACK, 	"Ry: %4d", JRY = Joystick_BY());		
			
			//joystick button
			TFT_DrawFullRect(TFT_BLACK, JLPOSX, JPOSY, J_SIZE,J_SIZE);	
			TFT_DrawFullRect(TFT_BLACK, JRPOSX, JPOSY, J_SIZE,J_SIZE);	
			
			//joystick analog
			TFT_DrawFullCircle(TFT_RED, JLMOUSESPOSX+JLX*J_MOUSESCALE, JMOUSESPOSY-JLY*J_MOUSESCALE, J_MOUSESIZE);	
			TFT_DrawFullCircle(TFT_RED, JRMOUSESPOSX+JRX*J_MOUSESCALE, JMOUSESPOSY-JRY*J_MOUSESCALE, J_MOUSESIZE);	
			
			if (ButtonA_Left()) 			TFT_DrawFullCircle(TFT_BLUE, 	ButtonAPOSX-ButtonSIZE*2,	ButtonPOSY,							ButtonSIZE);
												else		TFT_DrawFullCircle(TFT_BLACK,	ButtonAPOSX-ButtonSIZE*2,	ButtonPOSY,							ButtonSIZE);
			if (ButtonA_Right()) 			TFT_DrawFullCircle(TFT_RED, 	ButtonAPOSX+ButtonSIZE*2,	ButtonPOSY,							ButtonSIZE);
												else		TFT_DrawFullCircle(TFT_BLACK,	ButtonAPOSX+ButtonSIZE*2,	ButtonPOSY,							ButtonSIZE);
			if (ButtonA_Up()) 				TFT_DrawFullCircle(TFT_GREEN, ButtonAPOSX,							ButtonPOSY-ButtonSIZE*2,ButtonSIZE);
												else		TFT_DrawFullCircle(TFT_BLACK, ButtonAPOSX,							ButtonPOSY-ButtonSIZE*2,ButtonSIZE);
			if (ButtonA_Down()) 			TFT_DrawFullCircle(TFT_YELLOW,ButtonAPOSX,							ButtonPOSY+ButtonSIZE*2,ButtonSIZE);
												else		TFT_DrawFullCircle(TFT_BLACK,	ButtonAPOSX,							ButtonPOSY+ButtonSIZE*2,ButtonSIZE);

			if (ButtonB_Left()) 			TFT_DrawFullCircle(TFT_BLUE, 	ButtonBPOSX-ButtonSIZE*2,	ButtonPOSY,							ButtonSIZE);
												else		TFT_DrawFullCircle(TFT_BLACK,	ButtonBPOSX-ButtonSIZE*2,	ButtonPOSY,							ButtonSIZE);
			if (ButtonB_Right()) 			TFT_DrawFullCircle(TFT_RED, 	ButtonBPOSX+ButtonSIZE*2,	ButtonPOSY,							ButtonSIZE);
												else		TFT_DrawFullCircle(TFT_BLACK,	ButtonBPOSX+ButtonSIZE*2,	ButtonPOSY,							ButtonSIZE);
			if (ButtonB_Up()) 				TFT_DrawFullCircle(TFT_GREEN, ButtonBPOSX,							ButtonPOSY-ButtonSIZE*2,ButtonSIZE);
												else		TFT_DrawFullCircle(TFT_BLACK, ButtonBPOSX,							ButtonPOSY-ButtonSIZE*2,ButtonSIZE);
			if (ButtonB_Down()) 			TFT_DrawFullCircle(TFT_YELLOW,ButtonBPOSX,							ButtonPOSY+ButtonSIZE*2,ButtonSIZE);
												else		TFT_DrawFullCircle(TFT_BLACK,	ButtonBPOSX,							ButtonPOSY+ButtonSIZE*2,ButtonSIZE);
  }
}
