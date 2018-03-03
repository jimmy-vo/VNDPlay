#include "stm32f4xx.h"


#define DialogTHEMEColor 								TFT_WHITE

#define DialogTitleForeColor 						TFT_WHITE
#define DialogTitleBackColor 						TFT_BLACK

#define DialogContentForeColor 					TFT_BLACK
#define DialogContentBackColor					DialogTHEMEColor

#define	Dialog_ForeColor_ButtonNormal					TFT_BLACK
#define	Dialog_BackColor_ButtonNormal					TFT_LGRAY

#define	Dialog_ForeColor_ButtonSelected				TFT_BLACK
#define	Dialog_BackColor_ButtonSelected				TFT_DGRAY

#define DialogTitleFONT 								ASCII_08x16
#define DialogTitleFONTX 								8
#define DialogTitleFONTY 								16

#define DialogContentFONT								ASCII_08x16
#define DialogContentFONTX							8
#define DialogContentFONTY							16

#define DialogButtonFONT_								ASCII_08x16 
#define DialogButtonFONTX								8
#define DialogButtonFONTY								16
#define DialogButtonSeparate						5

#define DialogButtonYesSize							DialogButtonFONTX * 5
#define DialogButtonNoSize							DialogButtonFONTX * 4
#define DialogButtonOKSize							DialogButtonFONTX * 4

typedef enum
{
	Dialog_No 	= 0,
	Dialog_Yes 	= 1,
	Dialog_OK 	= 2,
	Dialog_Max = 3,
}TFTSevice_Dialog;

TFTSevice_Dialog	TFTSevice_ShowDialogYesNo (int16_t x, int16_t y, int16_t width, int16_t height, const char * Title, const char *Content);
TFTSevice_Dialog	TFTSevice_ShowDialogOK (int16_t x, int16_t y, int16_t width, int16_t height, const char * Title, const char *Content);
