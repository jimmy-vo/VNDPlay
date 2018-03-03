#include "Sudoku_Layout.h"
#include <stdio.h>
#include <stdlib.h>
#include "TFT_SSD1289.h"
#include "TFTService_Text.h"
#include "TFTService_Draw.h"


static char *SudokuButtonString[5] = 
{	
	" New   ",	
	" Check ",
	" Hint  ",
	" Solve ",
	" Exit  "	
};

static char *SudokuChoiceString[10] =
{
	" 0",	" 1",	" 2",	" 3",	" 4",	
	" 5", " 6",	" 7",	" 8",	" 9",
};
	

 Sudoku_BOARD PUZZLE[9][9];
 TFTService_Button SudokuChoice [10];
 TFTService_Button SudokuButton[5];


//////////////////////////////////////////////////////////////////////////////////
// Sudoku Init
//
void Sudoku_InitLayout(void)
{		
	 uint8_t row, col, cnt;
	 uint16_t	 posx , posy;
	
	//Init board matrix
	posx = SUDOKU_POS_Board_X ;
	posy = SUDOKU_POS_Board_Y;
	PUZZLE[4][4].IsSelected =1;
	for(row=0;row<9;row++)
	{		
		if (row%3 == 0) 							posy += SUDOKU_Separate_Board;			
		else 													posy += SUDOKU_CellGrid;	
		for(col=0;col<9;col++)
		{						
			if (col%3 == 0)  					posx += SUDOKU_Separate_Board;			
			else 											posx += SUDOKU_CellGrid;	
			
			PUZZLE[row][col].MinX = posx;
			PUZZLE[row][col].MinY = posy;
			PUZZLE[row][col].MaxX = PUZZLE[row][col].MinX + SUDOKU_FONTSize_BoardX ;
			PUZZLE[row][col].MaxY = PUZZLE[row][col].MinY + SUDOKU_FONTSize_BoardY ;
			
			posx = PUZZLE[row][col].MaxX; 
			posy = PUZZLE[row][col].MinY; 
		}
		posx =  SUDOKU_POS_Board_X;
		posy += SUDOKU_FONTSize_BoardY;
	}
		
	//Init Choice
	posx = SUDOKU_POS_choice_X; 
	posy = SUDOKU_POS_choice_Y; 
	for(cnt=0;cnt<10;cnt++)		
	{ 
			SudokuChoice[cnt].MinX = posx;
			SudokuChoice[cnt].MinY = posy;
			SudokuChoice[cnt].MaxX = SudokuChoice[cnt].MinX + SUDOKU_SIZE_choice_X;
			SudokuChoice[cnt].MaxY = SudokuChoice[cnt].MinY + SUDOKU_FONTSize_Choice_Y;	
			
			posy = SudokuChoice[cnt].MaxY; 		
						
			SudokuChoice[cnt].Normal.ForeColor 		= SUDOKU_ForeColor_ButtonNormal;
			SudokuChoice[cnt].Normal.BackColor 		= SUDOKU_BackColor_ButtonNormal;
			SudokuChoice[cnt].Normal.Text 				= SudokuChoiceString[cnt];
			SudokuChoice[cnt].Selected.ForeColor 	= SUDOKU_ForeColor_ButtonSelected;
			SudokuChoice[cnt].Selected.BackColor 	= SUDOKU_BackColor_ButtonSelected;
			SudokuChoice[cnt].Selected.Text 			= SudokuChoiceString[cnt];	
			SudokuChoice[cnt].Disable.ForeColor 		= SUDOKU_ForeColor_ButtonSelected;
			SudokuChoice[cnt].Disable.BackColor 		= SUDOKU_BackColor_ButtonSelected;
			SudokuChoice[cnt].Disable.Text 				= SudokuChoiceString[cnt];			
	}
	
	//Init Button
	posx = SUDOKU_POS_Button_X; 
	posy = SUDOKU_POS_Button_Y; 
	for(cnt=0;cnt<5;cnt++)		
	{ 
			SudokuButton[cnt].MinX = posx;
			SudokuButton[cnt].MinY = posy;
			SudokuButton[cnt].MaxX = SudokuButton[cnt].MinX + SUDOKU_SIZE_Button_X;
			SudokuButton[cnt].MaxY = SudokuButton[cnt].MinY + SUDOKU_FONTSize_Button_Y;	
			
			posy = SudokuButton[cnt].MaxY + SUDOKU_Separate_Button; 		
			if (cnt ==3) posy += SUDOKU_FONTSize_Button_Y;			
			
			SudokuButton[cnt].Normal.ForeColor 		= SUDOKU_ForeColor_ButtonNormal;
			SudokuButton[cnt].Normal.BackColor 		= SUDOKU_BackColor_ButtonNormal;
			SudokuButton[cnt].Normal.Text 				= SudokuButtonString[cnt];
			SudokuButton[cnt].Selected.ForeColor 	= SUDOKU_ForeColor_ButtonSelected;
			SudokuButton[cnt].Selected.BackColor 	= SUDOKU_BackColor_ButtonSelected;
			SudokuButton[cnt].Selected.Text 			= SudokuButtonString[cnt];
			SudokuButton[cnt].Disable.ForeColor 		= SUDOKU_ForeColor_ButtonSelected;
			SudokuButton[cnt].Disable.BackColor 		= SUDOKU_BackColor_ButtonSelected;
			SudokuButton[cnt].Disable.Text 				= SudokuButtonString[cnt];			
	}
}


