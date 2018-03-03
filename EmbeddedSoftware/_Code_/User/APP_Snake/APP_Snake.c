#include "APP_Snake.h"
#include "TFTService_Text.h"
#include "TFTService_Draw.h"
#include "TFTService_Button.h"
#include "TFT_SSD1289.h"
#include "Input.h"
#include "Voltage_FB.h"
#include "Bluetooth.h"
#include "TFTService_Dialog.h"
#include "APP_IMU.h"
#include "Timing_Manage.h"

#include <stdio.h>
#include <stdlib.h>


typedef enum 
{
	Snake_BUTTON_Min				= 0 ,
	Snake_BUTTON_Newgame 		= 0 ,
	Snake_BUTTON_Tilt 			= 1 ,
	Snake_BUTTON_Exit 			= 2 ,
	Snake_BUTTON_Max				= 3 ,
}Snake_BUTTON;


static char *SnakeButtonString[Snake_BUTTON_Max][2] = 
{	
	" New Game ",	" New Game ",	
	" Tilt On  ",	" Tilt Off ",	
	"   Exit   ",	"   Exit   "	
};



typedef enum 
{
	Snake_MATRIX_Head 		= 0,	
	Snake_MATRIX_Body 		= 1,	
	Snake_MATRIX_Tail 		= 2,
	Snake_MATRIX_Food 		= 3,
	Snake_MATRIX_Free 		= 4,
	Snake_MATRIX_Obstacle = 5,
}Snake_MATRIX;

typedef enum 
{
	Snake_DIR_forward,	
	Snake_DIR_right,	
}Snake_DIR;

static Snake_MATRIX Snake_Game[SNAKE_Matrix][SNAKE_Matrix];
static int16_t Snake_Map[SNAKE_Lenght_Max][2];
static uint16_t Snake_lenght; 
static float Snake_speed, Snake_dt;
static uint64_t Snake_Lastime;
static TFTService_Button Snake_Button[Snake_BUTTON_Max];
static EulerAngle Snake_TiltAngle;
static int16_t Snake_TiltX, Snake_TiltY;
 int8_t DIRx=0, DIRy=0;
static uint16_t Snake_point;

