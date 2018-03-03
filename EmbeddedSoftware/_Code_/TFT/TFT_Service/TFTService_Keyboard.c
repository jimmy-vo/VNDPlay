#include "TFTService_Keyboard.h"
#include "TFT_SSD1289.h"
#include "TFTService_Draw.h"
#include "TFTService_Text.h"
#include "TFTService_Button.h"


#define Keyboard_CodeNewline  40
#define Keyboard_CodeDel   		37
#define Keyboard_CodeShift	  29
#define Keyboard_CodeSpec	  	38

typedef struct
{
	uint16_t 						MinX;
	uint16_t 						MaxX;
	uint16_t 						MinY;
	uint16_t 						MaxY;
	uint8_t							IsPressed;
	char								*Capital;
	char								*Normal;
	char								*Special;
}TFTService_Keyboard_Button;

static uint8_t Keyboard_Shift;
static uint8_t Keyboard_Special;
static int8_t Keyboard_Row;
static int8_t Keyboard_Col;
static int8_t Keyboard_Index;

static	TFTService_Keyboard_Button KEY[Keyboard_Size];	
static	TFTService_Button 				 Keyboard_Button[2];	
static	char Keyboard_OUTPUT[Keyboard_OUTPUTSIZE];	

static	char *Keyboard_NormalKey[Keyboard_Size] = 
	{
			"1", "2", "3", "4", "5", "6", "7", "8", "9", "0",
			"q", "w", "e", "r", "t", "y", "u", "i", "o", "p",
		    "a", "s", "d", "f", "g", "h", "j", "k", "l", 
		"Aa", "z", "x", "c", "v", "b", "n", "m",     "<-",  
	   "..",            " ",                  "New Line", 
	};
static	char *Keyboard_CapitalKey[Keyboard_Size] = 
	{
	    "1", "2", "3", "4", "5", "6", "7", "8", "9", "0",
	    "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P",
			   "A", "S", "D", "F", "G", "H", "J", "K", "L",
	 "Aa", "Z", "X", "C", "V", "B", "N", "M",   "<-", 
	   "..",            " ",                  "New Line", 
	};
	
static	char *Keyboard_SpecialKey[Keyboard_Size] = 
	{
	    "1", "2", "3", "4", "5", "6", "7", "8", "9", "0",
	    "!", "@", "#", "$", "%", "^", "&", "*", "(", ")",
			   "?", "-", "=", "_", "+", "[", "]", "{", "}",
	   "Aa", ";", "'", ":", "<", ">", ",", ".",    "<-", 
	   "..",            " ",                  "New Line", 
	};	
static	char *Keyboard_ButtonStr[2] =
{
	" BACK ",
	" DONE ",
};
		
