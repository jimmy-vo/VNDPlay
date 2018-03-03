#include "SENSOR_DATA.h"

//////////////////////////////////////////////////////////////////////////////////

#define IMUWindow_Background								TFT_GREY
#define IMUWindow_Theme_Title								TFT_BLUE
#define IMUWindow_Theme_TERMINAL						TFT_BLACK

#define	IMUWindow_ForeColor_ButtonNormal					TFT_BLACK
#define	IMUWindow_BackColor_ButtonNormal					TFT_LGRAY

#define	IMUWindow_ForeColor_ButtonSelected				TFT_BLACK
#define	IMUWindow_BackColor_ButtonSelected				TFT_DGRAY

#define IMUWindow_Separate 									3

//////////////////////////////////////////////////////////////////////////////////

#define IMUWindow_TITLE_POSX									1
#define IMUWindow_TITLE_POSY									1
#define IMUWindow_TITLE_SIZEX									320 - (IMUWindow_TITLE_POSX*2)
#define IMUWindow_TITLE_SIZEY									35

//////////////////////////////////////////////////////////////////////////////////


#define IMUWindow_FONT_Button							ASCII_08x16 
#define IMUWindow_FONTSize_Button_X				8
#define IMUWindow_FONTSize_Button_Y				16

#define IMUWindow_ButtonSIZE					IMUWindow_FONTSize_Button_X*10
#define IMUWindow_ButtonPOSX					320 - IMUWindow_ButtonSIZE*3 - IMUWindow_Separate*3
#define IMUWindow_ButtonPOSY					240 - IMUWindow_FONTSize_Button_Y - IMUWindow_Separate

//////////////////////////////////////////////////////////////////////////////////


#define IMUWindow_DATA_FONT						  ASCII_08x16 
#define IMUWindow_DATA_FONTX						8
#define IMUWindow_DATA_FONTY						16

#define IMUWindow_DATA_POSX							IMUWindow_Separate
#define IMUWindow_DATA_POSY							IMUWindow_TITLE_POSY + IMUWindow_TITLE_SIZEY + IMUWindow_Separate

//////////////////////////////////////////////////////////////////////////////////

#define IMUWindow_Attitude_MOUSESIZE 		2

#define IMUWindow_Attitude_SIZE 				160
#define IMUWindow_Attitude_POSY 				IMUWindow_DATA_POSY + IMUWindow_Separate
#define IMUWindow_Attitude_POSX 				320 - IMUWindow_Attitude_SIZE -IMUWindow_Separate - IMUWindow_Attitude_MOUSESIZE*2

#define IMUWindow_Attitude_MOUSESCALE 	(IMUWindow_Attitude_SIZE/2 - IMUWindow_Attitude_MOUSESIZE-1)/90
#define IMUWindow_Attitude_MOUSESPOSX 	IMUWindow_Attitude_POSX + IMUWindow_Attitude_SIZE/2
#define IMUWindow_Attitude_MOUSESPOSY 	IMUWindow_Attitude_POSY + IMUWindow_Attitude_SIZE/2


//////////////////////////////////////////////////////////////////////////////////

#define IMUWindow_SIZE_Confirm_X				200
#define IMUWindow_SIZE_Confirm_Y				100
#define IMUWindow_POS_Confirm_X				(320 - IMUWindow_SIZE_Confirm_X)/2
#define IMUWindow_POS_Confirm_Y				(240 - IMUWindow_SIZE_Confirm_Y)/2



#define IMU_FREQUENCY 20

void IMU_Window (void);
EulerAngle IMU_Process(uint8_t cmd, IMU_Status *State);
int16_t IMU_FixAngle(float in);
