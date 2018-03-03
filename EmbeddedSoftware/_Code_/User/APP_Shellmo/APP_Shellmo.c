#include "APP_Shellmo.h"
#include "TFTService_Text.h"
#include "TFTService_Draw.h"
#include "TFTService_Button.h"
#include "TFT_SSD1289.h"
#include "Input.h"
#include "Voltage_FB.h"
#include "Bluetooth.h"
#include "TFTService_Dialog.h"
#include "APP_IMU.h"

typedef enum 
{
	ShellmoChoice_Min				= 0 ,
	ShellmoChoice_Tilt			= 0 ,
	ShellmoChoice_Bluetooth = 1 ,
	ShellmoChoice_Exit 			= 2 ,
	ShellmoChoice_Max				= 3 ,
}ShellmoChoice_BUTTON;

static char *ShellmoChoiceString[ShellmoChoice_Max][2] = 
{	
	" Tilt: ON  ",	" Tilt: OFF ",	
	" Bluetooth ",	" Bluetooth ",
	"   Exit    ", 	"   Exit    "	
};

static EulerAngle TiltAngle;
static int16_t TiltX, TiltY;
static char CHAR[10];
static TFTService_Button Shellmo_Choice[ShellmoChoice_Max];
static int16_t	JLX=0, JLY=0, JRX=0, JRY=0;	