static void TFTSevice_Keyboard_GetPos(int8_t *row, int8_t *col)
{
	uint8_t cnt = 0;
	*row = *col = 0;
	while (Keyboard_OUTPUT[cnt] != '\0')
	{
		if (Keyboard_OUTPUT[cnt] != '\n')
		{			
			if (++*col >= Keyboard_ColMax)  
			{
				col = 0;
				if (++*row >= Keyboard_RowMax)  
				{
					*row = Keyboard_RowMax-1;
					*col = Keyboard_ColMax-1;
				}
			}	
		}
		else
		{			
				*col =0;
				if (++*row >= Keyboard_RowMax)  
				{
					*row = Keyboard_RowMax-1;
					*col = Keyboard_ColMax-1;
				}
		}
		cnt++;
	}
}
static void TFTSevice_Keyboard_Layout (void)
{
	uint16_t cnt, posx, posy;
	
	posx = KeyboardButton_POSX; 
	posy = KeyboardButton_POSY; 
	for(cnt = 0; cnt < 2; cnt++)		
	{ 
			Keyboard_Button[cnt].MinX = posx;
			Keyboard_Button[cnt].MinY = posy;
			Keyboard_Button[cnt].MaxX = Keyboard_Button[cnt].MinX + KeyboardButton_SIZEX;
			Keyboard_Button[cnt].MaxY = Keyboard_Button[cnt].MinY + KeyboardButton_FONTY;	
						
			posy = Keyboard_Button[cnt].MaxY + Keyboard_Separate/2; 		
			
			
			Keyboard_Button[cnt].Normal.ForeColor 		= Keyboard_ButtonForeColorNormal;
			Keyboard_Button[cnt].Normal.BackColor 		= Keyboard_ButtonBackColorNormal;
			Keyboard_Button[cnt].Normal.Text 					= Keyboard_ButtonStr[cnt];
			Keyboard_Button[cnt].Selected.ForeColor 	= Keyboard_ButtonForeColorSelected;
			Keyboard_Button[cnt].Selected.BackColor 	= Keyboard_ButtonBackColorSelected;
			Keyboard_Button[cnt].Selected.Text 				= Keyboard_ButtonStr[cnt];		
	}
	
	posx = Keyboard_POSX ;
	posy = Keyboard_POSY - Keyboard_FONTY - Keyboard_Separate + Keyboard_Magin;
	for (cnt = 0; cnt < Keyboard_Size; cnt++)
	{
		/////////////////////////////////
		if (cnt == 0 || cnt == 10)
		{
			posy += Keyboard_FONTY + Keyboard_Separate;
			posx =  Keyboard_FONTX/4 + Keyboard_POSX ;
		}
		else if (cnt == 20)
		{
			posy += Keyboard_FONTY + Keyboard_Separate;
			posx = Keyboard_POSX + (Keyboard_FONTX + Keyboard_Separate)  +  Keyboard_FONTX/3;
		}
		else if (cnt == 29 || cnt == 38)
		{
			posy += Keyboard_FONTY + Keyboard_Separate;
			posx = Keyboard_FONTX/4 + Keyboard_POSX;
		}
		
		/////////////////////////////////
			KEY[cnt].MinX = posx;
			KEY[cnt].MinY = posy;
			KEY[cnt].MaxY = KEY[cnt].MinY + Keyboard_FONTY;	
		if (cnt == 29 || cnt == 37 || (cnt == 38)) // cap & spec
			KEY[cnt].MaxX = KEY[cnt].MinX + Keyboard_FONTX/2 + Keyboard_FONTX*2;			
		else if (cnt == 39)		// space
			KEY[cnt].MaxX = KEY[cnt].MinX + Keyboard_FONTX/2 + Keyboard_FONTX*7;
		else if (cnt == 40)		// new line
			KEY[cnt].MaxX = KEY[cnt].MinX+ Keyboard_FONTX/2 + Keyboard_FONTX*8;
		else
			KEY[cnt].MaxX = KEY[cnt].MinX + Keyboard_FONTX/2 + Keyboard_FONTX;
		
			posx = KEY[cnt].MaxX + Keyboard_Separate ; 
		/////////////////////////////////
		
		KEY[cnt].Capital				= Keyboard_CapitalKey[cnt];
		KEY[cnt].Normal 				= Keyboard_NormalKey[cnt];		
		KEY[cnt].Special 				= Keyboard_SpecialKey[cnt];	
		KEY[cnt].IsPressed 			= 0;	
	}	
	

}
static uint8_t	TFTSevice_Keyboard_KeyEvent (TFTService_Keyboard_Button *R, int16_t touchX, int16_t touchY, TouchFinger Touch)
{
	switch (Touch)
	{
		case TouchFinger_None : return 0;
		case TouchFinger_Up	:
								if (R->IsPressed != 0)
								{
									R->IsPressed = 0;
									return 1;
								}		
								return 0;
		default: 					
								if (((touchX >= R->MinX)&&(touchX <= R->MaxX)&&(touchY >= R->MinY)&&(touchY <= R->MaxY)) != R->IsPressed)			
								{
									R->IsPressed = !R->IsPressed ;
									return 1;
								}
								return 0;
	}
}
static void TFTService_Keyboard_RenderKey(TFTService_Keyboard_Button *R)
{		
	char *text;
	if (Keyboard_Special)			text = R->Special;	
	else if (Keyboard_Shift)	text = R->Capital;	
	else											text = R->Normal	;
	
	if (R->IsPressed) 
	{		
		TFT_DrawFullRect 		(Keyboard_BackColorSelected, R->MinX, R->MinY, R->MaxX - R->MinX, R->MaxY - R->MinY);
		TFT_Text_Transparent(Keyboard_FONTX/4 + R->MinX, 	 R->MinY, 	ASCII_16x24,	Keyboard_ForeColorSelected,"%s ",text);
		TFT_Text_Transparent(Keyboard_FONTX/4 + R->MinX-1, R->MinY, 		ASCII_16x24,	Keyboard_ForeColorSelected,"%s ",text);
		TFT_Text_Transparent(Keyboard_FONTX/4 + R->MinX+1, R->MinY, 		ASCII_16x24,	Keyboard_ForeColorSelected,"%s ",text);
	}
	else 
	{		
		TFT_DrawFullRect 		(Keyboard_BackColorNormal, R->MinX, R->MinY, R->MaxX - R->MinX, R->MaxY - R->MinY);
		TFT_Text_Transparent(Keyboard_FONTX/4 + R->MinX		, R->MinY, ASCII_16x24,	Keyboard_ForeColorNormal,"%s",text);
		TFT_Text_Transparent(Keyboard_FONTX/4 + R->MinX-1	, R->MinY, ASCII_16x24,	Keyboard_ForeColorNormal,"%s",text);
		TFT_Text_Transparent(Keyboard_FONTX/4 + R->MinX+1	, R->MinY, ASCII_16x24,	Keyboard_ForeColorNormal,"%s",text);
	}	
}

