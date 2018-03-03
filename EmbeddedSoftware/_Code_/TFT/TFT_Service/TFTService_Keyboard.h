#include "stm32f4xx.h"


#define Keyboard_Size 							41

#define Keyboard_RowMax 						4
#define Keyboard_ColMax 						26

#define Keyboard_OUTPUTSIZE  				Keyboard_RowMax*Keyboard_ColMax
///////////////////////////////////////////////////////////////

#define Keyboard_Separate 					8
#define Keyboard_Magin 							4

#define Keyboard_FONT								ASCII_16x24 
#define Keyboard_FONTX							16
#define Keyboard_FONTY							24

#define Keyboard_POSX								0
#define Keyboard_POSY								240 - (Keyboard_FONTY * 5 + Keyboard_Separate * 4 + Keyboard_Magin*2)

///////////////////////////////////////////////////////////////////////

#define KeyboardButton_FONT								ASCII_16x24
#define KeyboardButton_FONTX							16
#define KeyboardButton_FONTY							24

#define KeyboardButton_SIZEX							KeyboardButton_FONTX * 6
#define KeyboardButton_SIZEY							KeyboardButton_FONTY * 2  + Keyboard_Separate/2

#define KeyboardButton_POSX								320 - KeyboardButton_SIZEX - Keyboard_Separate/2
#define KeyboardButton_POSY								KeyboardTitle_FONTY + Keyboard_Separate*4/5

///////////////////////////////////////////////////////////////////////

#define KeyboardTitle_FONT							ASCII_08x16 
#define KeyboardTitle_FONTX							8
#define KeyboardTitle_FONTY							16

#define KeyboardTitle_POSX							0
#define KeyboardTitle_POSY							0

#define KeyboardTitle_SizeX							320
#define KeyboardTitle_SizeY							KeyboardTitle_FONTY
///////////////////////////////////////////////////////////////////////

#define KeyboardText_FONT								ASCII_08x16 
#define KeyboardText_FONTX							8
#define KeyboardText_FONTY							16

#define KeyboardText_POSX								2
#define KeyboardText_POSY								KeyboardTitle_POSY + KeyboardTitle_FONTY

#define KeyboardText_SIZEX							KeyboardText_FONTX * Keyboard_ColMax
#define KeyboardText_SIZEY							KeyboardText_FONTY * Keyboard_RowMax

///////////////////////////////////////////////////////////////////////

#define Keyboard_BACKGROUND								TFT_WHITE
#define Keyboard_THEME 										TFT_BLACK

#define Keyboard_Title_ForeColor					TFT_WHITE
#define Keyboard_Title_BackColor					TFT_BLACK

#define	KeyboardText_ForeColor						TFT_BLACK
#define	KeyboardText_BackColor						Keyboard_BACKGROUND

#define	Keyboard_ButtonForeColorNormal		TFT_BLACK
#define	Keyboard_ButtonBackColorNormal		TFT_LGRAY

#define	Keyboard_ButtonForeColorSelected	TFT_LGRAY
#define	Keyboard_ButtonBackColorSelected	TFT_BLACK

#define	Keyboard_ForeColorNormal					TFT_BLACK
#define	Keyboard_BackColorNormal					TFT_LGRAY

#define	Keyboard_ForeColorSelected				TFT_LGRAY
#define	Keyboard_BackColorSelected				TFT_CYAN

uint8_t TFTService_Keyboard (char* Tittle, char** buff);
int64_t TFTService_Keyboard_StringToInt64 (char* string);