void Snake_PutCell(uint16_t X, uint16_t Y)
{
	uint16_t x = SNAKE_POS_Matrix_X + X * SNAKE_Width_Max  + SNAKE_Width_Max/2;
	uint16_t y = SNAKE_POS_Matrix_Y + Y * SNAKE_Width_Max  + SNAKE_Width_Max/2;
	switch (Snake_Game[X][Y])
	{
		case Snake_MATRIX_Head:			TFT_DrawFullCircle (	SNAKE_Head, x , y, SNAKE_Width_Max/2); break;	
		case Snake_MATRIX_Body: 		TFT_DrawFullCircle (	SNAKE_Body, x , y, SNAKE_Width_Max/2); break;	
		case Snake_MATRIX_Tail:			TFT_DrawFullCircle (	SNAKE_Tail, x , y, SNAKE_Width_Max/2); break;	
		case Snake_MATRIX_Food:			TFT_DrawFullCircle (	SNAKE_Food, x , y, SNAKE_Width_Max/2); break;	
		case Snake_MATRIX_Free:			TFT_DrawFullCircle (	SNAKE_Free, x , y, SNAKE_Width_Max/2); break;	
		case Snake_MATRIX_Obstacle:	
																x -= SNAKE_Width_Max/2;
																y -= SNAKE_Width_Max/2;
																TFT_DrawFullRect (	SNAKE_OBST, x , y, SNAKE_Width_Max, SNAKE_Width_Max); 
																break;	
	}
}
void Snake_UpdatePoint(void)
{	
	Snake_point++;
	TFT_Text_Background (SNAKE_POS_Title_X - 2, SNAKE_POS_Title_Y+30, ASCII_16x24,	TFT_BLACK, SNAKE_BACKGROUND, " %03d ",Snake_point);
	TFT_Text_Transparent(SNAKE_POS_Title_X - 1, SNAKE_POS_Title_Y+30, ASCII_16x24,	TFT_BLACK,  " %03d",Snake_point);
	TFT_Text_Transparent(SNAKE_POS_Title_X + 0, SNAKE_POS_Title_Y+30, ASCII_16x24,	TFT_BLACK,  " %03d",Snake_point);
	TFT_Text_Transparent(SNAKE_POS_Title_X + 1, SNAKE_POS_Title_Y+30, ASCII_16x24,	TFT_BLACK,  " %03d",Snake_point);
	TFT_Text_Transparent(SNAKE_POS_Title_X + 2, SNAKE_POS_Title_Y+30, ASCII_16x24,	TFT_BLACK,  " %03d",Snake_point);
}
int8_t Snake_UpdateSnake(int16_t NewX, int16_t NewY, uint8_t Isgrow)
{
	uint16_t cnt=0, tmpX=NewX, tmpY=NewY;
	
	if (Isgrow == 1)		Snake_Game[NewX][NewY] = Snake_MATRIX_Food;
	else 
	{
		if (Get_msTick() - Snake_Lastime < (uint64_t)Snake_dt) return 0;
		else 	Snake_Lastime = Get_msTick();
	}
	
	//For hitting boundary
	if (NewX<0 || NewY<0 || NewX>=SNAKE_Matrix || NewY>=SNAKE_Matrix) return -1;
		
	switch (Snake_Game[NewX][NewY])
	{
		case Snake_MATRIX_Head: 		return 0;
		case Snake_MATRIX_Body: 		/////////////////////////////
																if (Snake_Map[Snake_lenght-2][0] == NewX && Snake_Map[Snake_lenght-2][1] == NewY) 	return 0;	 // in case go backward
																else																																								return -1; // in case eat body
		case Snake_MATRIX_Tail: 		/////////////////////////////		
		case Snake_MATRIX_Free: 		/////////////////////////////
																Snake_Game[Snake_Map[0][0]][Snake_Map[0][1]] = Snake_MATRIX_Free;
																Snake_PutCell(Snake_Map[0][0],Snake_Map[0][1]);
																Snake_Map[Snake_lenght][0] = NewX;
																Snake_Map[Snake_lenght][1] = NewY;
																for (cnt=0; cnt < Snake_lenght; cnt++)
																{
																	tmpX = Snake_Map[cnt+1][0];
																	tmpY = Snake_Map[cnt+1][1];
																	Snake_Map[cnt][0] = tmpX;
																	Snake_Map[cnt][1] = tmpY;
																	Snake_Game[tmpX][tmpY] = Snake_MATRIX_Body;														
																	Snake_PutCell(tmpX, tmpY);	
																}		
																Snake_PutCell(Snake_Map[Snake_lenght-2][0],Snake_Map[Snake_lenght-2][1]);																
																break;																																								// in case eat Tail
		case Snake_MATRIX_Food: 		/////////////////////////////																	
																if (Snake_speed<SNAKE_Speed_Max)		
																{
																	Snake_speed += SNAKE_Speed_delta;
																	Snake_dt = 1.0f/ Snake_speed;
																}
																if (Snake_lenght>=SNAKE_Lenght_Max)  //if max lenght, stop growth
																{
																	Snake_Game[NewX][NewY] = Snake_MATRIX_Free;	
																	Snake_Lastime = Snake_Lastime - (uint64_t)Snake_dt*2;
																	Snake_UpdateSnake(NewX, NewY,  0);
																}
																else
																{
																	if (Snake_lenght>0)
																	{
																		Snake_Game[Snake_Map[Snake_lenght-1][0]][Snake_Map[Snake_lenght-1][1]] = Snake_MATRIX_Body;																	
																		Snake_PutCell(Snake_Map[Snake_lenght-1][0],Snake_Map[Snake_lenght-1][1]);
																	}
																	Snake_Map[Snake_lenght][0] = NewX;
																	Snake_Map[Snake_lenght][1] = NewY;														
																	Snake_lenght++;	
																}
																//add food
																while (cnt==0)
																{
																	tmpX = rand() % (SNAKE_Matrix-1);
																	tmpY = rand() % (SNAKE_Matrix-1);
																	if (Snake_Game[tmpX][tmpY] == Snake_MATRIX_Free && (tmpX!=NewX || tmpY!=NewY))
																		cnt =1;
																}
																Snake_Game[tmpX][tmpY] = Snake_MATRIX_Food;
																Snake_PutCell(tmpX, tmpY);
																Snake_UpdatePoint();
																break;
		case Snake_MATRIX_Obstacle: return -1;		
	}
	
		Snake_Game[Snake_Map[Snake_lenght-1][0]][Snake_Map[Snake_lenght-1][1]] = Snake_MATRIX_Head;
		Snake_Game[Snake_Map[0][0]][Snake_Map[0][1]] = Snake_MATRIX_Tail;
		
		Snake_PutCell(Snake_Map[Snake_lenght-1][0],Snake_Map[Snake_lenght-1][1]);	
		Snake_PutCell(Snake_Map[0][0], Snake_Map[0][1]);		
		return 1;
}

