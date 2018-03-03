#include "TFT_SSD1289.h"
#include "AsciiLib.h"
#include "TFTService_Text.h"
#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h> 

static uint16_t 					TFT_Text_PosX 	= 0;
static uint16_t 					TFT_Text_PosY 	= 0;

static uint16_t 					TFT_Text_ForeColor 	= TFT_YELLOW;
static uint16_t 					TFT_Text_BackColor 	= TFT_GREEN;

static uint16_t 					TFT_Text_CharSizeX 	= 8;
static uint16_t 					TFT_Text_CharSizeY 	= 8;

static uint16_t 					TFT_Text_LimitXMin, TFT_Text_LimitXMax,	TFT_Text_LimitYMin, TFT_Text_LimitYMax;

static LCDTEXT_FONTMODE 	TFT_Text_FontMode		= LCDTEXT_Transparent;
static LCDTEXT_FONT 			TFT_Text_Font				=	ASCII_08x12;

static uint16_t	TFT_Text_Index=0;
static int32_t TFT_Text_Buffer[TFT_Text_LinebufferSize];

static void TFT_Text_SetFont(uint16_t posX, uint16_t posY, LCDTEXT_FONT font, uint16_t Fore,  uint16_t Back)
{
	TFT_Text_Font = font;
	TFT_Text_ForeColor 	= Fore;
	TFT_Text_BackColor 	= Back;
	TFT_Text_PosX = posX;	
	TFT_Text_PosY = posY;	
		
	switch (TFT_Text_Font)
	{
		case ASCII_16x24:	////////////////////////////////
						TFT_Text_CharSizeX = 16;
						TFT_Text_CharSizeY = 24;
						break;
		case ASCII_08x16:	////////////////////////////////
						TFT_Text_CharSizeX = 8;
						TFT_Text_CharSizeY = 16;
						break;
		case ASCII_08x12:	////////////////////////////////
						TFT_Text_CharSizeX = 8;
						TFT_Text_CharSizeY = 12;
						break;			
	}
}

static uint8_t TFT_Text_AsciiMask (uint8_t CHAR,  uint8_t x, uint8_t y)
{	
		switch (TFT_Text_Font)
		{
			case ASCII_16x24:		return(((ASCII16x24[CHAR-32][y-1] >> 	 x ) & 0x01) == 0x01) ;
			case ASCII_08x16:		return(((ASCII8x16[CHAR-32][y] >> (7-x)) & 0x01) == 0x01) ;
			case ASCII_08x12:		return(((ASCII8x12[CHAR-32][y] >> (7-x)) & 0x01) == 0x01) ;
			default 				: 	return(((ASCII8x12[CHAR-32][y] >> (7-x)) & 0x01) == 0x01) ;
		}
}

//void TFT_Text_TestFONT ()
//{
//	char c = 'a';
//  uint8_t x = 0;
//  uint8_t y = 0;
//	TFT_Text_SetFont(0, 0, ASCII_16x24,  BLACK,	GREEN);	
//	TFT_Text_FontMode		= LCDTEXT_BackGround;
//	
//	while (c < 'z')
//	{
//		for (y=0; y<TFT_Text_CharSizeY; y++)
//		{		
//			for (x=0; x<TFT_Text_CharSizeX; x++)
//			{
//				if (TFT_Text_AsciiMask(c, x, y))
//					Pixel(TFT_Text_PosX +x, TFT_Text_PosY + y, TFT_Text_ForeColor); 
//					else if (	TFT_Text_FontMode		!= LCDTEXT_Transparent)
//						Pixel(TFT_Text_PosX + x, TFT_Text_PosY + y, TFT_Text_BackColor);
//			}
//		}
//		// check cursor
//		TFT_Text_PosX += TFT_Text_CharSizeX+1;	
//		if (TFT_Text_PosX > TFT_Width)
//		{
//				TFT_Text_PosX = 	0;
//				TFT_Text_PosY += TFT_Text_CharSizeY+1;
//				if (TFT_Text_PosY > TFT_Height)		 
//					TFT_Text_PosX = 0;
//		}
//		c++;
//	}
//}

void TFT_Text_ShiftCursor(int x, int y)
{
	TFT_Text_PosX += x;
	TFT_Text_PosY += y;
}

void TFT_Text_SetCursor(int x, int y)
{
	TFT_Text_PosX = x;
	TFT_Text_PosY = y;
}

