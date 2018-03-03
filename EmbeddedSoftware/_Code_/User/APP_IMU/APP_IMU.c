
#include "TFTService_Text.h"
#include "TFTService_Draw.h"
#include "TFTService_Button.h"
#include "TFT_SSD1289.h"
#include "Input.h"
#include "TFTService_Dialog.h"
#include "TFTService_Keyboard.h"
#include "Timing_Manage.h"
#include "Dev_DIGITAL.h"
#include "APP_IMU.h"


static IMU_Status IMU_State = IMU_NONE;
static uint64_t IMU_Lastime;
static int16_3D Raw_Acce, Raw_Gyro;
static float IMU_dt, IMU_dtms;
static int16_t IMU_freq = IMU_FREQUENCY;
static  EulerAngle IMU_Angle;
static Quaternion IMU_Qua, IMU_QUnit;
static int16_3D NewAngle, LastAngle;

typedef enum 
{
	IMUWindow_Min				= 0 ,
	IMUWindow_Switch		= 0 ,
	IMUWindow_Changedt	= 1 ,
	IMUWindow_Exit 			= 2 ,
	IMUWindow_Max				= 3 ,
}IMUWindow_BUTTON;

static char *IMUWindow_ButtonString[IMUWindow_Max][2] = 
{	
	" Turn Off "," Turn On  ",
	"   Freq   ","   Freq   ",	
	"   Exit   ","   Exit   ",	
};

static TFTService_Button IMUWindow_Button[IMUWindow_Max];

int16_t IMU_FixAngle(float in)
{		
		return 90*sin(in*DEG_TO_RAD);
}


void IMUWindow_RenderData (void)
{
	int16_t line = 0;
	
	// Angle
	TFT_Text_Background (IMUWindow_DATA_POSX,IMUWindow_DATA_POSY + 16*(line++), IMUWindow_DATA_FONT,  TFT_BLUE, IMUWindow_Background, "Roll : %4d", (int)IMU_Angle.Roll);	
	TFT_Text_Background (IMUWindow_DATA_POSX,IMUWindow_DATA_POSY + 16*(line++), IMUWindow_DATA_FONT,  TFT_BLUE, IMUWindow_Background, "Pitch: %4d", (int)IMU_Angle.Pitch);	
	TFT_Text_Background (IMUWindow_DATA_POSX,IMUWindow_DATA_POSY + 16*(line++), IMUWindow_DATA_FONT,  TFT_BLUE, IMUWindow_Background, "Yaw  : %4d", (int)IMU_Angle.Yaw);	
	
	// Gyro
	TFT_Text_Background (IMUWindow_DATA_POSX,IMUWindow_DATA_POSY + 16*(line++), IMUWindow_DATA_FONT,  TFT_RED, IMUWindow_Background,  "Gyro X: %8d", Raw_Gyro.x);		
	TFT_Text_Background (IMUWindow_DATA_POSX,IMUWindow_DATA_POSY + 16*(line++), IMUWindow_DATA_FONT,  TFT_RED, IMUWindow_Background,  "Gyro Y: %8d", Raw_Gyro.y);	
	TFT_Text_Background (IMUWindow_DATA_POSX,IMUWindow_DATA_POSY + 16*(line++), IMUWindow_DATA_FONT,  TFT_RED, IMUWindow_Background,  "Gyro Z: %8d", Raw_Gyro.z);	
	
	// Accel
	TFT_Text_Background (IMUWindow_DATA_POSX,IMUWindow_DATA_POSY + 16*(line++), IMUWindow_DATA_FONT,  TFT_GREEN, IMUWindow_Background,"Acce X: %8d", Raw_Acce.x);		
	TFT_Text_Background (IMUWindow_DATA_POSX,IMUWindow_DATA_POSY + 16*(line++), IMUWindow_DATA_FONT,  TFT_GREEN, IMUWindow_Background,"Acce Y: %8d", Raw_Acce.y);	
	TFT_Text_Background (IMUWindow_DATA_POSX,IMUWindow_DATA_POSY + 16*(line++), IMUWindow_DATA_FONT,  TFT_GREEN, IMUWindow_Background,"Acce Z: %8d", Raw_Acce.z);	
	
	TFT_Text_Background (IMUWindow_DATA_POSX,IMUWindow_DATA_POSY + 16*(line++), IMUWindow_DATA_FONT,  TFT_BLACK, IMUWindow_Background,"dt(ms): %1.4f", IMU_dtms);	
	TFT_Text_Background (IMUWindow_DATA_POSX,IMUWindow_DATA_POSY + 16*(line++), IMUWindow_DATA_FONT,  TFT_BLACK, IMUWindow_Background,"f(Hz) : %8d", IMU_freq);	
	
	NewAngle.x = IMU_FixAngle(IMU_Angle.Roll) ;
	NewAngle.y = IMU_FixAngle(IMU_Angle.Pitch) ;
	NewAngle.z = IMU_FixAngle(IMU_Angle.Yaw) ;
	TFT_Text_Background (
												IMUWindow_Attitude_POSX,
												IMUWindow_DATA_POSY + IMUWindow_Attitude_SIZE + IMUWindow_Attitude_MOUSESIZE*2 + 1,
												IMUWindow_DATA_FONT,
												TFT_RED, IMUWindow_Background,
												"x: %3d y: %3d z: %3d", LastAngle.x, LastAngle.y, LastAngle.z);	
	
	if (NewAngle.x != LastAngle.x || NewAngle.y != LastAngle.y)
	{
		TFT_DrawFullCircle(
												IMUWindow_Background, 
												IMUWindow_Attitude_MOUSESPOSX - LastAngle.x * IMUWindow_Attitude_MOUSESCALE,
												IMUWindow_Attitude_MOUSESPOSY + LastAngle.y * IMUWindow_Attitude_MOUSESCALE, 
												IMUWindow_Attitude_MOUSESIZE);	
		
		LastAngle.x = NewAngle.x;
		LastAngle.y = NewAngle.y;
		
		TFT_DrawFullCircle(
												TFT_RED, 
												IMUWindow_Attitude_MOUSESPOSX - NewAngle.x * IMUWindow_Attitude_MOUSESCALE,
												IMUWindow_Attitude_MOUSESPOSY + NewAngle.y * IMUWindow_Attitude_MOUSESCALE, 
												IMUWindow_Attitude_MOUSESIZE);	
	}
}

