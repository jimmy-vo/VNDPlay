

#define ARMBOT_Separate 									7
//////////////////////////////////////////////////////////////////////////////////
#define ARMBOT_TITLE_POSX									1
#define ARMBOT_TITLE_POSY									1

#define ARMBOT_TITLE_SIZEX								320 - (ARMBOT_TITLE_POSX*2)
#define ARMBOT_TITLE_SIZEY								35

//////////////////////////////////////////////////////////////////////////////////
#define ARMBOT_VNDPlayVF_POSX									320 - ARMBOT_VNDPlayVF_FONTX * 15 - ARMBOT_Separate
#define ARMBOT_VNDPlayVF_POSY									ARMBOT_TITLE_POSY + ARMBOT_TITLE_SIZEY + ARMBOT_Separate

#define ARMBOT_VNDPlayVF_FONT									ASCII_08x16 
#define ARMBOT_VNDPlayVF_FONTX								8
#define ARMBOT_VNDPlayVF_FONTY								16
//////////////////////////////////////////////////////////////////////////////////

#define ARMBOT_Servo_FONT									ASCII_08x16 
#define ARMBOT_Servo_FONTX								8
#define ARMBOT_Servo_FONTY								16

#define ARMBOT_Servo_POSY									240 - ARMBOT_Servo_FONTY 

//////////////////////////////////////////////////////////////////////////////////

#define ARMBOT_POS_Button_X							320 - ARMBOT_SIZE_Button_X - ARMBOT_Separate_Button
#define ARMBOT_POS_Button_Y							240 - (ARMBOT_FONTSize_Button_Y + ARMBOT_Separate_Button)*3
	
#define ARMBOT_FONT_Button							ASCII_08x16 
#define ARMBOT_FONTSize_Button_X				8
#define ARMBOT_FONTSize_Button_Y				16
#define ARMBOT_Separate_Button					22

#define ARMBOT_SIZE_Button_X						ARMBOT_FONTSize_Button_X * 11
#define ARMBOT_SIZE_Button_Y						ARMBOT_FONTSize_Button_Y * 2 + ARMBOT_Separate_Button


//////////////////////////////////////////////////////////////////////////////////

#define ARMBOT_Background									TFT_WHITE
#define ARMBOT_Theme_Title								TFT_BLACK



#define	ARMBOT_ForeColor_ButtonNormal					TFT_BLACK
#define	ARMBOT_BackColor_ButtonNormal					TFT_LGRAY

#define	ARMBOT_ForeColor_ButtonSelected				TFT_BLACK
#define	ARMBOT_BackColor_ButtonSelected				TFT_DGRAY

void RobotArm_Window (void);