int32_t TFT_Text_PrintChar(int posx, int posy, int32_t ch) 
{	
  uint8_t x = 0;
  uint8_t y = 0;
	if (ch ==0 )							return ch;
	else if (ch == '\t' )
	{
		
	}
	else
		for (y=0; y<TFT_Text_CharSizeY; y++)
		{		
			for (x=0; x<TFT_Text_CharSizeX; x++)
			{
				if (TFT_Text_AsciiMask(ch, x, y))
					TFT_SetPixelSafe(posx +x, posy + y, TFT_Text_ForeColor); 
					else if (	TFT_Text_FontMode		!= LCDTEXT_Transparent)
						TFT_SetPixelSafe(posx + x, posy + y, TFT_Text_BackColor);
			}
		}
	return ch;
}
void TFT_Text_StringProcess(void)
{	
	uint16_t wordcount = 0, CheckIndex = 0, PosX = TFT_Text_LimitXMin, PosY = TFT_Text_LimitYMin;
	
	while (TFT_Text_PrintChar(PosX, PosY, TFT_Text_Buffer[CheckIndex++]))
	{
		while (TFT_Text_Buffer [CheckIndex] == '\n')	//new line case
		{
			wordcount = 0;
			PosX = TFT_Text_LimitXMin -  TFT_Text_CharSizeX;
			PosY += TFT_Text_CharSizeY;
			if (PosY + TFT_Text_CharSizeY > TFT_Text_LimitYMax)		 		
				TFT_Text_Buffer [CheckIndex] = 0;
			else CheckIndex++;
		}
		
		if (TFT_Text_Buffer [CheckIndex])
		{
			PosX += TFT_Text_CharSizeX;				
			
			if (TFT_Text_Buffer [CheckIndex] == ' ') wordcount++;
				
			if (PosX + TFT_Text_CharSizeX > TFT_Text_LimitXMax)		//over width limit == > new line
			{				
				if (TFT_Text_Buffer [CheckIndex] != ' ' && wordcount)
				{
					while ((TFT_Text_Buffer [CheckIndex] != ' ') && (TFT_Text_Buffer [CheckIndex] != '\n'))
					{
						CheckIndex -- ;
						PosX -= TFT_Text_CharSizeX;
						TFT_Text_PrintChar(PosX, PosY, ' ');
					}					
				}					
				
					
				PosX = 	TFT_Text_LimitXMin;
				PosY += TFT_Text_CharSizeY;
				wordcount = 0;
				
				while ((TFT_Text_Buffer [CheckIndex] == ' ') && (TFT_Text_Buffer [CheckIndex])) 
					CheckIndex++;				
					
				if (PosY + TFT_Text_CharSizeY > TFT_Text_LimitYMax)		 		
					TFT_Text_Buffer [CheckIndex] = 0;			
			}
		}
	}	
}

int fputc(int ch, FILE *f)
{	
	TFT_Text_Buffer [TFT_Text_Index++] = ch;
  return ch;
}

void TFT_Text_Background(int16_t posX, int16_t posY, LCDTEXT_FONT font, uint16_t Fore,  uint16_t Back, const char *fmt, ...) 
{	
	va_list args;		
	TFT_Text_SetFont (posX, posY, font, Fore,  Back);
	TFT_Text_FontMode		= LCDTEXT_BackGround;
	TFT_Text_LimitXMin = posX;
	TFT_Text_LimitYMin = posY;
	TFT_Text_LimitXMax = posX + TFT_Width;
	TFT_Text_LimitYMax = posY + TFT_Height;
	//Copy Content
	TFT_Text_Index = 0;
	va_start(args,fmt);
	vprintf(fmt,args);
	va_end(args);		
	TFT_Text_Buffer [TFT_Text_Index] = 0;
	
	//Process Content
	TFT_Text_StringProcess();
}


void TFT_Text_Transparent(int16_t posX, int16_t posY, LCDTEXT_FONT font, uint16_t Fore, const char *fmt, ...) 
{	
	va_list args;		
	
	TFT_Text_SetFont (posX, posY, font, Fore,  Fore);
	TFT_Text_FontMode		= LCDTEXT_Transparent;
	TFT_Text_LimitXMin = posX;
	TFT_Text_LimitYMin = posY;
	TFT_Text_LimitXMax = posX + TFT_Width;
	TFT_Text_LimitYMax = posY + TFT_Height;
	//Copy Content
	TFT_Text_Index = 0;
	va_start(args,fmt);
	vprintf(fmt,args);
	va_end(args);		
	TFT_Text_Buffer [TFT_Text_Index] = 0;
	
	//Process Content
	TFT_Text_StringProcess();
}

void TFT_Text_Background_Limit(int16_t posX, int16_t posY, int16_t width, int16_t height, LCDTEXT_FONT font, uint16_t Fore,  uint16_t Back, const char *fmt, ...) 
{	
	va_list args;		
	TFT_Text_SetFont (posX, posY, font, Fore,  Back);
	TFT_Text_FontMode		= LCDTEXT_BackGround;
	TFT_Text_LimitXMin = posX;
	TFT_Text_LimitYMin = posY;
	TFT_Text_LimitXMax = posX + width;
	TFT_Text_LimitYMax = posY + height;
	//Copy Content
	TFT_Text_Index = 0;
	va_start(args,fmt);
	vprintf(fmt,args);
	va_end(args);		
	TFT_Text_Buffer [TFT_Text_Index] = 0;	
	
	//Process Content
	TFT_Text_StringProcess();
}


void TFT_Text_Transparent_Limit(uint16_t posX, uint16_t posY,uint16_t width, uint16_t height, LCDTEXT_FONT font, uint16_t Fore, const char *fmt, ...) 
{	
	va_list args;		
	
	TFT_Text_SetFont (posX, posY, font, Fore,  Fore);
	TFT_Text_FontMode		= LCDTEXT_Transparent;
	TFT_Text_LimitXMin = posX;
	TFT_Text_LimitYMin = posY;
	TFT_Text_LimitXMax = posX + width;
	TFT_Text_LimitYMax = posY + height;
	//Copy Content
	TFT_Text_Index = 0;
	va_start(args,fmt);
	vprintf(fmt,args);
	va_end(args);		
	TFT_Text_Buffer [TFT_Text_Index] = 0;
	
	
	//Process Content
	TFT_Text_StringProcess();
}
