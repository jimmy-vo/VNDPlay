#include "Sudoku.h"
#include "Sudoku_Event.h"
#include "Sudoku_Layout.h"
#include "TFTService_Dialog.h"
#include <stdio.h>
#include <stdlib.h>
#include "Input.h"
#include "RGB.h"

typedef enum
{
	SUDOKU_CHOICE_Up,
	SUDOKU_CHOICE_Down,
	SUDOKU_MOVE_Up,
	SUDOKU_MOVE_Down,
	SUDOKU_MOVE_Left,
	SUDOKU_MOVE_Right	
}SUDOKU_MOVE;

static uint8_t Sudoku_Event_NewGame(void)
{
	uint8_t row, col, cnt, result = 0xff;	
	static int16_t x = 0, y = 0;	
	TouchFinger Event;
	
	if (TFTSevice_ShowDialogYesNo(
									SUDOKU_POS_NewGameConfirm_X, SUDOKU_POS_NewGameConfirm_Y, 
									SUDOKU_SIZE_NewGameConfirm_X, SUDOKU_SIZE_NewGameConfirm_Y, 
									"  New Game",
									"  Current game will be cancel. Are you sure to start new game?\n\n* Yes: Choose difficulty on the right bar.\n* No: Back to current game.")
			)		
	{	
		for(row = 0; row < 9; row++)								//searching for highlight row
				for(col = 0; col < 9; col++)							//searching for highlight Collumn
					{
						PUZZLE[row][col].Source 					= 0;
						PUZZLE[row][col].User 						= 0;
						PUZZLE[row][col].Indicate 				= Sudoku_Indicate_User;
						PUZZLE[row][col].IsSelected 	= 0;
					}
		
		Sudoku_Render();
		while (result == 0xff)
		{
			Event = TouchPanel_GetPoint(&x, &y); 		
			if (x >= SUDOKU_POS_choice_X &&  y>=SUDOKU_POS_choice_Y)////////////////////////// Choice Region
			{
				switch (Event)
				{
					case TouchFinger_None: break;
					case TouchFinger_Up: 
									for(cnt = 0; cnt < 10; cnt++)
										if (TFTSevice_RegionEvent(&SudokuChoice[cnt], x, y, Event)) 		
										{
											TFTSevice_Button_16x24(&SudokuChoice[cnt]);			
											result = cnt;
										}
					default:						
									for(cnt = 0; cnt < 10; cnt++)
										if (TFTSevice_RegionEvent(&SudokuChoice[cnt], x, y, Event)) 		
											TFTSevice_Button_16x24(&SudokuChoice[cnt]);				
									break;
				}
			}
		}		
		if (result)	Sudoku_Generate (result);
	}
	Sudoku_Render();	
	return 1;	
}

static uint8_t Sudoku_Event_Check (void)
{
	uint8_t row, col;	
	if (TFTSevice_ShowDialogYesNo(
									SUDOKU_POS_CheckConfirm_X, SUDOKU_POS_CheckConfirm_Y, 
									SUDOKU_SIZE_CheckConfirm_X, SUDOKU_SIZE_CheckConfirm_Y, 
									"  Check",
									"  Do you want to check current inputs?\n\n * Blue: Right cell(s).\n * Yellow: Wrong Cell(s).")
			)	
	{
		for(row = 0; row < 9; row++)								//searching for highlight row
			for(col = 0; col < 9; col++)							//searching for highlight Collumn
				if ((PUZZLE[row][col].Source) &&(PUZZLE[row][col].User) && (PUZZLE[row][col].Indicate != Sudoku_Indicate_Given))
				{
					if (PUZZLE[row][col].Source != PUZZLE[row][col].User)		
						PUZZLE[row][col].Indicate = Sudoku_Indicate_Wrong;
					else 
						PUZZLE[row][col].Indicate = Sudoku_Indicate_True;			
					Sudoku_RenderCell (row,col);	
				}
	}
	Sudoku_Render();	
	return 1;	
}

static uint8_t Sudoku_Event_Hint (void)
{
	uint8_t row, col;
	for(row = 0; row < 9; row++)								//searching for highlight row
		for(col = 0; col < 9; col++)							//searching for highlight Collumn											
			if ((PUZZLE[row][col].IsSelected) && (PUZZLE[row][col].Indicate != Sudoku_Indicate_Given))
			{
				PUZZLE[row][col].User = PUZZLE[row][col].Source;
				PUZZLE[row][col].Indicate = Sudoku_Indicate_True;	
				Sudoku_RenderCell (row,col);	
			}	
	return 1;	
}