void IMUWindow_Render (void)
{		
	uint8_t cnt;
	
  TFT_Clear(IMUWindow_Background);
	
	//Draw titile	
	TFT_DrawFullRect (IMUWindow_Theme_Title, IMUWindow_TITLE_POSX, IMUWindow_TITLE_POSY, IMUWindow_TITLE_SIZEX, IMUWindow_TITLE_SIZEY);
	
	TFT_Text_Transparent(84, 10, ASCII_16x24,	TFT_RED, "IMU Setup");
	TFT_Text_Transparent(85, 10, ASCII_16x24,	TFT_RED, "IMU Setup");
	TFT_Text_Transparent(294, 3, ASCII_08x12,	TFT_WHITE, "VND");	
	TFT_Text_Transparent(295, 3, ASCII_08x12,	TFT_WHITE, "VND");	
		
	
	TFT_DrawRect(
								TFT_BLUE, 
								IMUWindow_Attitude_POSX - IMUWindow_Attitude_MOUSESIZE, 
								IMUWindow_Attitude_POSY - IMUWindow_Attitude_MOUSESIZE,
								IMUWindow_Attitude_SIZE + IMUWindow_Attitude_MOUSESIZE*2,
								IMUWindow_Attitude_SIZE + IMUWindow_Attitude_MOUSESIZE*2);
	


	
	for(cnt = (IMUWindow_BUTTON)IMUWindow_Min; cnt<(IMUWindow_BUTTON)IMUWindow_Max; cnt++)		
	{				
			if (IMU_State == IMU_OK)			
			{		
				IMUWindow_Button[cnt].Normal.Text 				= IMUWindow_ButtonString[cnt][0];
				IMUWindow_Button[cnt].Selected.Text 			= IMUWindow_ButtonString[cnt][0];
				IMUWindow_Button[cnt].Disable.Text 				= IMUWindow_ButtonString[cnt][0];		
			}				
			else	
			{				
				IMUWindow_Button[cnt].Normal.Text 				= IMUWindow_ButtonString[cnt][1];
				IMUWindow_Button[cnt].Selected.Text 			= IMUWindow_ButtonString[cnt][1];
				IMUWindow_Button[cnt].Disable.Text 				= IMUWindow_ButtonString[cnt][1];		
			}			
			TFTSevice_Button_08x16(&IMUWindow_Button[cnt]);	
	}
							
	IMUWindow_RenderData();
}