//////////////////////////////////////////////////////////////////////////////////
// Sudoku RENDER
//

void Sudoku_RenderCell(uint8_t row, uint8_t col)
{	
	uint16_t  BackColor, ForeColor ;	
	
	switch (PUZZLE[row][col].Indicate)
	{
		case Sudoku_Indicate_Given:////////////////////////////////////////////////////////////////
					if (PUZZLE[row][col].IsSelected)
					{
						ForeColor = SUDOKU_ForeColor_SourceSelected;
						BackColor = SUDOKU_BackColor_SourceSelected;	
					}
					else
					{
						ForeColor = SUDOKU_ForeColor_SourceNormal;
						BackColor = SUDOKU_BackColor_SourceNormal ;
					}			
					if (PUZZLE[row][col].User)
						TFT_Text_Background(PUZZLE[row][col].MinX,PUZZLE[row][col].MinY, ASCII_16x24,	ForeColor, BackColor,"%d",PUZZLE[row][col].User);
					else
						TFT_Text_Background(PUZZLE[row][col].MinX,PUZZLE[row][col].MinY, ASCII_16x24,	ForeColor, BackColor," ");	
					break;
		case Sudoku_Indicate_True:////////////////////////////////////////////////////////////////			
					if (PUZZLE[row][col].IsSelected)
					{
						ForeColor = SUDOKU_ForeColor_TrueSelected;
						BackColor = SUDOKU_BackColor_TrueSelected;	
					}
					else
					{
						ForeColor = SUDOKU_ForeColor_TrueNormal;
						BackColor = SUDOKU_BackColor_TrueNormal ;
					}			
					if (PUZZLE[row][col].User)
						TFT_Text_Background(PUZZLE[row][col].MinX,PUZZLE[row][col].MinY, ASCII_16x24,	ForeColor, BackColor,"%d",PUZZLE[row][col].User);
					else
						TFT_Text_Background(PUZZLE[row][col].MinX,PUZZLE[row][col].MinY, ASCII_16x24,	ForeColor, BackColor," ");	
					break;
		case Sudoku_Indicate_Wrong:////////////////////////////////////////////////////////////////
			if (PUZZLE[row][col].IsSelected)
			{
				ForeColor = SUDOKU_ForeColor_WrongSelected;
				BackColor = SUDOKU_BackColor_WrongSelected;	
			}
			else
			{
				ForeColor = SUDOKU_ForeColor_WrongNormal;
				BackColor = SUDOKU_BackColor_WrongNormal ;
			}			
			if (PUZZLE[row][col].User)
				TFT_Text_Background(PUZZLE[row][col].MinX,PUZZLE[row][col].MinY, ASCII_16x24,	ForeColor, BackColor,"%d",PUZZLE[row][col].User);
			else
				TFT_Text_Background(PUZZLE[row][col].MinX,PUZZLE[row][col].MinY, ASCII_16x24,	ForeColor, BackColor," ");	
			break;
		case Sudoku_Indicate_User://////////////////////////////////////////////////////////////			
			if (PUZZLE[row][col].IsSelected)
			{
				ForeColor = SUDOKU_ForeColor_UserSelected;
				BackColor = SUDOKU_BackColor_UserSelected;	
			}
			else
			{
				ForeColor = SUDOKU_ForeColor_UserNormal;
				BackColor = SUDOKU_BackColor_UserNormal ;
			}
			if (PUZZLE[row][col].User)
				TFT_Text_Background(PUZZLE[row][col].MinX,PUZZLE[row][col].MinY, ASCII_16x24,	ForeColor, BackColor,"%d",PUZZLE[row][col].User);
			else
				TFT_Text_Background(PUZZLE[row][col].MinX,PUZZLE[row][col].MinY, ASCII_16x24,	ForeColor, BackColor," ");	
			break;
	}
}