int8_t Snake_ProcessInput(int8_t *x, int8_t *y)
{	
	int16_t	JLX, JLY, JRX, JRY;	
	JLX = Joystick_AX();
	JLY = Joystick_AY();
	JRX = Joystick_BX();
	JRY = Joystick_BY();
	
	if			(ButtonA_Left()  	&& ButtonA_Up())					return 0;	//up-left
	else if (ButtonA_Right() 	&& ButtonA_Up())					return 0;		//up-right
	else if (ButtonA_Left()  	&& ButtonA_Down())				return 0;		//down-left
	else if (ButtonA_Right() 	&& ButtonA_Down())				return 0;			//down-right
	else if (ButtonB_Left()  	&& ButtonB_Up())					return 0;		//up-left
	else if (ButtonB_Right() 	&& ButtonB_Up())					return 0;		//up-right
	else if (ButtonB_Left() 	&& ButtonB_Down())				return 0;		//down-left
	else if (ButtonB_Right() 	&& ButtonB_Down())				return 0;			//down-right
	else if (ButtonA_Up() 		|| ButtonB_Up() ) 				{*y = -1; *x = 0;}	//up
	else if (ButtonA_Down() 	|| ButtonB_Down()) 				{*y = 1; *x = 0;}		//down
	else if (ButtonA_Left()  	|| ButtonB_Left())				{*y = 0; *x = -1;}	//left
	else if (ButtonA_Right() 	|| ButtonB_Right())				{*y = 0; *x = 1;}		//right
	else if (abs(JLX)>2 || abs(JLY)>2) 
	{
		if 			(JLY>2) 	*y = -1;	//up
		else if (JLY<-2) 	*y =  1;	//down	
		if 			(JLX<-2) 	*x = -1;	//left
		else if (JLX>2) 	*x =  1;	//right
		if 			(abs(JLX)>abs(JLY))		*y = 0;
		else 													*x = 0;
		return 1;
	}
	else if (abs(JRX)>2 || abs(JRY)>2) 
	{
		if 			(JRY>2) 	*y = -1;	//up
		else if (JRY<-2) 	*y =  1;	//down	
		if 			(JRX<-2) 	*x = -1;	//left
		else if (JRX>2) 	*x =  1;	//right
		if 			(abs(JRX)>abs(JRY))		*y = 0;
		else 													*x = 0;
		return 1;
	}
	else return 0;
	
	return 0;
}


