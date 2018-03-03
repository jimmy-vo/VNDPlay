#include "Sudoku.h"
#include "Sudoku_Layout.h"
#include "Sudoku_Event.h"
#include <stdio.h>
#include <stdlib.h>
#include "TFT_SSD1289.h"
#include "TFTService_Text.h"
#include "TFTService_Draw.h"

//////////////////////////////////////////////////////////////////////////////////
// Sudoku Algorithms
//

static void Sudoku_SolBoardFill (void)
{
	uint8_t x, y, num, cnt, check = 0;
	uint8_t rowStart;
	uint8_t colStart;
	
	for(num=1; num<=9; num++)
	{				
		for(x=0; x<9; x++)	
			for (y=0; y<9; y++)	
			{		
				rowStart = (x/3) * 3;
				colStart = (y/3) * 3;
				
				//make map
				if (PUZZLE[x][y].User != 0)	
					PUZZLE[x][y].MAP = num;		
				
				if (PUZZLE[x][y].User == num)					
					for (cnt=0; cnt<9; cnt++)	
					{
							PUZZLE[x][cnt].MAP = num;
							PUZZLE[cnt][y].MAP = num;
							PUZZLE[rowStart + (cnt%3)][colStart + (cnt/3)].MAP  = num;	
					}

				//check map		
				if ((PUZZLE[x][y].User == 0) && (PUZZLE[x][y].MAP != num))
				{				
						if (check <8)	
							{
								check = 0; cnt = 0;
								while (check<8 && cnt<9)
									if (PUZZLE[x][cnt++].MAP == num) 	
										check++;		
							}	
							
						if (check <8)	
							{
								check = 0; cnt = 0;
								while (check<8 && cnt<9)
									if (PUZZLE[cnt++][y].MAP == num) 	
										check++;		
							}	
							
						if (check <8)	
							{
								check = 0; cnt = 0;
								while (check<8 && cnt<9)
								{
									if (PUZZLE[rowStart + (cnt%3)][colStart + (cnt/3)].MAP == num) 	
										check++;		
									cnt ++;
								}
							}
							
						if (check ==8)	
						{
							cnt = 0 ;
							check = 0;
							PUZZLE[x][y].User = num;
						}
				}
			}	
	}
}
static uint8_t Sudoku_SolCellFill(int row, int col)
{
    uint8_t rowStart = (row/3) * 3;
    uint8_t colStart = (col/3) * 3;
		uint8_t Sudoku_check[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t i = 0, num = 0, posible = 0, solution =0 ;		
		
		for(num=1; num<=9; num++)
		{
			for(i=0; i<9; i++)	
			{
					if (PUZZLE[row][i].User  == num)																		Sudoku_check[num] 	|= 1 ;	
					if (PUZZLE[i][col].User  == num)																		Sudoku_check[num] 	|= 1 ;	
					if (PUZZLE[rowStart + (i%3)][colStart + (i/3)].User  == num)				Sudoku_check[num] 	|= 1 ;	
			}
			//check minor group duplicate	
			if (Sudoku_check[num] == 0) 
			{
					posible++;
					solution = num;
			}				
		}		
			
		if (posible == 1)			return solution;
	 
		return 0;
}


static uint8_t Sudoku_SolCheckVaild(int row, int col, int num)
{
    uint8_t rowStart = (row/3) * 3;
    uint8_t colStart = (col/3) * 3;
    uint8_t i;		
		
	for(i=0; i<9; i++)	
	{
		if (PUZZLE[row][i].User == num)                             return 0;
		if (PUZZLE[i][col].User == num)                             return 0;
		if (PUZZLE[rowStart + (i%3)][colStart + (i/3)].User == num) return 0;
	}
	return 1;
}

static uint8_t Sudoku_RecursiveLogical(int done, int row, int col)
{		
    if(done>=80)	return 1;
		else
    {				
				if	(col == 0 && row == 0)		Sudoku_SolBoardFill();
				if			(col>=9)							return Sudoku_RecursiveLogical( done, row + 1 , 0);
				else	if(row>=9)							return Sudoku_RecursiveLogical(0, 0, 0);
													 
				
        if(PUZZLE[row][col].User != 0)					 														return Sudoku_RecursiveLogical(done+1, row, col+1);
        else if((PUZZLE[row][col].User = Sudoku_SolCellFill(row, col))!=0) 		return Sudoku_RecursiveLogical(done+1, row, col+1);
				else 																																return Sudoku_RecursiveLogical(done, row, col+1);		
    }	
}



uint8_t Sudoku_RecursiveRandom(int row, int col)
{			
	if( row<9 || col<9 )	
	{													
			if			(col>=9)							return Sudoku_RecursiveRandom( row + 1 , 0);
			else	if(row>=9)							return 1;
	
			else if (PUZZLE[row][col].User) 
			{
					return Sudoku_RecursiveRandom(row,  col+1);
			}
			else
			{
				while (++PUZZLE[row][col].MAP  <= 9)
				{	
					if (Sudoku_SolCheckVaild(row, col, PUZZLE[row][col].MAP))		//check for validity
					{
						PUZZLE[row][col].User = PUZZLE[row][col].MAP;	
						if (Sudoku_RecursiveRandom(row,  col+1))		
							return 1;									
					}					
				}	
				PUZZLE[row][col].User = 0;	
				PUZZLE[row][col].MAP = 0;
				return 0;																								//end loop but still no sulution found
			}
	}
	else	
		return 1;		
}




//////////////////////////////////////////////////////////////////////////////////
// Sudoku App
//
uint8_t Sudoku_source[9][9] =
{
	1,3,4,5,2,8,6,9,7,
	2,5,6,7,9,1,3,4,8,
	7,8,9,3,4,6,1,2,5,
	3,1,2,4,6,7,5,8,9,
	5,4,7,9,8,3,2,1,6,
	9,6,8,1,5,2,7,3,4,
	4,7,5,2,1,9,8,6,3,
	8,9,1,6,3,5,4,7,2,
	6,2,3,8,7,4,9,5,1
};

void Sudoku_Generate(uint8_t given)
{
	 static uint8_t   x, y, row, col;	
	 
	//Erase
	for(row=0;row<9;row++)
		for(col=0;col<9;col++)
		{
			PUZZLE[row][col].Source 	= Sudoku_source[row][col];
			PUZZLE[row][col].User 		= 0;
			PUZZLE[row][col].MAP 			= 0;
			PUZZLE[row][col].Indicate = Sudoku_Indicate_User;
		}
	
	given = (10 - given) * 7 + (rand() % 9);
	while (given)		
	{
		x = rand() % 9;
		y = rand() % 9;
		
		if (PUZZLE[x][y].User==0) 
		{
			given--;
			PUZZLE[x][y].User = PUZZLE[x][y].Source;
			PUZZLE[x][y].Indicate = Sudoku_Indicate_Given;
		}
	}	
}

//////////////////////////////////////////////////////////////////////////////////
// Sudoku global
//
void Sudoku_Window (void)
{	
	uint8_t  Touch = 1; 
	uint64_t Zero;
	
	Sudoku_InitLayout();
	Sudoku_Generate (5);	
	Sudoku_Render();

	while (Touch)
	{		
		Touch = Sudoku_TouchEvent_Process();
		if (Zero++ > SUDOKU_ButtonSensitivity)
		{			
			if (Sudoku_InputEvent_Process()) 	Zero = 0;
			else 															Zero = SUDOKU_ButtonSensitivity;
		}
		
	}	
}
