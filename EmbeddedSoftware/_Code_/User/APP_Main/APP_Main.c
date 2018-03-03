#include "APP_Main.h"
#include "TFTService_Text.h"
#include "TFTService_Draw.h"
#include "TFTService_Button.h"
#include "TFT_SSD1289.h"
#include "TFTService_Dialog.h"

#include "Input.h"
#include "Voltage_FB.h"
#include "Bluetooth.h"

#include "Sudoku.h"    
#include "App_Test.h"
#include "APP_RobotArm.h"
#include "APP_Shellmo.h"
#include "APP_IMU.h"
#include "APP_Snake.h"

#define MAIN_GameNum			 	2
#define MAIN_RemoteNum			2
#define MAIN_SettingNum			3

typedef enum 
{
	MAIN_Min					= 0 ,
	MAIN_Snake				= 0 ,
	MAIN_Sudoku				= 1 ,
	
	MAIN_Control			= 1 ,
	MAIN_Shellmo 			= 2 ,
	MAIN_RobotArm 		= 3 ,
	
	MAIN_Setting	 		= 3 ,
	MAIN_Bluetooth 		= 4 ,
	MAIN_Tilt			 		= 5 ,
	MAIN_Test 				= 6 ,
	
	MAIN_Max					= 7 ,
}MAIN_BUTTON;

static char *MAIN_String[MAIN_Max] = 
{	
	"  Snake    ",
	"  Sudoku   ",	
	" Shellmo   ",
	" RobotArm  ",
	" Bluetooth ", 
	"    IMU    ",	
	"   Test    "
};

static TFTService_Button Main_Button[MAIN_Max];

uint8_t Main_ButtonEvent(MAIN_BUTTON cnt)
{
	switch (cnt)
	{		
		case MAIN_Snake				:		Snake_Window(); return 1;
		case MAIN_Sudoku			:		Sudoku_Window(); return 1;
		case MAIN_Shellmo 		:		Shellmo_Window(); return 1;
		case MAIN_RobotArm 		:		RobotArm_Window(); return 1;
		case MAIN_Bluetooth 	:		Bluetooth_Window(); return 1;
		case MAIN_Tilt			 	:		IMU_Window(); return 1;
		case MAIN_Test 				:		Test_Window(); return 1;
		default								:		return 0;
	}
}

void Main_Render (void)
{		
	uint8_t cnt;
	
  TFT_Clear(Main_Background);
	
	//Draw titile	
	TFT_DrawFullRect (Main_Theme_Title, Main_TITLE_POSX, Main_TITLE_POSY, Main_TITLE_SIZEX, Main_TITLE_SIZEY);
	
	TFT_Text_Transparent(94, 10, ASCII_16x24,	TFT_BLUE, "VND Play");
	TFT_Text_Transparent(95, 10, ASCII_16x24,	TFT_BLUE, "VND Play");
	TFT_Text_Transparent(96, 10, ASCII_16x24,	TFT_BLUE, "VND Play");
	TFT_Text_Transparent(294, 3, ASCII_08x12,	TFT_WHITE, "VND");	
	TFT_Text_Transparent(295, 3, ASCII_08x12,	TFT_WHITE, "VND");			
	
	//Draw game region	
	TFT_DrawFullRect(
										Main_Theme_Button, 
										Main_Button[MAIN_Snake].MinX - 2 ,
										Main_Button[MAIN_Snake].MinY - Main_FONTSize_Button_Y - Main_Separate_Button - 2,
										Main_ButtonSIZE  + 4, 
										Main_FONTSize_Button_Y * (MAIN_GameNum+1) + Main_Separate_Button * MAIN_GameNum + 4
									);
	TFT_Text_Transparent(
												Main_Button[MAIN_Snake].MinX,
												Main_Button[MAIN_Snake].MinY - Main_FONTSize_Button_Y - Main_Separate_Button/2,
												ASCII_08x16,	Main_Label_Button, "    GAME"
											);	
		
	
	//Draw Remote region	
	TFT_DrawFullRect(
										Main_Theme_Button, 
										Main_Button[MAIN_Shellmo].MinX - 2 ,
										Main_Button[MAIN_Shellmo].MinY - Main_FONTSize_Button_Y - Main_Separate_Button- 2,
										Main_ButtonSIZE  + 4, 
										Main_FONTSize_Button_Y * (MAIN_RemoteNum+1) + Main_Separate_Button * MAIN_RemoteNum + 4
									);
	TFT_Text_Transparent(
												Main_Button[MAIN_Shellmo].MinX,
												Main_Button[MAIN_Shellmo].MinY - Main_FONTSize_Button_Y - Main_Separate_Button/2,
												ASCII_08x16,	Main_Label_Button, "  REMOTE"
											);	
	
	//Draw Setting region	
	TFT_DrawFullRect(
										Main_Theme_Button, 
										Main_Button[MAIN_Bluetooth].MinX - 2 ,
										Main_Button[MAIN_Bluetooth].MinY - Main_FONTSize_Button_Y - Main_Separate_Button - 2,
										Main_ButtonSIZE  + 4, 
										Main_FONTSize_Button_Y * (MAIN_SettingNum+1) + Main_Separate_Button * MAIN_SettingNum + 4
									);
	TFT_Text_Transparent(
												Main_Button[MAIN_Bluetooth].MinX,
												Main_Button[MAIN_Bluetooth].MinY - Main_FONTSize_Button_Y - Main_Separate_Button/2,
												ASCII_08x16,	Main_Label_Button, "  SETTING"
											);	
											
	for(cnt = (MAIN_BUTTON)MAIN_Min; cnt<(MAIN_BUTTON)MAIN_Max; cnt++)			
			TFTSevice_Button_08x16(&Main_Button[cnt]);	
}