int8_t Snake_ProcessTilt (int16_t thrs, int8_t *x, int8_t *y)
{
	IMU_Status State;
	Snake_TiltAngle = IMU_Process(0,&State);
	if (State == IMU_OK)
	{
		Snake_TiltX = IMU_FixAngle(Snake_TiltAngle.Roll);
		Snake_TiltY = IMU_FixAngle(Snake_TiltAngle.Pitch);
				
		if (abs(Snake_TiltX)>thrs || abs(Snake_TiltY)>thrs)
		{			
			if 			(Snake_TiltX<-thrs) 	*x = 1;	
			else if (Snake_TiltX>thrs) 		*x = -1;
			
			if 			(Snake_TiltY<-thrs) 	*y = -1;
			else if (Snake_TiltY>thrs) 		*y = 1;
			if 			(abs(Snake_TiltX)>abs(Snake_TiltY))		*y = 0;
			else 																					*x = 0;
		}
		return 1;
	}
	else return 0;
}

void Snake_Generate(int16_t Obst, int16_t Food,  float Speed)
{	
	uint16_t x=0, y=0;
	int16_t cnt, signx, signy;	
	
	Snake_speed = Speed;
	Snake_dt = 1/Snake_speed;
	
	//all are free
	for (x=0;x<SNAKE_Matrix;x++)
		for (y=0;y<SNAKE_Matrix;y++)
			Snake_Game[x][y] = Snake_MATRIX_Free;
	
	//snake
	x=SNAKE_Matrix/2-1; 
	y=SNAKE_Matrix/2;
	Snake_lenght = 0;
	
	Snake_UpdateSnake(x++,y,1);
	Snake_UpdateSnake(x++,y,1);
	Snake_UpdateSnake(x++,y,1);
	
	//Obstacle
	while (Obst>0)
	{
		cnt = rand() % SNAKE_Obst_Max;
		x = rand() % (SNAKE_Matrix-1);
		y = rand() % (SNAKE_Matrix-1);
		if (Snake_Game[x][y] == Snake_MATRIX_Free )
		{
			if (rand()%2 == 1) signx =-1;
				else signx =1;
			if (rand()%2 == 1) signy =-1;
				else signy =1;
				
			while (--cnt>0)
			{
				Snake_Game[x][y] = Snake_MATRIX_Obstacle;
				
				if (rand()%2 == 1)
				{
					if ((x+signx < SNAKE_Matrix-1) && (x+signx > 0) && (Snake_Game[x+signx][y] == Snake_MATRIX_Free))
						x+= signx;
					else		
						cnt = 0;		
				}
				else
				{
					if ((y+signy < SNAKE_Matrix-1) && (y+signy > 0) && (Snake_Game[x][y+signy] == Snake_MATRIX_Free))
						y+= signy;
					else		
						cnt = 0;		
				}	
			}
			Obst --;
		}
	}
	
	//Food
	while (Food-->0)
	{
		x = rand() % (SNAKE_Matrix-1);
		y = rand() % (SNAKE_Matrix-1);
		if (Snake_Game[x][y] == Snake_MATRIX_Free )
		Snake_Game[x][y] = Snake_MATRIX_Food;
	}
	
	Snake_point = 0;
  DIRx = DIRy = 0;
}