void Shellmo_UpdateInput (void)
{
	// volt fb
	TFT_Text_Background (120,Shellmo_TITLE_SIZEY + Shellmo_Separate , ASCII_08x16, TFT_RED, Shellmo_Background, "VDD: %2.1f V", Get_Voltage());	
					
	//joystick button			
	if (JLX != Joystick_AX()|JLY != Joystick_AY())
	{
		TFT_DrawFullCircle(Shellmo_Background, Shellmo_JLMOUSESPOSX+JLX*Shellmo_J_MOUSESCALE, Shellmo_JMOUSESPOSY-JLY*Shellmo_J_MOUSESCALE, Shellmo_J_MOUSESIZE);	
		JLX = Joystick_AX();		JLY = Joystick_AY();	
	
	}
	if (JRX != Joystick_BX()|JRY != Joystick_BY())
	{
		TFT_DrawFullCircle(Shellmo_Background, Shellmo_JRMOUSESPOSX+JRX*Shellmo_J_MOUSESCALE, Shellmo_JMOUSESPOSY-JRY*Shellmo_J_MOUSESCALE, Shellmo_J_MOUSESIZE);	
		JRX = Joystick_BX();			JRY = Joystick_BY();	
	}
	
	if (!ButtonA_Joystick())		TFT_DrawFullCircle(TFT_BLACK, Shellmo_JLMOUSESPOSX+JLX*Shellmo_J_MOUSESCALE, Shellmo_JMOUSESPOSY-JLY*Shellmo_J_MOUSESCALE, Shellmo_J_MOUSESIZE);	
	else											TFT_DrawFullCircle(TFT_RED, Shellmo_JLMOUSESPOSX+JLX*Shellmo_J_MOUSESCALE, Shellmo_JMOUSESPOSY-JLY*Shellmo_J_MOUSESCALE, Shellmo_J_MOUSESIZE);	
	if (!ButtonB_Joystick())		TFT_DrawFullCircle(TFT_BLACK, Shellmo_JRMOUSESPOSX+JRX*Shellmo_J_MOUSESCALE, Shellmo_JMOUSESPOSY-JRY*Shellmo_J_MOUSESCALE, Shellmo_J_MOUSESIZE);	
	else											TFT_DrawFullCircle(TFT_RED, Shellmo_JRMOUSESPOSX+JRX*Shellmo_J_MOUSESCALE, Shellmo_JMOUSESPOSY-JRY*Shellmo_J_MOUSESCALE, Shellmo_J_MOUSESIZE);	

	if (ButtonA_Left()) 			TFT_DrawFullCircle(TFT_BLACK, 	Shellmo_ButtonAPOSX-Shellmo_ButtonSIZE*2,	Shellmo_ButtonPOSY, Shellmo_ButtonSIZE);
										else		TFT_DrawFullCircle(Shellmo_Background,	Shellmo_ButtonAPOSX-Shellmo_ButtonSIZE*2,	Shellmo_ButtonPOSY, Shellmo_ButtonSIZE);
	if (ButtonA_Right()) 			TFT_DrawFullCircle(TFT_BLACK, 	Shellmo_ButtonAPOSX+Shellmo_ButtonSIZE*2,	Shellmo_ButtonPOSY, Shellmo_ButtonSIZE);
										else		TFT_DrawFullCircle(Shellmo_Background,	Shellmo_ButtonAPOSX+Shellmo_ButtonSIZE*2,	Shellmo_ButtonPOSY, Shellmo_ButtonSIZE);
	if (ButtonA_Up()) 				TFT_DrawFullCircle(TFT_BLACK, Shellmo_ButtonAPOSX, Shellmo_ButtonPOSY- Shellmo_ButtonSIZE*2, Shellmo_ButtonSIZE);
										else		TFT_DrawFullCircle(Shellmo_Background, Shellmo_ButtonAPOSX, Shellmo_ButtonPOSY- Shellmo_ButtonSIZE*2, Shellmo_ButtonSIZE);
	if (ButtonA_Down()) 			TFT_DrawFullCircle(TFT_BLACK,Shellmo_ButtonAPOSX, Shellmo_ButtonPOSY+ Shellmo_ButtonSIZE*2, Shellmo_ButtonSIZE);
										else		TFT_DrawFullCircle(Shellmo_Background,	Shellmo_ButtonAPOSX, Shellmo_ButtonPOSY+ Shellmo_ButtonSIZE*2, Shellmo_ButtonSIZE);
	if (ButtonB_Left()) 			TFT_DrawFullCircle(TFT_BLUE, 	Shellmo_ButtonBPOSX - Shellmo_ButtonSIZE*2,	Shellmo_ButtonPOSY, Shellmo_ButtonSIZE);
										else		TFT_DrawFullCircle(Shellmo_Background,	Shellmo_ButtonBPOSX - Shellmo_ButtonSIZE*2,	Shellmo_ButtonPOSY, Shellmo_ButtonSIZE);
	if (ButtonB_Right()) 			TFT_DrawFullCircle(TFT_RED, 	Shellmo_ButtonBPOSX + Shellmo_ButtonSIZE*2,	Shellmo_ButtonPOSY, Shellmo_ButtonSIZE);
										else		TFT_DrawFullCircle(Shellmo_Background,	Shellmo_ButtonBPOSX + Shellmo_ButtonSIZE*2,	Shellmo_ButtonPOSY, Shellmo_ButtonSIZE);
	if (ButtonB_Up()) 				TFT_DrawFullCircle(TFT_GREEN, Shellmo_ButtonBPOSX, Shellmo_ButtonPOSY - Shellmo_ButtonSIZE*2,Shellmo_ButtonSIZE);
										else		TFT_DrawFullCircle(Shellmo_Background, Shellmo_ButtonBPOSX, Shellmo_ButtonPOSY - Shellmo_ButtonSIZE*2,Shellmo_ButtonSIZE);
	if (ButtonB_Down()) 			TFT_DrawFullCircle(TFT_YELLOW,Shellmo_ButtonBPOSX, Shellmo_ButtonPOSY + Shellmo_ButtonSIZE*2,Shellmo_ButtonSIZE);
										else		TFT_DrawFullCircle(Shellmo_Background,	Shellmo_ButtonBPOSX, Shellmo_ButtonPOSY + Shellmo_ButtonSIZE*2,Shellmo_ButtonSIZE);
}

