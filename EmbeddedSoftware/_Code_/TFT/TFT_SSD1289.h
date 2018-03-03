#include "stm32f4xx.h"

#define   TFT_BLACK        0x0000
#define   TFT_NAVY         0x000F
#define   TFT_DGREEN       0x03E0
#define   TFT_DCYAN        0x03EF
#define   TFT_MAROON       0x7800
#define   TFT_PURPLE       0x780F
#define   TFT_OLIVE        0x7BE0
#define   TFT_GREY         0xF7DE
#define   TFT_LGRAY        0xC618
#define   TFT_DGRAY        0x7BEF
#define   TFT_BLUE         0x001F
#define   TFT_GREEN        0x07E0
#define   TFT_CYAN         0x07FF
#define   TFT_RED          0xF800
#define   TFT_MAGENTA      0xF81F
#define   TFT_YELLOW       0xFFE0
#define   TFT_WHITE        0xFFFF

extern uint16_t TFT_Width ;
extern uint16_t TFT_Height;

void TFT_Init(uint8_t rev);

void TFT_WriteRAM_Prepare(void);
void TFT_WriteRAM(uint16_t RGB_Code);
void TFT_WriteReg(uint8_t TFT_Reg, uint16_t TFT_RegValue);

void TFT_SetDisplayWindow(uint8_t Xpos, uint16_t Ypos, uint8_t Height, uint16_t Width);
void TFT_WriteBMP(uint8_t Xpos, uint16_t Ypos, uint8_t Height, uint16_t Width, uint8_t *bitmap);

void TFT_DisplayOn(void);
void TFT_DisplayOff(void);
void TFT_BackLight(int procentai);
void TFT_Clear(uint16_t Color);

uint8_t TFT_SetCursorSafe(int16_t Xpos, int16_t Ypos);
uint8_t TFT_SetPixelSafe(int16_t x, int16_t y,int16_t c);


