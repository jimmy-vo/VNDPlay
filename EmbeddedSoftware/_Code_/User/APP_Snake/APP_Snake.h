#include "stm32f4xx.h"


//////////////////////////////////////////////////////////////////////////////////

#define SNAKE_Width_Max										5
#define SNAKE_Lenght_Max									512

#define SNAKE_FOOD_num										10

#define SNAKE_OBST_num										50
#define SNAKE_Obst_Max										10

#define SNAKE_Speed_Max										0.2
#define SNAKE_Speed_delta									0.0002f
#define SNAKE_Speed_Min										0.002f

//////////////////////////////////////////////////////////////////////////////////

#define SNAKE_Separate								1

//////////////////////////////////////////////////////////////////////////////////
#define SNAKE_Matrix									45

#define SNAKE_SIZE_Matrix							(SNAKE_Matrix * SNAKE_Width_Max) 
#define SNAKE_POS_Matrix_Y						(240 - SNAKE_SIZE_Matrix)/2 
#define SNAKE_POS_Matrix_X						5

//////////////////////////////////////////////////////////////////////////////////
	

#define SNAKE_POS_Board_X							SNAKE_POS_Matrix_X - 4
#define SNAKE_POS_Board_Y							SNAKE_POS_Matrix_Y - 4

#define SNAKE_SIZE_Board							SNAKE_SIZE_Matrix + 4*2
//////////////////////////////////////////////////////////////////////////////////


#define SNAKE_POS_Title_X							SNAKE_SIZE_Board + SNAKE_Separate*4
#define SNAKE_POS_Title_Y							0 

#define SNAKE_SIZE_Title_X						100
#define SNAKE_SIZE_Title_Y						60

//////////////////////////////////////////////////////////////////////////////////

#define SNAKE_Separate_Button					10
#define SNAKE_POS_Button_X						SNAKE_POS_Title_X 
#define SNAKE_POS_Button_Y						SNAKE_POS_Title_Y +  SNAKE_SIZE_Title_Y + 25
	
#define SNAKE_FONT_Button							ASCII_08x16 
#define SNAKE_FONTSize_Button_X				8
#define SNAKE_FONTSize_Button_Y				16

#define SNAKE_SIZE_Button_X						SNAKE_FONTSize_Button_X * 6
#define SNAKE_SIZE_Button_Y						SNAKE_FONTSize_Button_Y * 6 + SNAKE_Separate * 4

//////////////////////////////////////////////////////////////////////////////////
#define SNAKE_SIZE_Confirm_X			160
#define SNAKE_SIZE_Confirm_Y			80
#define SNAKE_POS_Confirm_X				(320 - SNAKE_SIZE_Confirm_X)/2
#define SNAKE_POS_Confirm_Y				(240 - SNAKE_SIZE_Confirm_Y)/2

//////////////////////////////////////////////////////////////////////////////////
// Color
//
#define	SNAKE_BACKGROUND											TFT_WHITE

#define	SNAKE_Theme_Title										TFT_BLACK
#define	SNAKE_Theme_Board										TFT_NAVY


#define	SNAKE_Head												TFT_DCYAN+4
#define	SNAKE_Body												TFT_DCYAN 
#define	SNAKE_Tail												TFT_DCYAN-4
#define	SNAKE_Free												SNAKE_BACKGROUND
#define	SNAKE_Food												TFT_RED
#define	SNAKE_OBST												TFT_BLACK
/////////////////////////

#define	SNAKE_ForeColor_ButtonNormal					TFT_BLACK
#define	SNAKE_BackColor_ButtonNormal					TFT_LGRAY

#define	SNAKE_ForeColor_ButtonSelected				TFT_BLACK
#define	SNAKE_BackColor_ButtonSelected				TFT_DGRAY
//////////////////////////

#define	SNAKE_ForeColor_SourceNormal					TFT_RED
#define	SNAKE_BackColor_SourceNormal					TFT_LGRAY

#define	SNAKE_ForeColor_SourceSelected				TFT_RED
#define	SNAKE_BackColor_SourceSelected				TFT_DGRAY

#define	SNAKE_ForeColor_TrueNormal						TFT_BLUE
#define	SNAKE_BackColor_TrueNormal						TFT_LGRAY

#define	SNAKE_ForeColor_TrueSelected					TFT_BLUE
#define	SNAKE_BackColor_TrueSelected					TFT_DGRAY

#define	SNAKE_ForeColor_WrongNormal					TFT_YELLOW
#define	SNAKE_BackColor_WrongNormal					TFT_LGRAY

#define	SNAKE_ForeColor_WrongSelected				TFT_YELLOW
#define	SNAKE_BackColor_WrongSelected				TFT_DGRAY

#define	SNAKE_ForeColor_UserNormal						TFT_BLACK
#define	SNAKE_BackColor_UserNormal						TFT_LGRAY

#define	SNAKE_ForeColor_UserSelected					TFT_LGRAY
#define	SNAKE_BackColor_UserSelected					TFT_DGRAY
///////////////////////////


void Snake_Window (void);