void Sudoku_Render(void)
{
	uint8_t row, col, cnt;	
	
  TFT_Clear(SUDOKU_BACKGROUND);	
	
	//Draw titile	
	TFT_DrawRect (SUDOKU_Theme_Title+10, SUDOKU_POS_Title_X+3, SUDOKU_POS_Title_Y+0, SUDOKU_SIZE_Title_X-3, SUDOKU_SIZE_Title_Y-3);
	TFT_DrawRect (SUDOKU_Theme_Title+20, SUDOKU_POS_Title_X+2, SUDOKU_POS_Title_Y+1, SUDOKU_SIZE_Title_X-3, SUDOKU_SIZE_Title_Y-3);
	TFT_DrawRect (SUDOKU_Theme_Title+30, SUDOKU_POS_Title_X+1, SUDOKU_POS_Title_Y+2, SUDOKU_SIZE_Title_X-3, SUDOKU_SIZE_Title_Y-3);
	TFT_DrawRect (SUDOKU_Theme_Title+40, SUDOKU_POS_Title_X+0, SUDOKU_POS_Title_Y+3, SUDOKU_SIZE_Title_X-3, SUDOKU_SIZE_Title_Y-3);
	
	TFT_Text_Transparent(14, 10, ASCII_16x24,	TFT_BLUE, "SuDoKu");
	TFT_Text_Transparent(15, 10, ASCII_16x24,	TFT_BLUE, "SuDoKu");
	TFT_Text_Transparent(12, 10, ASCII_16x24,	TFT_BLUE, "SuDoKu");
	TFT_Text_Transparent(13, 10, ASCII_16x24,	TFT_BLUE, "SuDoKu");
	TFT_Text_Transparent(95, 28, ASCII_08x12,	TFT_BLACK, "VND");	
	TFT_Text_Transparent(94, 28, ASCII_08x12,	TFT_BLACK, "VND");	
	
	//Draw theme	
	TFT_DrawFullRect (	SUDOKU_Theme_Board, SUDOKU_POS_Board_X , SUDOKU_POS_Board_Y, SUDOKU_SIZE_Board_X, SUDOKU_SIZE_Board_Y);
	
	
	//Put Choices	
	for(cnt=0;cnt<5;cnt++)		
		TFTSevice_Button_16x24(&SudokuButton[cnt]);			
	
	//Put cell	
	for(row = 0; row < 9; row++)
		for(col = 0; col < 9; col++)	
			Sudoku_RenderCell(row, col);
			
	//Put Choices	
	for(cnt=0;cnt<10;cnt++)		
		TFTSevice_Button_16x24(&SudokuChoice[cnt]);		
}
