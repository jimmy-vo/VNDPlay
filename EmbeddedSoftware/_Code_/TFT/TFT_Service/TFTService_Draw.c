#include "TFTService_Draw.h"
#include "TFT_SSD1289.h"

#define MAX_POLY_CORNERS   200
#define POLY_Y(Z)          ((int32_t)((Points + Z)->X))
#define POLY_X(Z)          ((int32_t)((Points + Z)->Y))
#define ABS(X)  ((X) > 0 ? (X) : -(X))     

static uint16_t ForeColor = TFT_RED;


//////////////////////////////////////////////////////////////////////////////
//DRAW
//
void TFT_Draw_BlurScreen(uint16_t Color)
{	
 int x,y;
	for (x=0; x<TFT_Width; x++)
		for(y=0; y<TFT_Height; y++)
			if (x%2 ||y%2)
				TFT_SetPixelSafe((int16_t)x,(int16_t)y, Color);
}
void TFT_DrawTwoPointLine(uint16_t Color, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	float x, y;
	
	if (y1<y2) 
		for (y = y1; y<=y2 ; y++)
		{
			x = x1 + ((x2-x1)*(y-y1)/(y2-y1));	
			TFT_SetPixelSafe((int16_t)x,(int16_t)y, Color);
		}
	else if (y1>y2) 
		for (y = y2; y<=y1; y++)
		{
			x = x1 + ((x2-x1)*(y-y1)/(y2-y1));			
			TFT_SetPixelSafe((int16_t)x,(int16_t)y, Color);
		}
	else if (x1<x2)
		for (x = x1; x<=y2; x++)
			TFT_SetPixelSafe((int16_t)x,y1, Color);
			
	else if (x2<x1)
		for (x = x2; x<=y1; x++)
			TFT_SetPixelSafe((int16_t)x,y2, Color);		
	
	else	TFT_SetPixelSafe(x2,y2, Color);					
}

void TFT_DrawRect(uint16_t Color, uint16_t Xpos, uint16_t Ypos, uint16_t Height, uint16_t Width)
{
 int x,y;
 x=0;
 y=0;
 while(x<Height)
 {
   TFT_SetPixelSafe(Xpos+x,Ypos, Color);
   TFT_SetPixelSafe(Xpos+x,Ypos+Width-1, Color);
   x++;
 }
 while(y<Width)
 {
   TFT_SetPixelSafe(Xpos,Ypos+y, Color);
   TFT_SetPixelSafe(Xpos+Height-1,Ypos+y, Color);
   y++;
 }
}

void TFT_DrawFullRect(uint16_t Color, uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  int x,y;
  x=0;
  y=0;
  while(x<Width)
  {
    while(y<Height)
    {
      TFT_SetPixelSafe(Xpos+x,Ypos+y, Color);
      y++;
    }
    y=0;
    x++;
  }
}


void TFT_DrawCircle(uint16_t Color, uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
  int16_t  D;/* Decision Variable */ 
  uint16_t  CurX;/* Current X Value */
  uint16_t  CurY;/* Current Y Value */ 
  D = 3 - (Radius << 1);
  CurX = 0;
  CurY = Radius;
  while (CurX <= CurY)
  {
    TFT_SetPixelSafe(Xpos + CurX, Ypos + CurY, Color);
    TFT_SetPixelSafe(Xpos + CurX, Ypos - CurY, Color);
    TFT_SetPixelSafe(Xpos - CurX, Ypos + CurY, Color);
    TFT_SetPixelSafe(Xpos - CurX, Ypos - CurY, Color);
    TFT_SetPixelSafe(Xpos + CurY, Ypos + CurX, Color);
    TFT_SetPixelSafe(Xpos + CurY, Ypos - CurX, Color);
    TFT_SetPixelSafe(Xpos - CurY, Ypos + CurX, Color);
    TFT_SetPixelSafe(Xpos - CurY, Ypos - CurX, Color);
    if (D < 0)
    { 
      D += (CurX << 2) + 6;
    }
    else
    {
      D += ((CurX - CurY) << 2) + 10;
      CurY--;
    }
    CurX++;
  }
}

