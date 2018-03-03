#include "TFTService_Button.h"
#include "stm32f4xx.h"

//////////////////////////////////////////////////////////////////////////////////
#define SUDOKU_OFFSET_X 								0
#define SUDOKU_OFFSET_Y 								0

#define SUDOKU_POS_Title_X							SUDOKU_OFFSET_X 
#define SUDOKU_POS_Title_Y							SUDOKU_OFFSET_Y 

#define SUDOKU_SIZE_Title_X							120
#define SUDOKU_SIZE_Title_Y							45

//////////////////////////////////////////////////////////////////////////////////

#define SUDOKU_POS_Button_X							SUDOKU_OFFSET_X + 5 + 0
#define SUDOKU_POS_Button_Y							SUDOKU_POS_Title_Y +  SUDOKU_SIZE_Title_Y + 25
	
#define SUDOKU_FONT_Button							ASCII_16x24 
#define SUDOKU_FONTSize_Button_X				16
#define SUDOKU_FONTSize_Button_Y				24
#define SUDOKU_Separate_Button					5

#define SUDOKU_SIZE_Button_X						SUDOKU_FONTSize_Button_X * 7
#define SUDOKU_SIZE_Button_Y						SUDOKU_FONTSize_Button_Y * 6 + SUDOKU_Separate_Button * 4

//////////////////////////////////////////////////////////////////////////////////
	
#define SUDOKU_POS_Board_X							SUDOKU_POS_Title_X + SUDOKU_SIZE_Title_X + 2
#define SUDOKU_POS_Board_Y							SUDOKU_OFFSET_Y + 3
	
#define SUDOKU_FONT_Board								ASCII_16x24 	
#define SUDOKU_FONTSize_BoardX					16
#define SUDOKU_FONTSize_BoardY					24
#define SUDOKU_CellGrid									1
#define SUDOKU_Separate_Board 					3

#define SUDOKU_SIZE_Board_X								SUDOKU_FONTSize_BoardX * 9 + SUDOKU_Separate_Board * 4 + SUDOKU_CellGrid*6
#define SUDOKU_SIZE_Board_Y								SUDOKU_FONTSize_BoardY * 9 + SUDOKU_Separate_Board * 4 + SUDOKU_CellGrid*6

//////////////////////////////////////////////////////////////////////////////////
#define SUDOKU_POS_choice_X								SUDOKU_POS_Board_X + SUDOKU_SIZE_Board_X
#define SUDOKU_POS_choice_Y								SUDOKU_OFFSET_Y

#define SUDOKU_FONT_Choice								ASCII_16x24 
#define SUDOKU_FONTSize_Choice_X					16
#define SUDOKU_FONTSize_Choice_Y					24
#define SUDOKU_Separate_Choice						10

#define SUDOKU_SIZE_choice_X							SUDOKU_FONTSize_Choice_X*3
#define SUDOKU_SIZE_choice_Y							SUDOKU_FONTSize_Choice_Y * 10


#define SUDOKU_SIZE_NewGameConfirm_X		280
#define SUDOKU_SIZE_NewGameConfirm_Y		140
#define SUDOKU_POS_NewGameConfirm_X			(319 - SUDOKU_SIZE_NewGameConfirm_X)/2
#define SUDOKU_POS_NewGameConfirm_Y			(239 - SUDOKU_SIZE_NewGameConfirm_Y)/2

#define SUDOKU_SIZE_CheckConfirm_X			280
#define SUDOKU_SIZE_CheckConfirm_Y			140
#define SUDOKU_POS_CheckConfirm_X				(319 - SUDOKU_SIZE_CheckConfirm_X)/2
#define SUDOKU_POS_CheckConfirm_Y				(239 - SUDOKU_SIZE_CheckConfirm_Y)/2

#define SUDOKU_SIZE_SolveConfirm_X			200
#define SUDOKU_SIZE_SolveConfirm_Y			100
#define SUDOKU_POS_SolveConfirm_X				(319 - SUDOKU_SIZE_SolveConfirm_X)/2
#define SUDOKU_POS_SolveConfirm_Y				(239 - SUDOKU_SIZE_SolveConfirm_Y)/2

//////////////////////////////////////////////////////////////////////////////////
// Color
//
#define	SUDOKU_BACKGROUND											TFT_WHITE

#define	SUDOKU_Theme_Title										TFT_BLACK
#define	SUDOKU_Theme_Board										TFT_NAVY
/////////////////////////

#define	SUDOKU_ForeColor_ButtonNormal					TFT_BLACK
#define	SUDOKU_BackColor_ButtonNormal					TFT_LGRAY

#define	SUDOKU_ForeColor_ButtonSelected				TFT_BLACK
#define	SUDOKU_BackColor_ButtonSelected				TFT_DGRAY
//////////////////////////

#define	SUDOKU_ForeColor_SourceNormal					TFT_RED
#define	SUDOKU_BackColor_SourceNormal					TFT_LGRAY

#define	SUDOKU_ForeColor_SourceSelected				TFT_RED
#define	SUDOKU_BackColor_SourceSelected				TFT_DGRAY

#define	SUDOKU_ForeColor_TrueNormal						TFT_BLUE
#define	SUDOKU_BackColor_TrueNormal						TFT_LGRAY

#define	SUDOKU_ForeColor_TrueSelected					TFT_BLUE
#define	SUDOKU_BackColor_TrueSelected					TFT_DGRAY

#define	SUDOKU_ForeColor_WrongNormal					TFT_YELLOW
#define	SUDOKU_BackColor_WrongNormal					TFT_LGRAY

#define	SUDOKU_ForeColor_WrongSelected				TFT_YELLOW
#define	SUDOKU_BackColor_WrongSelected				TFT_DGRAY

#define	SUDOKU_ForeColor_UserNormal						TFT_BLACK
#define	SUDOKU_BackColor_UserNormal						TFT_LGRAY

#define	SUDOKU_ForeColor_UserSelected					TFT_LGRAY
#define	SUDOKU_BackColor_UserSelected					TFT_DGRAY
///////////////////////////


typedef enum 
{
	Sudoku_BUTTON_New 		 = 0,	
	Sudoku_BUTTON_Check		 = 1,
	Sudoku_BUTTON_Hint		 = 2,
	Sudoku_BUTTON_Solve		 = 3,
	Sudoku_BUTTON_Exit		 = 4,
}Sudoku_BUTTON;

typedef enum 
{
	Sudoku_Indicate_Given	 = 0,	
	Sudoku_Indicate_Wrong	 = 1,
	Sudoku_Indicate_True	 = 2,
	Sudoku_Indicate_User	 = 3,
}Sudoku_Indicate;

typedef struct 
{
	uint16_t 						MinX;
	uint16_t 						MaxX;
	uint16_t 						MinY;
	uint16_t 						MaxY;
	uint8_t  						IsSelected;
	uint8_t 						Source;
	uint8_t 						User;
	Sudoku_Indicate			Indicate;
	int8_t							MAP;
}Sudoku_BOARD; 

extern Sudoku_BOARD PUZZLE[9][9];
extern TFTService_Button SudokuChoice [10];
extern TFTService_Button SudokuButton[5];

void Sudoku_InitLayout(void);
void Sudoku_RenderCell(uint8_t row, uint8_t col);
void Sudoku_Render(void);