void Snake_Render(void)
{
	uint16_t x, y, cnt;	
	
  TFT_Clear(SNAKE_BACKGROUND);	
	
	TFT_Text_Background(SNAKE_POS_Title_X    , SNAKE_POS_Title_Y, ASCII_16x24,	TFT_BLUE,SNAKE_Theme_Title, "Snake");
	TFT_Text_Transparent(SNAKE_POS_Title_X + 1, SNAKE_POS_Title_Y, ASCII_16x24,	TFT_BLUE, "Snake");
	TFT_Text_Transparent(SNAKE_POS_Title_X + 2, SNAKE_POS_Title_Y, ASCII_16x24,	TFT_BLUE, "Snake");
	TFT_Text_Transparent(290, 225, ASCII_08x12,	TFT_BLACK, "VND");	
	TFT_Text_Transparent(291, 225, ASCII_08x12,	TFT_BLACK, "VND");	
	
	//Draw border	
	TFT_DrawRect (	SNAKE_Theme_Board, SNAKE_POS_Board_X , SNAKE_POS_Board_Y, SNAKE_SIZE_Board, SNAKE_SIZE_Board);	
	TFT_DrawRect (	SNAKE_Theme_Board, SNAKE_POS_Board_X+1 , SNAKE_POS_Board_Y+1, SNAKE_SIZE_Board-2, SNAKE_SIZE_Board-2);	
	TFT_DrawRect (	SNAKE_Theme_Board, SNAKE_POS_Board_X+2 , SNAKE_POS_Board_Y+2, SNAKE_SIZE_Board-4, SNAKE_SIZE_Board-4);	
		
	//Put Button	
	for(cnt = (Snake_BUTTON)Snake_BUTTON_Min; cnt < (Snake_BUTTON)Snake_BUTTON_Max; cnt++)		
		TFTSevice_Button_08x16(&Snake_Button[cnt]);			
		
	for(cnt = (Snake_BUTTON)Snake_BUTTON_Min; cnt<(Snake_BUTTON)Snake_BUTTON_Max; cnt++)			
		{
			if (Snake_ProcessTilt(30, &DIRx, &DIRy))
			{
				Snake_Button[cnt].Normal.Text 				= SnakeButtonString[cnt][0];
				Snake_Button[cnt].Selected.Text 			= SnakeButtonString[cnt][0];
				Snake_Button[cnt].Disable.Text 				= SnakeButtonString[cnt][0];
			}
			else
			{				
				Snake_Button[cnt].Normal.Text 				= SnakeButtonString[cnt][1];
				Snake_Button[cnt].Selected.Text 			= SnakeButtonString[cnt][1];
				Snake_Button[cnt].Disable.Text 				= SnakeButtonString[cnt][1];
			}
			TFTSevice_Button_08x16(&Snake_Button[cnt]);	
		}
		
	//Put Game
	for (x=0;x<SNAKE_Matrix;x++)
		for (y=0;y<SNAKE_Matrix;y++)
			Snake_PutCell(x,y);
	
	//Put Point		
	TFT_Text_Background (SNAKE_POS_Title_X - 2, SNAKE_POS_Title_Y+30, ASCII_16x24,	TFT_BLACK, SNAKE_BACKGROUND, " %03d ",Snake_point);
	TFT_Text_Transparent(SNAKE_POS_Title_X - 1, SNAKE_POS_Title_Y+30, ASCII_16x24,	TFT_BLACK,  " %03d",Snake_point);
	TFT_Text_Transparent(SNAKE_POS_Title_X + 0, SNAKE_POS_Title_Y+30, ASCII_16x24,	TFT_BLACK,  " %03d",Snake_point);
	TFT_Text_Transparent(SNAKE_POS_Title_X + 1, SNAKE_POS_Title_Y+30, ASCII_16x24,	TFT_BLACK,  " %03d",Snake_point);
	TFT_Text_Transparent(SNAKE_POS_Title_X + 2, SNAKE_POS_Title_Y+30, ASCII_16x24,	TFT_BLACK,  " %03d",Snake_point);
}

uint8_t Snake_ButtonEvent(Snake_BUTTON cnt)
{
	switch (cnt)
	{
		case Snake_BUTTON_Newgame: 
																		Snake_Generate(SNAKE_OBST_num, SNAKE_FOOD_num, SNAKE_Speed_Min);
																		Snake_Render();																		
																		return 1;		
		case Snake_BUTTON_Tilt: 
																		IMU_Window(); 
																		Snake_Render();
																		return 1;		
		case Snake_BUTTON_Exit:					return 0;	
		default: 												return 1;	
	}
}

