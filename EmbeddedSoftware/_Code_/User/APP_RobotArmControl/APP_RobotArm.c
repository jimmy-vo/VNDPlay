#include "APP_RobotArm.h"
#include "TFTService_Button.h"
#include "Bluetooth.h"
#include "TFTService_Text.h"
#include "TFTService_Draw.h"
#include "TFT_SSD1289.h"
#include "Input.h"
#include "Voltage_FB.h"
#include "TFTService_TrackBar.h"
#include "TFTService_Dialog.h"

typedef enum 
{
	RobotArm_Min				= 0 ,
	RobotArm_Bluetooth 	= 0 ,
	RobotArm_Reset 			= 1 ,
	RobotArm_Exit 			= 2 ,
	RobotArm_Max				= 3 ,
}RobotArm_BUTTON;


static char *RobotArmButtonString[5] = 
{	
	" Bluetooth ",	
	"   Reset   ",	
	"   Exit    "	
};

static TFTService_TrackBar Servo[6];
static char RobotArm_Serial [4+12];
static TFTService_Button RobotArm_Button[RobotArm_Max];

void RobotArm_Render (void)
{
	uint8_t cnt;
	
  TFT_Clear(ARMBOT_Background);	
	
	//Draw titile	
	TFT_DrawFullRect (ARMBOT_Theme_Title, ARMBOT_TITLE_POSX, ARMBOT_TITLE_POSY, ARMBOT_TITLE_SIZEX, ARMBOT_TITLE_SIZEY);
	
	TFT_Text_Transparent(66, 10, ASCII_16x24,	TFT_BLUE, "Arm Control");
	TFT_Text_Transparent(67, 10, ASCII_16x24,	TFT_BLUE, "Arm Control");
	TFT_Text_Transparent(64, 10, ASCII_16x24,	TFT_BLUE, "Arm Control");
	TFT_Text_Transparent(65, 10, ASCII_16x24,	TFT_BLUE, "Arm Control");
	TFT_Text_Transparent(294, 3, ASCII_08x12,	TFT_WHITE, "VND");	
	TFT_Text_Transparent(295, 3, ASCII_08x12,	TFT_WHITE, "VND");	
	
	for(cnt = (RobotArm_BUTTON)RobotArm_Min; cnt<(RobotArm_BUTTON)RobotArm_Max; cnt++)	
		TFTSevice_Button_08x16(&RobotArm_Button[cnt]);			
		
	for (cnt = 0; cnt < 6; cnt++)
	{
		TFTSevice_TrackBarRender (&Servo[cnt]);		
		TFT_Text_Background(Servo[cnt].MinX ,Servo[cnt].MaxY + ARMBOT_Separate, ASCII_08x16,  TFT_BLACK, ARMBOT_Background,"%3d",(int16_t)Servo[cnt].Value);	
	}
}

void RobotArm_SendData(void)
{
	uint8_t Index = 0, cnt;
	RobotArm_Serial[Index++] = 'V';
	RobotArm_Serial[Index++] = 'N';
	RobotArm_Serial[Index++] = 'D';
	RobotArm_Serial[Index++] = 'S';
	
	for (cnt = 0; cnt < 12; cnt++)
		RobotArm_Serial[Index++] = (char) Servo[cnt].Value;
		
	
	Bluetooth_Send(RobotArm_Serial, sizeof (RobotArm_Serial));	
}

uint8_t RobotArm_TouchProcess(void)
{			
	static int16_t touchX=0, touchY=0; 	
	uint8_t cnt;
	
	TouchFinger event = TouchPanel_GetPoint(&touchX,&touchY);		
	for (cnt = 0; cnt < 6; cnt++)
		if (TFTSevice_TrackBarEvent(&Servo[cnt] ,touchX, touchY, event ) )	
			return 1;
	return 0;
}

uint8_t RobotArm_ButtonEvent(RobotArm_BUTTON cnt)
{
	uint8_t servos;
	switch (cnt)
	{
		case RobotArm_Bluetooth: Bluetooth_Window(); RobotArm_Render(); return 1;
		case RobotArm_Reset: 			
															for (servos=0; servos<6; servos++) 
																TFTSevice_TrackBarValueRender ( &Servo[servos],Servo[servos].Value = 50); 
															return 1;
		case RobotArm_Exit:				return 0;
		default: 									return 1;	
	}
}

uint8_t RobotArm_ButtonProcess(void)
{			
	static int16_t touchX=0, touchY=0; 	
	RobotArm_BUTTON cnt;
	
	TouchFinger event = TouchPanel_GetPoint(&touchX,&touchY);		
		switch (event)
		{
			case TouchFinger_None: 	return 1;
			case TouchFinger_Up: 					
							for (cnt = RobotArm_Min; cnt < RobotArm_Max; cnt++)
								if (TFTSevice_RegionEvent(&RobotArm_Button[cnt], touchX, touchY, event))
								{
									TFTSevice_Button_08x16(&RobotArm_Button[cnt]);			
									return RobotArm_ButtonEvent (cnt);	
								}									
			default:								
							for (cnt = RobotArm_Min; cnt < RobotArm_Max; cnt++)
								if (TFTSevice_RegionEvent(&RobotArm_Button[cnt], touchX, touchY, event))
									TFTSevice_Button_08x16(&RobotArm_Button[cnt]);			
		}
	return 1;
}