void IMUWindow_ErrorConfirm(const char *text)
{
	TFTSevice_ShowDialogOK(
									IMUWindow_POS_Confirm_X, IMUWindow_POS_Confirm_Y, 
									IMUWindow_SIZE_Confirm_X, IMUWindow_SIZE_Confirm_Y, 
									"  Error",
									text);
									IMUWindow_Render();
}


IMU_Status IMU_ChangeFreq (int16_t freq)
{
	if (freq>100) IMUWindow_ErrorConfirm("Frequency is too high");	
	if (freq>0)
	{		
		IMU_freq = freq; 
		IMU_dt = 1.0f/(float)freq;
		IMU_dtms = IMU_dt*1000;
		return IMU_OK;	
	}
	else return IMU_NONE;
}

uint8_t IMUWindow_ButtonEvent(IMUWindow_BUTTON cnt)
{
	char *Dt;
	switch (cnt)
	{
		case IMUWindow_Switch:
						if (IMU_State == IMU_NONE) 
						{
							for(cnt = (IMUWindow_BUTTON)IMUWindow_Min; cnt<(IMUWindow_BUTTON)IMUWindow_Max; cnt++)		
							{											
								IMUWindow_Button[IMUWindow_Switch].Normal.Text 				= IMUWindow_ButtonString[IMUWindow_Switch][0];
								TFTSevice_Button_08x16(&IMUWindow_Button[cnt]);	
							}
							IMU_State = IMU_Initialize_Fail;
						}
						else
						{
							for(cnt = (IMUWindow_BUTTON)IMUWindow_Min; cnt<(IMUWindow_BUTTON)IMUWindow_Max; cnt++)		
							{											
								IMUWindow_Button[IMUWindow_Switch].Normal.Text 				= IMUWindow_ButtonString[IMUWindow_Switch][1];
								TFTSevice_Button_08x16(&IMUWindow_Button[cnt]);	
							}
							IMU_State = IMU_NONE;
						}						
						return 1;
		case IMUWindow_Changedt: 
						if (TFTService_Keyboard("New Frequency",&Dt)) 
						{
							if (IMU_ChangeFreq (TFTService_Keyboard_StringToInt64(Dt)) != IMU_OK)			
								IMUWindow_ErrorConfirm("Wrong frequency");	
						}
						IMUWindow_Render(); 
						return 1;
		case IMUWindow_Exit:			return 0;
		default: 									return 1;	
	}
}

uint8_t IMUWindow_TouchProcess(void)
{			
	static int16_t touchX, touchY; 	
	IMUWindow_BUTTON cnt;
	
	TouchFinger IMUWindow_event = TouchPanel_GetPoint(&touchX,&touchY);	
	switch (IMUWindow_event)
	{
		case TouchFinger_None: 	
			return 1;
		case TouchFinger_Up: 					
							for(cnt = (IMUWindow_BUTTON)IMUWindow_Min; cnt<(IMUWindow_BUTTON)IMUWindow_Max; cnt++)		
								if (TFTSevice_RegionEvent(&IMUWindow_Button[cnt], touchX, touchY, IMUWindow_event))
								{
									TFTSevice_Button_08x16(&IMUWindow_Button[cnt]);			
									return IMUWindow_ButtonEvent (cnt);	
								}
		default:								
							for(cnt = (IMUWindow_BUTTON)IMUWindow_Min; cnt<(IMUWindow_BUTTON)IMUWindow_Max; cnt++)		
								if (TFTSevice_RegionEvent(&IMUWindow_Button[cnt], touchX, touchY, IMUWindow_event))
									TFTSevice_Button_08x16(&IMUWindow_Button[cnt]);			
	}
	return 1;
}