int8_t Shellmo_ProcessInput(void)
{
	int16_t cntx,cnty;
	uint8_t addr=0;
	
	if (JLX!=0|JLY!=0) 
	{
		cntx = JLX;
		cnty = JLY;
		while (cntx!=0||cnty!=0)
		{
			if 			(cnty>0)	{CHAR[addr++] = 'w'; cnty--;}
			else if (cnty<0)	{CHAR[addr++] = 's'; cnty++;}			
			
			if 			(cntx>0)	{CHAR[addr++] = 'd'; cntx--;}
			else if (cntx<0)	{CHAR[addr++] = 'a'; cntx++;}			
		}		
		
		Bluetooth_Send(CHAR,addr);
		return 1;
	}
	else if (ButtonA_Left()) 													CHAR[0] = 'a';
	else if (ButtonA_Right()) 												CHAR[0] = 'd';
	else if (ButtonA_Up()) 														CHAR[0] = 'w';
	else if (ButtonA_Down()) 													CHAR[0] = 's';
	else if (ButtonB_Left()||ButtonA_Joystick()) 			CHAR[0] = 'x';
	else if (ButtonB_Right()) 												CHAR[0] = 'c';
	else if (ButtonB_Up()) 														CHAR[0] = 'b';
	else if (ButtonB_Down()) 													CHAR[0] = 'v'; 
	else return 0;
	Bluetooth_Send(CHAR,1);
	return 1;
}

int8_t Shellmo_ProcessTilt (int16_t thrs)
{
	IMU_Status State;
	int16_t cnt=0;
	TiltAngle = IMU_Process(0,&State);
	if (State == IMU_OK)
	{
		TiltX = IMU_FixAngle(TiltAngle.Roll);
		TiltY = IMU_FixAngle(TiltAngle.Pitch);
		
		if (TiltX<thrs && TiltX>-thrs && TiltY<thrs && TiltY>-thrs) CHAR[cnt++] = 'x';	
			else
			{
				if (TiltX<-thrs) 	CHAR[cnt++] = 'd';	
				if (TiltX>thrs) 	CHAR[cnt++] = 'a';
				if (TiltY<-thrs) 	CHAR[cnt++] = 'w';	
				if (TiltY>thrs) 	CHAR[cnt++] = 's';
			}		
		Bluetooth_Send(CHAR,cnt);
		return 1;
	}
	else return 0;
}

void Shellmo_Render (void)
{		
	uint8_t cnt;
	
  TFT_Clear(Shellmo_Background);
	
	//Draw titile	
	TFT_DrawFullRect (Shellmo_Theme_Title, Shellmo_TITLE_POSX, Shellmo_TITLE_POSY, Shellmo_TITLE_SIZEX, Shellmo_TITLE_SIZEY);
	
	TFT_Text_Transparent(44, 10, ASCII_16x24,	TFT_BLUE, "SHELLMO Control");
	TFT_Text_Transparent(45, 10, ASCII_16x24,	TFT_BLUE, "SHELLMO Control");
	TFT_Text_Transparent(294, 3, ASCII_08x12,	TFT_WHITE, "VND");	
	TFT_Text_Transparent(295, 3, ASCII_08x12,	TFT_WHITE, "VND");	
		
	for(cnt = (ShellmoChoice_BUTTON)ShellmoChoice_Min; cnt<(ShellmoChoice_BUTTON)ShellmoChoice_Max; cnt++)			
		{
			if (Shellmo_ProcessTilt(30))
			{
				Shellmo_Choice[cnt].Normal.Text 				= ShellmoChoiceString[cnt][0];
				Shellmo_Choice[cnt].Selected.Text 			= ShellmoChoiceString[cnt][0];
				Shellmo_Choice[cnt].Disable.Text 				= ShellmoChoiceString[cnt][0];
			}
			else
			{				
				Shellmo_Choice[cnt].Normal.Text 				= ShellmoChoiceString[cnt][1];
				Shellmo_Choice[cnt].Selected.Text 			= ShellmoChoiceString[cnt][1];
				Shellmo_Choice[cnt].Disable.Text 				= ShellmoChoiceString[cnt][1];
			}
			TFTSevice_Button_08x16(&Shellmo_Choice[cnt]);	
		}
}