static uint8_t Sudoku_Event_Solve (void)
{
	uint8_t row = 0, col = 0, source = 0, empty = 0;
	
	//check if it is user input
	for(row = 0; row < 9; row++)
		for(col = 0; col < 9; col++)
		{
			if((PUZZLE[row][col].Source)||(PUZZLE[row][col].Indicate == Sudoku_Indicate_True))		source ++;
			if((PUZZLE[row][col].User == 0) || (PUZZLE[row][col].User != PUZZLE[row][col].Source))	empty ++;
		}
	if (empty==0)	TFTSevice_ShowDialogOK(
																				SUDOKU_POS_SolveConfirm_X, SUDOKU_POS_SolveConfirm_Y, 
																				SUDOKU_SIZE_SolveConfirm_X, SUDOKU_SIZE_SolveConfirm_Y, 
																				"  Solved", "  The game is already solve!"
																			);
	
	else if (
						TFTSevice_ShowDialogYesNo(
																				SUDOKU_POS_SolveConfirm_X, SUDOKU_POS_SolveConfirm_Y, 
																				SUDOKU_SIZE_SolveConfirm_X, SUDOKU_SIZE_SolveConfirm_Y, 
																				"  Solve",
																				"  Do you want to solve current game?"
																			)
					)
						{							
							if (source)	
							{		
								for(row = 0; row < 9; row++)								
									for(col = 0; col < 9; col++)						
										if (PUZZLE[row][col].Indicate != Sudoku_Indicate_Given)
										{
											PUZZLE[row][col].User = PUZZLE[row][col].Source;
											PUZZLE[row][col].Indicate = Sudoku_Indicate_True;	
											Sudoku_RenderCell (row,col);	
										}					
								TFTSevice_ShowDialogOK(
																				SUDOKU_POS_SolveConfirm_X, SUDOKU_POS_SolveConfirm_Y, 
																				SUDOKU_SIZE_SolveConfirm_X, SUDOKU_SIZE_SolveConfirm_Y, 
																				"  Solved", "  Solving succeed!"
																			);
							}
							else	//user input mode
							{
								for(row = 0; row < 9; row++)
									for(col = 0; col < 9; col++)
										if	(PUZZLE[row][col].User)	
											{
												PUZZLE[row][col].Source = PUZZLE[row][col].User;
												PUZZLE[row][col].Indicate = Sudoku_Indicate_Given;
											}
											if (Sudoku_RecursiveRandom(0,0))	
												TFTSevice_ShowDialogOK(
																								SUDOKU_POS_SolveConfirm_X, SUDOKU_POS_SolveConfirm_Y, 
																								SUDOKU_SIZE_SolveConfirm_X, SUDOKU_SIZE_SolveConfirm_Y, 
																								"  Solved", "  Solving succeed!"
																							);
											else 
												TFTSevice_ShowDialogOK(
																								SUDOKU_POS_SolveConfirm_X, SUDOKU_POS_SolveConfirm_Y, 
																								SUDOKU_SIZE_SolveConfirm_X, SUDOKU_SIZE_SolveConfirm_Y, 
																								"  Solved", "  Input Unsolvable!"
																							);
							}	
						}
	Sudoku_Render();	
	return 1;	
}


static void Sudoku_Event_Choice(uint8_t NUM)
{
	uint8_t row, col;	
		
	if (SudokuChoice[NUM].IsPressed)										//if choice is highlight
	{
		for(row = 0; row < 9; row++)								//searching for highlight row
			for(col = 0; col < 9; col++)							//searching for highlight Collumn
				if (PUZZLE[row][col].IsSelected && PUZZLE[row][col].Indicate != Sudoku_Indicate_Given && PUZZLE[row][col].Indicate != Sudoku_Indicate_True)					//if found
				{
					PUZZLE[row][col].User 	= NUM;				//Assign choice to cell
					if (PUZZLE[row][col].Indicate == Sudoku_Indicate_Wrong)
						PUZZLE[row][col].Indicate = Sudoku_Indicate_User;
					Sudoku_RenderCell (row,col);					
				}		
	}	
}