EulerAngle IMU_Process(uint8_t cmd, IMU_Status *State)
{
	switch (IMU_State)
	{
		case IMU_NONE:
						break;
		case IMU_Initialize_Fail:						
						if (Get_msTick() - IMU_Lastime < IMU_dtms)			break;
						else 
						{
							if (IMU_Initialize(&IMU_Qua, &IMU_QUnit, &IMU_Angle)== IMU_Initialize_Fail)	
							{								
								if (cmd) 	
								{
									IMUWindow_RenderData();									
									IMUWindow_ErrorConfirm("MPU6050 initializes fail");	
								}
								IMU_State = IMU_NONE;
							}
							else	
							{
									IMU_State = IMU_OK;
							}
							break;
						}
		case IMU_Read_Fail:	
						if (Get_msTick() - IMU_Lastime < IMU_dtms)			break;
						else
						{
							if (IMU_Initialize(&IMU_Qua, &IMU_QUnit, &IMU_Angle)== IMU_Initialize_Fail)	
							{								
								if (cmd) 	IMUWindow_ErrorConfirm("MPU6050 re-initializes fail");	
			
								IMU_State = IMU_NONE;
							}
							else
								IMU_State = IMU_OK;
							break;
						}
		case IMU_OK:
						if (Get_msTick() - IMU_Lastime < IMU_dtms)			break;
						else
						{						
							IMU_Lastime = Get_msTick();
							if (IMU_Update_Data (&Raw_Acce, &Raw_Gyro) != IMU_OK)			
							{
								if (cmd) 	IMUWindow_ErrorConfirm("MPU6050 Fail to read");	
								IMU_State = IMU_Read_Fail;
								break;
							}
							else	
							{			
//								IMU_Angle = IMU_Compensate(&Raw_Acce, &Raw_Gyro, IMU_Angle, IMU_dt);		
								if (Get_msTick() - IMU_Lastime >= 2*IMU_dtms)										
									{										
										if (cmd) IMUWindow_ErrorConfirm("Sampling fail");										
										IMU_ChangeFreq (IMU_FREQUENCY);
										IMU_State = IMU_Initialize_Fail;
									}
								IMU_AHRS(&Raw_Acce, &Raw_Gyro, &IMU_Qua, &IMU_dt);		
								IMU_Angle = IMU_QuaternionToEuler(IMU_Qua);			
								if (cmd) 	IMUWindow_RenderData();					
								break;
							}
						}		
	}		
	*State = IMU_State;
	return IMU_Angle;
}

void IMU_Window (void)
{
	uint8_t cnt;
	int16_t posx, posy;
	IMU_Status   State;
	posx = IMUWindow_ButtonPOSX;
	posy = IMUWindow_ButtonPOSY;		
	
	for(cnt = (IMUWindow_BUTTON)IMUWindow_Min; cnt<(IMUWindow_BUTTON)IMUWindow_Max; cnt++)		
	{ 
			IMUWindow_Button[cnt].MinX = posx;
			IMUWindow_Button[cnt].MinY = posy;
			IMUWindow_Button[cnt].MaxX = IMUWindow_Button[cnt].MinX + IMUWindow_ButtonSIZE;
			IMUWindow_Button[cnt].MaxY = IMUWindow_Button[cnt].MinY + IMUWindow_FONTSize_Button_Y;	
			
			posx = IMUWindow_Button[cnt].MaxX + IMUWindow_Separate; 			
			
			IMUWindow_Button[cnt].Normal.ForeColor 		= IMUWindow_ForeColor_ButtonNormal;
			IMUWindow_Button[cnt].Normal.BackColor 		= IMUWindow_BackColor_ButtonNormal;
			IMUWindow_Button[cnt].Normal.Text 				= IMUWindow_ButtonString[cnt][0];
			IMUWindow_Button[cnt].Selected.ForeColor 	= IMUWindow_ForeColor_ButtonSelected;
			IMUWindow_Button[cnt].Selected.BackColor 	= IMUWindow_BackColor_ButtonSelected;
			IMUWindow_Button[cnt].Selected.Text 			= IMUWindow_ButtonString[cnt][0];
			IMUWindow_Button[cnt].Disable.ForeColor 	= IMUWindow_ForeColor_ButtonSelected;
			IMUWindow_Button[cnt].Disable.BackColor 	= IMUWindow_BackColor_ButtonSelected;
			IMUWindow_Button[cnt].Disable.Text 				= IMUWindow_ButtonString[cnt][0];			
	}
	
	IMU_ChangeFreq (IMU_freq);
	IMUWindow_Render();
	
	while(IMUWindow_TouchProcess())
	{
		IMU_Process(1, &State);
	}
}
