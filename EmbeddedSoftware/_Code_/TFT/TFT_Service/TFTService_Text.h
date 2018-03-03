#include "stm32f4xx.h"

#define TFT_Text_LinebufferSize 				1024
typedef enum 
{
	LCDTEXT_BackGround,	
	LCDTEXT_Transparent,
}LCDTEXT_FONTMODE;

typedef enum 
{
	ASCII_16x24,
	ASCII_08x16, 
	ASCII_08x12,
}LCDTEXT_FONT;

void TFT_Text_ShiftCursor(int x, int y);
void TFT_Text_SetCursor(int x, int y);
void TFT_Text_Background(int16_t posX, int16_t posY, LCDTEXT_FONT font, uint16_t Fore,  uint16_t Back, const char *fmt, ...) ;
void TFT_Text_Transparent(int16_t posX, int16_t posY, LCDTEXT_FONT font, uint16_t Fore, const char *fmt, ...) ;
void TFT_Text_Background_Limit(int16_t posX, int16_t posY, int16_t width, int16_t height, LCDTEXT_FONT font, uint16_t Fore,  uint16_t Back, const char *fmt, ...) ;
void TFT_Text_Transparent_Limit(uint16_t posX, uint16_t posY,uint16_t width, uint16_t height, LCDTEXT_FONT font, uint16_t Fore, const char *fmt, ...);