static void Sudoku_Event_Board(uint8_t row, uint8_t col)
{
	uint8_t cnt;	
	
	if (PUZZLE[row][col].IsSelected)								//if cell is highlight
	{
		for(cnt = 0; cnt < 10; cnt++)								//reset all highlight array
		{
			SudokuChoice[cnt].IsPressed = 0;	
			TFTSevice_Button_16x24(&SudokuChoice[cnt]);		
		}
		
		SudokuChoice[PUZZLE[row][col].User].IsPressed = 1;			//highlight following cell			
		Sudoku_RenderCell (row,col);					
		TFTSevice_Button_16x24(&SudokuChoice[cnt]);		
	}			
	else
	{
		Sudoku_RenderCell (row,col);
	}
}

static int Sudoku_Event_Button(Sudoku_BUTTON Button)
{
	switch (Button)
	{
		case Sudoku_BUTTON_New: 		return Sudoku_Event_NewGame();
		case Sudoku_BUTTON_Check:		return Sudoku_Event_Check();	
		case Sudoku_BUTTON_Hint:		return Sudoku_Event_Hint();
		case Sudoku_BUTTON_Solve:		return Sudoku_Event_Solve();	
		case Sudoku_BUTTON_Exit:		return 0;		
	}
	
	return 1;
}

static uint8_t Sudoku_Event_INPUT (SUDOKU_MOVE input)
{
	uint8_t row, col;	
	
	for(row = 0; row < 9; row++)								//searching for highlight row
		for(col = 0; col < 9; col++)							//searching for highlight Collumn
			if (PUZZLE[row][col].IsSelected)
			{
				PUZZLE[row][col].IsSelected = 0;
				Sudoku_RenderCell (row,col);
				switch (input)
				{
					case SUDOKU_MOVE_Up: 								
																if (row > 0) 	row --;
																else 					row = 8;						
																break;
					case SUDOKU_MOVE_Down: 
																if (row < 8) row ++;
																else row = 0;			
																break;
					case SUDOKU_MOVE_Left: 								
																if (col > 0) 	col --;
																else 					col = 8;						
																break;
					case SUDOKU_MOVE_Right: 
																if (col < 8) col ++;
																else col = 0;			
																break;														
					case SUDOKU_CHOICE_Up: 	
																if ((PUZZLE[row][col].Indicate == Sudoku_Indicate_User) || (PUZZLE[row][col].Indicate == Sudoku_Indicate_Wrong))
																{	
																	if (PUZZLE[row][col].User < 9)
																		PUZZLE[row][col].User ++;					
																	else 
																		PUZZLE[row][col].User = 0;
																}
																break;															
					case SUDOKU_CHOICE_Down: 
																if ((PUZZLE[row][col].Indicate == Sudoku_Indicate_User) || (PUZZLE[row][col].Indicate == Sudoku_Indicate_Wrong))
																{	
																	if (PUZZLE[row][col].User > 0)
																		PUZZLE[row][col].User --;					
																	else 
																		PUZZLE[row][col].User = 9;
																}
																break;
				}
				PUZZLE[row][col].IsSelected = 1;
				Sudoku_Event_Board (row,col);
			}			
	return 1;
}

static uint8_t	Sudoku_BoardEvent (Sudoku_BOARD *R, int16_t touchX, int16_t touchY, TouchFinger State)
{
	switch (State)
	{
		case TouchFinger_None : break;
		case TouchFinger_Up	:
														if (R->IsSelected != 0)
														{
															R->IsSelected = 0;
															return 1;
														}
														else			
															return 0;
		default: 					
							if (((touchX >= R->MinX) &&	(touchX <= R->MaxX) &&	(touchY >= R->MinY) &&	(touchY <= R->MaxY))	!= R->IsSelected)		
							{
								R->IsSelected = !R->IsSelected ;
								return 1;
							}
							else	
								return 0;
	}
	return 0;
}