void TFT_DrawFullCircle(uint16_t Color, uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
  int16_t  D;
  uint16_t  CurX;
  uint16_t  CurY;
  D = 3 - (Radius << 1);
  CurX = 0;
  CurY = Radius;
  
  while (CurX <= CurY)
  {
    TFT_DrawTwoPointLine(Color, Xpos + CurX, Ypos + CurY,Xpos + CurX, Ypos - CurY);
    TFT_DrawTwoPointLine(Color, Xpos - CurX, Ypos + CurY,Xpos - CurX, Ypos - CurY);
    TFT_DrawTwoPointLine(Color, Xpos + CurY, Ypos + CurX,Xpos + CurY, Ypos - CurX);
    TFT_DrawTwoPointLine(Color, Xpos - CurY, Ypos + CurX,Xpos - CurY, Ypos - CurX);
    
    if (D < 0)
    { 
      D += (CurX << 2) + 6;
    }
    else
    {
      D += ((CurX - CurY) << 2) + 10;
      CurY--;
    }
    CurX++;
  }
}



void TFT_DrawUniLine(uint16_t Color, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
  int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0, 
  yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0, 
  curpixel = 0;
  
  deltax = ABS(x2 - x1);        /* The difference between the x's */
  deltay = ABS(y2 - y1);        /* The difference between the y's */
  x = x1;                       /* Start x off at the first pixel */
  y = y1;                       /* Start y off at the first pixel */
  
  if (x2 >= x1)                 /* The x-values are increasing */
  {
    xinc1 = 1;
    xinc2 = 1;
  }
  else                          /* The x-values are decreasing */
  {
    xinc1 = -1;
    xinc2 = -1;
  }
  
  if (y2 >= y1)                 /* The y-values are increasing */
  {
    yinc1 = 1;
    yinc2 = 1;
  }
  else                          /* The y-values are decreasing */
  {
    yinc1 = -1;
    yinc2 = -1;
  }
  
  if (deltax >= deltay)         /* There is at least one x-value for every y-value */
  {
    xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
    yinc2 = 0;                  /* Don't change the y for every iteration */
    den = deltax;
    num = deltax / 2;
    numadd = deltay;
    numpixels = deltax;         /* There are more x-values than y-values */
  }
  else                          /* There is at least one y-value for every x-value */
  {
    xinc2 = 0;                  /* Don't change the x for every iteration */
    yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
    den = deltay;
    num = deltay / 2;
    numadd = deltax;
    numpixels = deltay;         /* There are more y-values than x-values */
  }
  
  for (curpixel = 0; curpixel <= numpixels; curpixel++)
  {
    TFT_SetPixelSafe(x, y, Color);     /* Draw the current pixel */
    num += numadd;              /* Increase the numerator by the top of the fraction */
    if (num >= den)             /* Check if numerator >= denominator */
    {
      num -= den;               /* Calculate the new numerator value */
      x += xinc1;               /* Change the x as appropriate */
      y += yinc1;               /* Change the y as appropriate */
    }
    x += xinc2;                 /* Change the x as appropriate */
    y += yinc2;                 /* Change the y as appropriate */
  }
}


  
//void TFT_PolyLine(pPoint Points, uint16_t PointCount)
//{
//  int16_t X = 0, Y = 0;

//  if(PointCount < 2)
//  {
//    return;
//  }

//  while(--PointCount)
//  {
//    X = Points->X;
//    Y = Points->Y;
//    Points++;
//    TFT_DrawUniLine(X, Y, Points->X, Points->Y);
//  }
//}

//static void TFT_PolyLineRelativeClosed(pPoint Points, uint16_t PointCount, uint16_t Closed)
//{
//  int16_t X = 0, Y = 0;
//  pPoint First = Points;

//  if(PointCount < 2)
//  {
//    return;
//  }  
//  X = Points->X;
//  Y = Points->Y;
//  while(--PointCount)
//  {
//    Points++;
//    TFT_DrawUniLine(X, Y, X + Points->X, Y + Points->Y);
//    X = X + Points->X;
//    Y = Y + Points->Y;
//  }
//  if(Closed)
//  {
//    TFT_DrawUniLine(First->X, First->Y, X, Y);
//  }  
//}