uint8_t Shellmo_ButtonEvent(ShellmoChoice_BUTTON cnt)
{
	switch (cnt)
	{
		case ShellmoChoice_Tilt: 
																		IMU_Window(); 
																		Shellmo_Render();
																		return 1;		
		case ShellmoChoice_Bluetooth: 
																		Bluetooth_Window(); 
																		Shellmo_Render();
																		return 1;
		case ShellmoChoice_Exit:				return 0;	
		default: 												return 1;	
	}
}

uint8_t Shellmo_ChoiceProcess(void)
{			
	static int16_t touchX, touchY; 	
	ShellmoChoice_BUTTON cnt;
	
	TouchFinger Shellmo_event = TouchPanel_GetPoint(&touchX,&touchY);	
	
//			TFT_Text_Background(0,0, ASCII_08x16,  TFT_WHITE, TFT_BLACK,"x: %3d\ny: %3d", touchX, touchY);	
	switch (Shellmo_event)
	{
		case TouchFinger_None: 	
			return 1;
		case TouchFinger_Up: 					
						for (cnt = ShellmoChoice_Min; cnt < ShellmoChoice_Max; cnt++)
							if (TFTSevice_RegionEvent(&Shellmo_Choice[cnt], touchX, touchY, Shellmo_event))
							{
								TFTSevice_Button_08x16(&Shellmo_Choice[cnt]);			
								return Shellmo_ButtonEvent (cnt);	
							}
		default:								
						for (cnt = ShellmoChoice_Min; cnt < ShellmoChoice_Max; cnt++)
							if (TFTSevice_RegionEvent(&Shellmo_Choice[cnt], touchX, touchY, Shellmo_event))
								TFTSevice_Button_08x16(&Shellmo_Choice[cnt]);			
	}
	return 1;
}

void Shellmo_Window (void)
{
	uint8_t cnt;
	int16_t posx, posy;
		
	posx = Shellmo_ChoicePOSX;
	posy = Shellmo_ChoicePOSY;
		
	for(cnt = (ShellmoChoice_BUTTON)ShellmoChoice_Min; cnt<(ShellmoChoice_BUTTON)ShellmoChoice_Max; cnt++)			
	{ 
			Shellmo_Choice[cnt].MinX = posx;
			Shellmo_Choice[cnt].MinY = posy;
			Shellmo_Choice[cnt].MaxX = Shellmo_Choice[cnt].MinX + Shellmo_ChoiceSIZE;
			Shellmo_Choice[cnt].MaxY = Shellmo_Choice[cnt].MinY + Shellmo_FONTSize_Choice_Y;	
			
			posy = Shellmo_Choice[cnt].MaxY + Shellmo_Separate_Choice; 			
			
			Shellmo_Choice[cnt].Normal.ForeColor 		= Shellmo_ForeColor_ButtonNormal;
			Shellmo_Choice[cnt].Normal.BackColor 		= Shellmo_BackColor_ButtonNormal;
			Shellmo_Choice[cnt].Selected.ForeColor 	= Shellmo_ForeColor_ButtonSelected;
			Shellmo_Choice[cnt].Selected.BackColor 	= Shellmo_BackColor_ButtonSelected;
			Shellmo_Choice[cnt].Disable.ForeColor 	= Shellmo_ForeColor_ButtonSelected;
			Shellmo_Choice[cnt].Disable.BackColor 	= Shellmo_BackColor_ButtonSelected;
	}
	
	Shellmo_Render();
	
	while(Shellmo_ChoiceProcess())
	{
		Shellmo_UpdateInput();
		Shellmo_ProcessInput();
		Shellmo_ProcessTilt(30);
	}
}