//////////////////////////////////////////////////////
// Global Extern
uint8_t Sudoku_TouchEvent_Process(void)
{	
	uint8_t row, col, cnt;	
	static int16_t x = 0, y = 0;	
	TouchFinger Event = TouchPanel_GetPoint(&x, &y); //Pixel (x, y, RED) ;
	
	if (x >= SUDOKU_POS_choice_X &&  y>=SUDOKU_POS_choice_Y)////////////////////////// Choice Region
	{
		switch (Event)
		{
			case TouchFinger_None: 	return 1;
			case TouchFinger_Up:		return 1;
			default:						
							for(cnt = 0; cnt < 10; cnt++)
								if (TFTSevice_RegionEvent(&SudokuChoice[cnt], x, y, Event)) 		
								{							
									TFTSevice_Button_16x24(&SudokuChoice[cnt]);			
									Sudoku_Event_Choice (cnt);					
								}
							return 1;
		}
	}
	else if (x >= SUDOKU_POS_Board_X &&  y>=SUDOKU_POS_Board_Y)////////////////////// Board Region
	{
		switch (Event)
		{
			case TouchFinger_None: 	return 1;
			case TouchFinger_Up: 		return 1;
			default:		
							for(row = 0; row < 9; row++)																			
								for(col = 0; col < 9; col++)																								
									if (Sudoku_BoardEvent(&PUZZLE[row][col], x, y, Event))
									{
										Sudoku_RenderCell(row,col);
										Sudoku_Event_Board(row,col);
									}
							return 1;
		}
	}
	else if (x>=  SUDOKU_POS_Button_X  &&  y>=SUDOKU_POS_Button_Y)//////////////// Button Region
	{			
		switch (Event)
		{
			case TouchFinger_None: 	return 1;
			case TouchFinger_Up: 					
							for(cnt = 0; cnt < 5; cnt++)
								if (TFTSevice_RegionEvent(&SudokuButton[cnt], x, y, Event))
								{
									TFTSevice_Button_16x24(&SudokuButton[cnt]);			
									return Sudoku_Event_Button ((Sudoku_BUTTON)cnt);	
								}									
								return 1;
			default:								
							for(cnt = 0; cnt < 5; cnt++)
								if (TFTSevice_RegionEvent(&SudokuButton[cnt], x, y, Event))
									TFTSevice_Button_16x24(&SudokuButton[cnt]);			
								return 1;
		}
	}	
	return 1;
}

uint8_t Sudoku_InputEvent_Process(void)
{
	uint8_t event = 0;
	static uint8_t Input[10];
	
	int16_t J_AX = Joystick_AX();
	int16_t J_AY = Joystick_AY();
	int16_t J_BX = Joystick_BX();
	int16_t J_BY = Joystick_BY();
	INPUT_ReadList(Input);	
	
	///////////////////////////////////////////////////////////////////////////
	// Inputs A
	
	// up and down
	if (Input[A_Up] != Input[A_Down])
	{
		if (Input[A_Up]) 					event += Sudoku_Event_INPUT(SUDOKU_MOVE_Up);
		if (Input[A_Down]) 				event += Sudoku_Event_INPUT(SUDOKU_MOVE_Down);
	}
	else
	{
		if (J_AY > SUDOKU_ANALOGTHRES) 	event += Sudoku_Event_INPUT(SUDOKU_MOVE_Up);
		if (J_AY <-SUDOKU_ANALOGTHRES) 	event += Sudoku_Event_INPUT(SUDOKU_MOVE_Down);		
	}
	
	// left and right
	if (Input[A_Left] != Input[A_Right])
	{
		if (Input[A_Left]) 				event += Sudoku_Event_INPUT(SUDOKU_MOVE_Left);
		if (Input[A_Right]) 			event += Sudoku_Event_INPUT(SUDOKU_MOVE_Right);
	}
	else
	{
		if (J_AX > SUDOKU_ANALOGTHRES)	event += Sudoku_Event_INPUT(SUDOKU_MOVE_Right);
		if (J_AX <-SUDOKU_ANALOGTHRES) 	event += Sudoku_Event_INPUT(SUDOKU_MOVE_Left);		
	}
	
	
	///////////////////////////////////////////////////////////////////////////
	// Inputs B
	
	// up and down
	if (Input[B_Up] != Input[B_Down])
	{
		if (Input[B_Up]) 					event += Sudoku_Event_INPUT(SUDOKU_CHOICE_Down);
		if (Input[B_Down]) 				event += Sudoku_Event_INPUT(SUDOKU_CHOICE_Up);
	}
	else
	{
		if (J_BY > SUDOKU_ANALOGTHRES) 	event += Sudoku_Event_INPUT(SUDOKU_CHOICE_Down);
		if (J_BY <-SUDOKU_ANALOGTHRES) 	event += Sudoku_Event_INPUT(SUDOKU_CHOICE_Up);		
	}
	return event;
}