static uint8_t TFTService_Keyboard_Add (TFTService_Keyboard_Button *key)
{	
	char *text;
		
	if (++Keyboard_Col >= Keyboard_ColMax)  
	{
		Keyboard_Col = 0;
		if (++Keyboard_Row >= Keyboard_RowMax)  
		{
			Keyboard_Row = Keyboard_RowMax-1;
			Keyboard_Col = Keyboard_ColMax-1;
			return 0;
		}
	}	
	
	if (Keyboard_Special)				text = key->Special;	
	else if (Keyboard_Shift)		text = key->Capital;	
	else												text = key->Normal	;
	
	Keyboard_OUTPUT[Keyboard_Index++] 		= *(text);
	TFT_Text_Background_Limit	(
															KeyboardText_POSX, KeyboardText_POSY,
															KeyboardText_SIZEX, KeyboardText_SIZEY,
															KeyboardText_FONT, KeyboardText_ForeColor,  KeyboardText_BackColor,
															Keyboard_OUTPUT
														)	;
	
	return 1;
}

static uint8_t TFTService_Keyboard_Newline (void)
{
	Keyboard_Col = 0;
	if (++Keyboard_Row >= Keyboard_RowMax)  
	{
		Keyboard_Row = Keyboard_RowMax-1;
		Keyboard_Col = Keyboard_ColMax-1;
		return 0;
	}
	
	Keyboard_OUTPUT[Keyboard_Index++] = '\n';
	TFT_DrawFullRect(KeyboardText_BackColor, KeyboardText_POSX, KeyboardText_POSY, KeyboardText_SIZEX, KeyboardText_SIZEY);
	TFT_Text_Background_Limit	(
															KeyboardText_POSX, KeyboardText_POSY,
															KeyboardText_SIZEX, KeyboardText_SIZEY,
															KeyboardText_FONT, KeyboardText_ForeColor,  KeyboardText_BackColor,
															Keyboard_OUTPUT
														)	;								
														
	return 1;
}

static uint8_t TFTService_Keyboard_Del (void)
{	
	if (Keyboard_Index)
	{							
		Keyboard_OUTPUT[--Keyboard_Index] = '\0';
		TFT_DrawFullRect(KeyboardText_BackColor, KeyboardText_POSX, KeyboardText_POSY, KeyboardText_SIZEX, KeyboardText_SIZEY);
		TFT_Text_Background_Limit	(
																KeyboardText_POSX, KeyboardText_POSY,
																KeyboardText_SIZEX, KeyboardText_SIZEY,
																KeyboardText_FONT, KeyboardText_ForeColor,  KeyboardText_BackColor,
																Keyboard_OUTPUT
															)	;		
															
		
		TFTSevice_Keyboard_GetPos(&Keyboard_Row, &Keyboard_Col);
	}
	return 1;
}