uint8_t RobotArm_InputProcess(void)
{
	uint8_t flag = 0;
	float analog;
	
		if (ButtonA_Up()) 	 	
		{
			TFTSevice_TrackBarValueRender ( &Servo[0],Servo[0].Value + 0.1f);
			flag++;
		}
		
		if (ButtonA_Down())   	
		{ 
			TFTSevice_TrackBarValueRender ( &Servo[0], Servo[0].Value - 0.1f);
			flag++;
		}
		
		if (ButtonA_Left())  	 	
		{
			TFTSevice_TrackBarValueRender ( &Servo[1], Servo[1].Value + 0.1f);
			flag++;
		}
		if (ButtonA_Right()) 
		{
			TFTSevice_TrackBarValueRender ( &Servo[1], Servo[1].Value - 0.1f);
			flag++;
		}
		
		if (ButtonB_Up()) 	
		{
			TFTSevice_TrackBarValueRender ( &Servo[2], Servo[2].Value + 0.1f);
			flag++;
		}
		
		if (ButtonB_Down()) 	
		{
			TFTSevice_TrackBarValueRender ( &Servo[2], Servo[2].Value - 0.1f);
			flag++;
		}
		
		if ((analog = Joystick_AY())!=0)
		{
			TFTSevice_TrackBarValueRender ( &Servo[3], Servo[3].Value + analog*0.01f);
			flag++;
		}
		
		if (ButtonB_Left()) 		
		{
			TFTSevice_TrackBarValueRender ( &Servo[5], Servo[5].Value + 0.1f);
			flag++;
		}
		if (ButtonB_Right()) 	
		{
			TFTSevice_TrackBarValueRender ( &Servo[5], Servo[5].Value - 0.1f);
			flag++;
		}
		
		if ((analog = Joystick_BY())!=0)
		{
			TFTSevice_TrackBarValueRender ( &Servo[4], Servo[4].Value + analog*0.01f);
			flag++;
		}
		
		return flag;
}

void RobotArm_Window (void)
{	
	uint8_t cnt;
	int16_t posx, posy;
		
	posx = ARMBOT_POS_Button_X;
	posy = ARMBOT_POS_Button_Y;
	
	for(cnt = (RobotArm_BUTTON)RobotArm_Min; cnt<(RobotArm_BUTTON)RobotArm_Max; cnt++)		
	{ 
			RobotArm_Button[cnt].MinX = posx;
			RobotArm_Button[cnt].MinY = posy;
			RobotArm_Button[cnt].MaxX = RobotArm_Button[cnt].MinX + ARMBOT_SIZE_Button_X;
			RobotArm_Button[cnt].MaxY = RobotArm_Button[cnt].MinY + ARMBOT_FONTSize_Button_Y;	
			
			posy = RobotArm_Button[cnt].MaxY + ARMBOT_Separate_Button; 			
			
			RobotArm_Button[cnt].Normal.ForeColor 		= ARMBOT_ForeColor_ButtonNormal;
			RobotArm_Button[cnt].Normal.BackColor 		= ARMBOT_BackColor_ButtonNormal;
			RobotArm_Button[cnt].Normal.Text 					= RobotArmButtonString[cnt];
			RobotArm_Button[cnt].Selected.ForeColor 	= ARMBOT_ForeColor_ButtonSelected;
			RobotArm_Button[cnt].Selected.BackColor 	= ARMBOT_BackColor_ButtonSelected;
			RobotArm_Button[cnt].Selected.Text 				= RobotArmButtonString[cnt];
			RobotArm_Button[cnt].Disable.ForeColor 		= ARMBOT_ForeColor_ButtonSelected;
			RobotArm_Button[cnt].Disable.BackColor 		= ARMBOT_BackColor_ButtonSelected;
			RobotArm_Button[cnt].Disable.Text 				= RobotArmButtonString[cnt];			
	}
	
	posx = ARMBOT_Separate;
	for (cnt = 0; cnt < 6; cnt++)
	{		
		Servo[cnt].MinX = posx;
		Servo[cnt].MinY = ARMBOT_VNDPlayVF_POSY;
		Servo[cnt].MaxX = Servo[cnt].MinX  + ARMBOT_Servo_FONTX*3;
		Servo[cnt].MaxY = ARMBOT_Servo_POSY - ARMBOT_Separate;
		Servo[cnt].MaxValue = 100;
		Servo[cnt].MinValue = 0;
		Servo[cnt].Step 		 = 1;
		Servo[cnt].Type 		 = TFTService_TrackBar_Vertical;
		posx = Servo[cnt].MaxX + ARMBOT_Separate;
	}
	
	RobotArm_Render();
  
	while(RobotArm_ButtonProcess())
	{				
		// servos 
		if (RobotArm_InputProcess()==1 || RobotArm_TouchProcess() == 1)
		{					
			for (cnt = 0; cnt < 6; cnt++)	
				TFT_Text_Background(Servo[cnt].MinX ,Servo[cnt].MaxY + ARMBOT_Separate, ASCII_08x16,  TFT_BLACK, ARMBOT_Background,"%3d",(int16_t)Servo[cnt].Value);	
			RobotArm_SendData();
		}
		
		// volt fb
		TFT_Text_Background (ARMBOT_VNDPlayVF_POSX, ARMBOT_VNDPlayVF_POSY, ARMBOT_VNDPlayVF_FONT,		TFT_RED, ARMBOT_Background, "VND Play: %2.1f V", Get_Voltage());	
	}
}
