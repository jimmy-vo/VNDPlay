#include "stm32f4xx.h"

typedef struct 
{
  int16_t X;
  int16_t Y;
} Point, * pPoint;   

void TFT_DrawTwoPointLine(uint16_t ForeColor, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

void TFT_DrawRect(uint16_t Color, uint16_t Xpos, uint16_t Ypos, uint16_t Height, uint16_t Width);
void TFT_DrawFullRect(uint16_t Color, uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);

void TFT_DrawUniLine(uint16_t Color, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void TFT_DrawCircle(uint16_t Color, uint16_t Xpos, uint16_t Ypos, uint16_t Radius);
void TFT_DrawFullCircle(uint16_t Color, uint16_t Xpos, uint16_t Ypos, uint16_t Radius);

void TFT_PolyLine(pPoint Points, uint16_t PointCount);
void TFT_PolyLineRelative(pPoint Points, uint16_t PointCount);
void TFT_ClosedPolyLine(pPoint Points, uint16_t PointCount);
void TFT_ClosedPolyLineRelative(pPoint Points, uint16_t PointCount);
void TFT_FillPolyLine(pPoint Points, uint16_t PointCount);
void TFT_Draw_BlurScreen(uint16_t Color);