uint8_t Snake_ButtonProcess(void)
{			
	static int16_t touchX, touchY; 	
	Snake_BUTTON cnt;
	
	TouchFinger Shellmo_event = TouchPanel_GetPoint(&touchX,&touchY);	
	switch (Shellmo_event)
	{
		case TouchFinger_None: 	
			return 1;
		case TouchFinger_Up: 					
						for (cnt = Snake_BUTTON_Min; cnt < Snake_BUTTON_Max; cnt++)
							if (TFTSevice_RegionEvent(&Snake_Button[cnt], touchX, touchY, Shellmo_event))
							{
								TFTSevice_Button_08x16(&Snake_Button[cnt]);			
								return Snake_ButtonEvent (cnt);	
							}
		default:								
						for (cnt = Snake_BUTTON_Min; cnt < Snake_BUTTON_Max; cnt++)
							if (TFTSevice_RegionEvent(&Snake_Button[cnt], touchX, touchY, Shellmo_event))
								TFTSevice_Button_08x16(&Snake_Button[cnt]);			
	}
	return 1;
}

void Snake_Window (void)
{	
	uint16_t posx, posy, cnt;
	
	//Init Button
	posx = SNAKE_POS_Button_X; 
	posy = SNAKE_POS_Button_Y; 
	for(cnt = (Snake_BUTTON)Snake_BUTTON_Min; cnt < (Snake_BUTTON)Snake_BUTTON_Max; cnt++)		
	{ 
			Snake_Button[cnt].MinX = posx;
			Snake_Button[cnt].MinY = posy;
			Snake_Button[cnt].MaxX = Snake_Button[cnt].MinX + SNAKE_SIZE_Button_X;
			Snake_Button[cnt].MaxY = Snake_Button[cnt].MinY + SNAKE_FONTSize_Button_Y;	
			
			posy = Snake_Button[cnt].MaxY + SNAKE_Separate_Button; 		
			
			Snake_Button[cnt].Normal.ForeColor 		= SNAKE_ForeColor_ButtonNormal;
			Snake_Button[cnt].Normal.BackColor 		= SNAKE_BackColor_ButtonNormal;
			Snake_Button[cnt].Normal.Text 				= SnakeButtonString[cnt][0];
			Snake_Button[cnt].Selected.ForeColor 	= SNAKE_ForeColor_ButtonSelected;
			Snake_Button[cnt].Selected.BackColor 	= SNAKE_BackColor_ButtonSelected;
			Snake_Button[cnt].Selected.Text 			= SnakeButtonString[cnt][0];
			Snake_Button[cnt].Disable.ForeColor 	= SNAKE_ForeColor_ButtonSelected;
			Snake_Button[cnt].Disable.BackColor 	= SNAKE_BackColor_ButtonSelected;
			Snake_Button[cnt].Disable.Text 				= SnakeButtonString[cnt][0];			
	}
	
	Snake_Generate(SNAKE_OBST_num, SNAKE_FOOD_num, SNAKE_Speed_Min);
	Snake_Render();
	while (Snake_ButtonProcess())
	{		
			if (!Snake_ProcessInput(&DIRx, &DIRy))				
				Snake_ProcessTilt(15, &DIRx, &DIRy); 		
			if (Snake_UpdateSnake(Snake_Map[Snake_lenght-1][0] + DIRx, Snake_Map[Snake_lenght-1][1] + DIRy, 0) == -1)
			{				
				TFTSevice_ShowDialogOK(
																SNAKE_POS_Confirm_X, SNAKE_POS_Confirm_Y, 
																SNAKE_SIZE_Confirm_X, SNAKE_SIZE_Confirm_Y, 
																"     Snake Game", " \n GAME OVER !"
															);
													
				Snake_Generate(SNAKE_OBST_num, SNAKE_FOOD_num, SNAKE_Speed_Min);
				Snake_Render();
			}
		
	}	
}