//void TFT_ClosedPolyLine(pPoint Points, uint16_t PointCount)
//{
//  TFT_PolyLine(Points, PointCount);
//  TFT_DrawUniLine(Points->X, Points->Y, (Points+PointCount-1)->X, (Points+PointCount-1)->Y);
//}


//void TFT_PolyLineRelative(pPoint Points, uint16_t PointCount)
//{
//  TFT_PolyLineRelativeClosed(Points, PointCount, 0);
//}


//void TFT_ClosedPolyLineRelative(pPoint Points, uint16_t PointCount)
//{
//  TFT_PolyLineRelativeClosed(Points, PointCount, 1);
//}

void TFT_FillPolyLine(pPoint Points, uint16_t PointCount)
{
  /*  public-domain code by Darel Rex Finley, 2007 */
  uint16_t  nodes = 0, nodeX[MAX_POLY_CORNERS], pixelX = 0, pixelY = 0, i = 0, cnt =0, 
  j = 0, swap = 0;
  uint16_t  IMAGE_LEFT = 0, IMAGE_RIGHT = 0, IMAGE_TOP = 0, IMAGE_BOTTOM = 0;

  IMAGE_LEFT = IMAGE_RIGHT = Points->X;
  IMAGE_TOP= IMAGE_BOTTOM = Points->Y;

  for(i = 1; i < PointCount; i++)
  {
    pixelX = POLY_X(i);
    if(pixelX < IMAGE_LEFT)
    {
      IMAGE_LEFT = pixelX;
    }
    if(pixelX > IMAGE_RIGHT)
    {
      IMAGE_RIGHT = pixelX;
    }
    
    pixelY = POLY_Y(i);
    if(pixelY < IMAGE_TOP)
    { 
      IMAGE_TOP = pixelY;
    }
    if(pixelY > IMAGE_BOTTOM)
    {
      IMAGE_BOTTOM = pixelY;
    }
  }
  

  /*  Loop through the rows of the image. */
  for (pixelY = IMAGE_TOP; pixelY < IMAGE_BOTTOM; pixelY++) 
  {  
    /* Build a list of nodes. */
    nodes = 0; j = PointCount-1;

    for (i = 0; i < PointCount; i++) 
    {
      if (((POLY_Y(i)<(double) pixelY) && (POLY_Y(j)>=(double) pixelY)) || \
          ((POLY_Y(j)<(double) pixelY) && (POLY_Y(i)>=(double) pixelY)))
      {
        nodeX[nodes++]=(int) (POLY_X(i)+((pixelY-POLY_Y(i))*(POLY_X(j)-POLY_X(i)))/(POLY_Y(j)-POLY_Y(i))); 
      }
      j = i; 
    }
  
    /* Sort the nodes, via a simple "Bubble" sort. */
    i = 0;
    while (i < nodes-1) 
    {
      if (nodeX[i]>nodeX[i+1]) 
      {
        swap = nodeX[i]; 
        nodeX[i] = nodeX[i+1]; 
        nodeX[i+1] = swap; 
        if(i)
        {
          i--; 
        }
      }
      else 
      {
        i++;
      }
    }
  
    /*  Fill the pixels between node pairs. */
    for (i = 0; i < nodes; i+=2) 
    {
      if(nodeX[i] >= IMAGE_RIGHT) 
      {
        break;
      }
      if(nodeX[i+1] > IMAGE_LEFT) 
      {
        if (nodeX[i] < IMAGE_LEFT)
        {
          nodeX[i]=IMAGE_LEFT;
        }
        if(nodeX[i+1] > IMAGE_RIGHT)
        {
          nodeX[i+1] = IMAGE_RIGHT;
        }
				
				/* FILL */
				TFT_SetCursorSafe(pixelY, nodeX[i+1]);
				TFT_WriteRAM_Prepare(); 
				for(cnt = 0; cnt < nodeX[i+1] - nodeX[i]; cnt++)
					TFT_WriteRAM(ForeColor);
		
        TFT_SetPixelSafe(pixelY, nodeX[i+1], ForeColor);
        TFT_SetPixelSafe(pixelY, nodeX[i], ForeColor);
        /* for (j=nodeX[i]; j<nodeX[i+1]; j++) Pixel(j,pixelY); */
      }
    }
  } 
}