uint8_t TFTService_Keyboard (char* Tittle, char** buff)
{
	static int16_t x = 0, y = 0;	
	uint16_t cnt;	
	
	Keyboard_Row = 0;
	Keyboard_Col = 0;
	Keyboard_Index = 0;
	
	for (cnt = 0; cnt < Keyboard_OUTPUTSIZE; cnt++)	
		Keyboard_OUTPUT[cnt] = '\0';
	
	TFTSevice_Keyboard_Layout();
	
  TFT_Clear(Keyboard_BACKGROUND);	
	
	
	TFT_DrawFullRect(Keyboard_Title_BackColor, KeyboardButton_POSX - Keyboard_Separate, KeyboardButton_POSY - Keyboard_Separate, KeyboardButton_SIZEX + Keyboard_Separate*2, KeyboardButton_SIZEY + Keyboard_Separate*2);	
	TFT_DrawFullRect (Keyboard_THEME, 0, Keyboard_POSY, 320, (Keyboard_FONTY * 5 + Keyboard_Separate * 4 + Keyboard_Magin *2));	
	TFT_DrawFullRect(Keyboard_Title_BackColor, KeyboardTitle_POSX, KeyboardTitle_POSY, KeyboardTitle_SizeX, KeyboardTitle_SizeY);
	
	for (cnt = 0; cnt < Keyboard_Size; cnt++)
		TFTService_Keyboard_RenderKey(&KEY[cnt]);	
		
		TFTSevice_Button_16x24(&Keyboard_Button[0]);
		TFTSevice_Button_16x24(&Keyboard_Button[1]);
	
	
	TFT_Text_Transparent_Limit	(
																KeyboardTitle_POSX, KeyboardTitle_POSY,
																KeyboardTitle_SizeX, KeyboardTitle_SizeY,
																KeyboardTitle_FONT, Keyboard_Title_ForeColor,  
																Tittle
															)	;
	TFT_Text_Transparent_Limit	(
																KeyboardTitle_POSX+1, KeyboardTitle_POSY,
																KeyboardTitle_SizeX-1, KeyboardTitle_SizeY,
																KeyboardTitle_FONT, Keyboard_Title_ForeColor,  
																Tittle
															)	;
	
	while (1)
	{
		TouchFinger Event = TouchPanel_GetPoint(&x, &y); //TFT_SetPixelSafe (x, y, TFT_BLUE) ;	
		if (y <= Keyboard_POSY )//////////////////////////////////////////////////////////// Upper Region
		{
			switch (Event)
			{
				case TouchFinger_None: 				break;
				case TouchFinger_Up:	
									if (TFTSevice_RegionEvent(&Keyboard_Button[0], x, y, Event))		
									{	
										*buff = "\0";	
										return 0;
									}
									else if (TFTSevice_RegionEvent(&Keyboard_Button[1], x, y, Event)) 	
									{
										*buff = Keyboard_OUTPUT;	
										return 1;
									}
				default:
									for(cnt = 0; cnt < 2; cnt++)
										if (TFTSevice_RegionEvent(&Keyboard_Button[cnt], x, y, Event)) 	
											TFTSevice_Button_16x24(&Keyboard_Button[cnt]);		
									break;
			}
		}
		else //////////////////////////////////////////////////////////////////////////////////// key Region
		{
			switch (Event)
			{
				

				case TouchFinger_None: 				break;
				case TouchFinger_Up:						
								for(cnt = 0; cnt < Keyboard_Size; cnt++)
									if (TFTSevice_Keyboard_KeyEvent(&KEY[cnt], x, y, Event)) 		
									{					
										TFTService_Keyboard_RenderKey(&KEY[cnt]);	
										if 			(cnt == Keyboard_CodeNewline) 
										{
											TFTService_Keyboard_Newline();
										}
										else if (cnt == Keyboard_CodeSpec) 		
										{
											Keyboard_Shift = 0;
											Keyboard_Special = ! Keyboard_Special;
											for (cnt = 0; cnt < Keyboard_Size; cnt++)
												TFTService_Keyboard_RenderKey(&KEY[cnt]);	
										}
										else if (cnt == Keyboard_CodeShift) 		
										{
											Keyboard_Shift = ! Keyboard_Shift;
											KEY[cnt].IsPressed = Keyboard_Shift;
											for (cnt = 0; cnt < Keyboard_Size; cnt++)
												TFTService_Keyboard_RenderKey(&KEY[cnt]);	
										}
										else if (cnt == Keyboard_CodeDel) 		
										{
											TFTService_Keyboard_RenderKey(&KEY[cnt]);	
											TFTService_Keyboard_Del();
										}
										else
										{
											TFTService_Keyboard_RenderKey(&KEY[cnt]);	
											TFTService_Keyboard_Add(&KEY[cnt]);			
											if (Keyboard_Shift) 
											{
												Keyboard_Shift=0;
												KEY[cnt].IsPressed = Keyboard_Shift;
												for (cnt = 0; cnt < Keyboard_Size; cnt++)
													TFTService_Keyboard_RenderKey(&KEY[cnt]);	
											}
										}											
									}										
								break;
				default:					
								for(cnt = 0; cnt < Keyboard_Size; cnt++)
									if (TFTSevice_Keyboard_KeyEvent(&KEY[cnt], x, y, Event)) 		
									{					
										TFTService_Keyboard_RenderKey(&KEY[cnt]);		
									}
									break;
			}
		}
		
//		TFT_Text_Background	(320 - KeyboardTitle_FONTX*9, 0, KeyboardTitle_FONT, TFT_GREEN,  Keyboard_Title_BackColor,"%3d %2d %2d", Keyboard_Index, Keyboard_Row, Keyboard_Col);
		TFT_DrawFullRect(
											TFT_BLACK, 
											KeyboardText_POSX +  Keyboard_Col*KeyboardText_FONTX, 
											KeyboardText_POSY + Keyboard_Row*KeyboardText_FONTY + KeyboardText_FONTY/8,
											1,
											KeyboardText_FONTY*6/8
										);
	}
}

	
int64_t TFTService_Keyboard_StringToInt64 (char* string)
{	
	int64_t value = 0;
	uint32_t cnt = 20;
	int8_t  sign = 1;
	if (*string=='-') 
	{
		sign = -1;
		string ++;
	}
	else if (*string=='+') 
	{
		sign = 1;
		string ++;
	}
		
	while (*string>=48 && *string<=57 && cnt--!=0)
	{
		value *= 10;
		value += (int64_t) (*string - 48);
		string++;
	}
	return value*sign;
}