uint8_t Main_ButtonProcess(void)
{			
	static int16_t touchX, touchY; 	
	MAIN_BUTTON cnt;
	
	TouchFinger Shellmo_event = TouchPanel_GetPoint(&touchX,&touchY);	
	
	switch (Shellmo_event)
	{
		case TouchFinger_None: 	
			return 1;
		case TouchFinger_Up: 					
						for (cnt = MAIN_Min; cnt < MAIN_Max; cnt++)
							if (TFTSevice_RegionEvent(&Main_Button[cnt], touchX, touchY, Shellmo_event))
							{
								TFTSevice_Button_08x16(&Main_Button[cnt]);			
								if( Main_ButtonEvent (cnt))			Main_Render();
							}
		default:								
						for (cnt = MAIN_Min; cnt < MAIN_Max; cnt++)
							if (TFTSevice_RegionEvent(&Main_Button[cnt], touchX, touchY, Shellmo_event))
								TFTSevice_Button_08x16(&Main_Button[cnt]);			
	}
	return 1;
}

void Main_Window (void)
{
	uint8_t cnt;
	int16_t posx, posy;
		
	posx = Main_ButtonPOSX;
	posy = Main_ButtonPOSY;
		
	for(cnt = (MAIN_BUTTON)MAIN_Min; cnt<(MAIN_BUTTON)MAIN_Max; cnt++)			
	{ 
			Main_Button[cnt].MinX = posx;
			Main_Button[cnt].MinY = posy;
			Main_Button[cnt].MaxX = Main_Button[cnt].MinX + Main_ButtonSIZE;
			Main_Button[cnt].MaxY = Main_Button[cnt].MinY + Main_FONTSize_Button_Y;	
			
			if ((cnt == (MAIN_BUTTON)MAIN_Setting)|| (cnt == (MAIN_BUTTON)MAIN_Control))
			{
				posx = Main_Button[cnt].MaxX + Main_Separate_Button*2;
				posy = Main_ButtonPOSY;
			}
			else
				posy = Main_Button[cnt].MaxY + Main_Separate_Button; 			
				
			Main_Button[cnt].Normal.Text 					= MAIN_String[cnt];
			Main_Button[cnt].Selected.Text 				= MAIN_String[cnt];
			Main_Button[cnt].Disable.Text 				= MAIN_String[cnt];
			Main_Button[cnt].Normal.ForeColor 		= Main_ForeColor_ButtonNormal;
			Main_Button[cnt].Normal.BackColor 		= Main_BackColor_ButtonNormal;
			Main_Button[cnt].Selected.ForeColor 	= Main_ForeColor_ButtonSelected;
			Main_Button[cnt].Selected.BackColor 	= Main_BackColor_ButtonSelected;
			Main_Button[cnt].Disable.ForeColor 		= Main_ForeColor_ButtonSelected;
			Main_Button[cnt].Disable.BackColor 		= Main_BackColor_ButtonSelected;
	}
	
	Main_Render();
	
	while(Main_ButtonProcess())
	{		
		TFT_Text_Background (275,18, ASCII_08x16,		TFT_RED, Main_Theme_Title, "%2.1f V", Get_Voltage());	
	}
}
