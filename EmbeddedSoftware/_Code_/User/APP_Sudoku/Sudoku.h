#include "stm32f4xx.h"


#define SUDOKU_ButtonSensitivity			0x4ffff
#define SUDOKU_ANALOGTHRES 2


void Sudoku_Generate(uint8_t given);
uint8_t Sudoku_RecursiveRandom(int row, int col);
void Sudoku_Window (void);
