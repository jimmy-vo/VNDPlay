#include "TFT_SSD1289.h"
#include "Dev_TIMER.h"

#define TFT_REG      (*((volatile unsigned short *) 0x60000000)) 
#define TFT_RAM      (*((volatile unsigned short *) 0x60020000)) 


uint16_t TFT_Width 	= 239;
uint16_t TFT_Height = 319;
uint8_t  TFT_Rev 		= 1;


uint16_t TimerPeriod    = 0;
uint16_t Channel3Pulse  = 0;

//////////////////////////////////////////////////////////////////////////////
//Local Function
//
static void TFT_CtrlLinesConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOG | 
						   RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF, 
						   ENABLE);
  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);

  GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FSMC);		// D2
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FSMC);		// D3
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FSMC);		// NOE -> RD
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FSMC);		// NWE -> WR
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource7, GPIO_AF_FSMC);		// NE1 -> CS
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FSMC);		// D13
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FSMC);		// D14
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FSMC);		// D15
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_FSMC);		// A16 -> RS
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);		// D0
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC);		// D1
  
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource7, GPIO_AF_FSMC);		// D4
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource8, GPIO_AF_FSMC);		// D5
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_FSMC);		// D6
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource10, GPIO_AF_FSMC);		// D7
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_FSMC);		// D8
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource12, GPIO_AF_FSMC);		// D9
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_FSMC);		// D10
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_FSMC);		// D11
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource15, GPIO_AF_FSMC);		// D12

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 |
                                  GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
                                  GPIO_Pin_11 | GPIO_Pin_14 | GPIO_Pin_15;
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
                                  GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 |
                                  GPIO_Pin_15;
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

  GPIO_Init(GPIOE, &GPIO_InitStructure);
  
}
static void TFT_Backlight_Init(void)
{	
  GPIO_InitTypeDef GPIO_InitStructure;
	
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD  , ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);
	
  GPIO_InitStructure.GPIO_Pin 			= GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode 			= GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed 		= GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType 		= GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd 			= GPIO_PuPd_UP ;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
	Initialize_TIMER(TIM4, 100, 100);
	Initialize_TIMER_PWM(TIM4, TIMER_CH2, 100);
	TIM_SetCompare2(TIM4, 1);
}

static void TFT_FSMCConfig(void)
{
  FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef FSMC_NORSRAMTimingInitStructure;
	
  FSMC_NORSRAMTimingInitStructure.FSMC_AddressSetupTime 			= 0;  //0
  FSMC_NORSRAMTimingInitStructure.FSMC_AddressHoldTime 				= 0;   //0   
  FSMC_NORSRAMTimingInitStructure.FSMC_DataSetupTime 					= 2;     //3   
  FSMC_NORSRAMTimingInitStructure.FSMC_BusTurnAroundDuration 	= 0;
  FSMC_NORSRAMTimingInitStructure.FSMC_CLKDivision 						= 1;//1
  FSMC_NORSRAMTimingInitStructure.FSMC_DataLatency 						= 0;
  FSMC_NORSRAMTimingInitStructure.FSMC_AccessMode 						= FSMC_AccessMode_A;
  
  FSMC_NORSRAMInitStructure.FSMC_Bank 												= FSMC_Bank1_NORSRAM1;
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux 							= FSMC_DataAddressMux_Disable;
  FSMC_NORSRAMInitStructure.FSMC_MemoryType 									= FSMC_MemoryType_SRAM;
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth 							= FSMC_MemoryDataWidth_16b;
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode 							= FSMC_BurstAccessMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity 					= FSMC_WaitSignalPolarity_Low;
  FSMC_NORSRAMInitStructure.FSMC_WrapMode 										= FSMC_WrapMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive 						= FSMC_WaitSignalActive_BeforeWaitState;
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation 							= FSMC_WriteOperation_Enable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal 									= FSMC_WaitSignal_Disable;
  FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait 						= FSMC_AsynchronousWait_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode 								= FSMC_ExtendedMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst 									= FSMC_WriteBurst_Enable;//disable
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct 				= &FSMC_NORSRAMTimingInitStructure;
  
  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
  FSMC_NORSRAMTimingInitStructure.FSMC_AddressSetupTime 			= 0;    //0  
  FSMC_NORSRAMTimingInitStructure.FSMC_AddressHoldTime 				= 0;	//0   
  FSMC_NORSRAMTimingInitStructure.FSMC_DataSetupTime 					= 4;	//3   
  FSMC_NORSRAMTimingInitStructure.FSMC_BusTurnAroundDuration 	= 0;
  FSMC_NORSRAMTimingInitStructure.FSMC_CLKDivision 						= 1;//1
  FSMC_NORSRAMTimingInitStructure.FSMC_DataLatency 						= 0;
  FSMC_NORSRAMTimingInitStructure.FSMC_AccessMode 						= FSMC_AccessMode_A;	
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct 						= &FSMC_NORSRAMTimingInitStructure;
  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);

  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
}





//////////////////////////////////////////////////////////////////////////////
//Export Function
//
void TFT_WriteRAM_Prepare(void)
{
  TFT_REG = 0x22;
}

void TFT_WriteRAM(uint16_t RGB_Code)
{
  TFT_RAM = RGB_Code;
}

void TFT_WriteReg(uint8_t TFT_Reg, uint16_t TFT_RegValue)
{
  TFT_REG = TFT_Reg;
  TFT_RAM = TFT_RegValue;
}

void TFT_DisplayOn(void)
{
  TFT_WriteReg(0x07, 0x0173); 
}

void TFT_DisplayOff(void)
{
  TFT_WriteReg(0x07, 0x0000); 
}


uint8_t TFT_SetCursorSafe(int16_t Xpos, int16_t Ypos)
{
	if ((Xpos > TFT_Width) || (Ypos > TFT_Height) || (Xpos <0) || (Ypos <0)) return 0;
	else
	{		
		if (TFT_Height < TFT_Width) //Landscape
		{
			TFT_WriteReg(0x4E, Ypos);
			TFT_WriteReg(0x4F, Xpos);
		}
		else
		{
			TFT_WriteReg(0x4E, Xpos);
			TFT_WriteReg(0x4F, Ypos);
		}
		return 1;
	}
}

uint8_t TFT_SetPixelSafe(int16_t x, int16_t y, int16_t c)
{ 
		if (TFT_SetCursorSafe(x,y))
		{
			TFT_WriteRAM_Prepare();
			TFT_WriteRAM(c);
			return 1;
		}
		else return 0;
}

void TFT_SetCursor(int16_t Xpos, int16_t Ypos)
{
	if (TFT_Height < TFT_Width) //Landscape
	{
		TFT_WriteReg(0x4E, Ypos);
		TFT_WriteReg(0x4F, Xpos);
	}
	else
	{
		TFT_WriteReg(0x4E, Xpos);
		TFT_WriteReg(0x4F, Ypos);
	}
}

void TFT_Clear(uint16_t Color)
{
  uint32_t index = 0;
  TFT_SetCursor(0x00, 0x00);
  TFT_WriteRAM_Prepare();
  for(index = 0; index <76800; index++)
  {
    TFT_RAM = Color;   
  }  
}



void TFT_Pixel(int16_t x, int16_t y, int16_t c)
{ 
	TFT_SetCursor(x,y);
	TFT_WriteRAM_Prepare();
	TFT_WriteRAM(c);
}


void TFT_SetDisplayWindow(uint8_t Xpos, uint16_t Ypos, uint8_t Height, uint16_t Width)
{
	
  if(Xpos >= Height)
    TFT_WriteReg(0x0044, (Xpos - Height +1));
		else
			TFT_WriteReg(0x0044, 0x0000);
  if(Ypos >= Width)
		TFT_WriteReg(0x0045, (Ypos - Width +1));
		else
			TFT_WriteReg(0x0045, 0x0000);
  TFT_WriteReg(0x0046, Ypos);
  TFT_SetCursor(Xpos, Ypos);

}

void TFT_WriteBMP(uint8_t Xpos, uint16_t Ypos, uint8_t Height, uint16_t Width, uint8_t *bitmap)
{
  volatile uint32_t index;
  uint16_t *bitmap_ptr = (uint16_t *)bitmap;
  uint32_t size;
  size=(Height * Width);
  TFT_SetDisplayWindow(Xpos, Ypos,Height, Width);

  TFT_WriteReg(0x11, 0x6048);
 
  TFT_WriteRAM_Prepare();
 
  for(index = 0; index < size ; index++)
		TFT_WriteRAM(*bitmap_ptr++);
	
  TFT_WriteReg(0x11, 0x6028);
  TFT_SetDisplayWindow(0, 0, TFT_Height, TFT_Width);
}

void TFT_Init(uint8_t rotate)
{ 
	if (rotate%180)
	{		
		TFT_Width 	= 319;
		TFT_Height 	= 239;
		TFT_Rev = 1;
	}
	else
	{
		TFT_Width 	= 239;
		TFT_Height 	= 319;
		TFT_Rev = 0;
	}	
	
  TFT_CtrlLinesConfig();
  TFT_FSMCConfig();
	TFT_Backlight_Init();
	
  /*Start Oscillation*/
	TFT_WriteReg(0x0000,0x0001);    
	
	/* Power Control */
	TFT_WriteReg(0x0003,0xA8A4);        
	TFT_WriteReg(0x000C,0x0000);        
	TFT_WriteReg(0x000D,0x080C);        
	TFT_WriteReg(0x000E,0x2B00);        
	TFT_WriteReg(0x001E,0x00B0);        
	
	if (TFT_Rev)
	TFT_WriteReg(0x0001,0x293F);   
	else
	TFT_WriteReg(0x0001,0x2B3F);       	
	
	TFT_WriteReg(0x0002,0x0600);      	//LCD drive AC control
	
	TFT_WriteReg(0x0010,0x0000);     	//Sleep mode cancel
	
	TFT_WriteReg(0x0011,0x7030);      	//Entry Mode
																							// DFM   0x4000 = 262L color
																							// DFM   0x6000 = 65K color
																							// AM    0x0000 = horizontal display
																							// AM    0x0008 = Vertical display
																							// ID[0] 0x0000 = horizontal decrement
																							// ID[0] 0x0010 = horizontal increment
																							// ID[1] 0x0000 = Vertical decrement
																							// ID[1] 0x0020 = Vertical increment
	// Compare register
	TFT_WriteReg(0x0005,0x0000);     
	TFT_WriteReg(0x0006,0x0000);     
	
	// Horizontal and Vertical porch are for DOTCLK mode operation
	TFT_WriteReg(0x0016,0xEF1C);     	// Horizontal Porch 
	TFT_WriteReg(0x0017,0x0003);      	// Vertical Porch
	
	TFT_WriteReg(0x0007,0x0133);     	// Display Control
																							// D1 0x0000 = display off
																							// D1 0x0002 = display on
																							// D0 0x0000 = internal display halt
																							// D0 0x0001 = internal display operate   
																							
	TFT_WriteReg(0x000B,0x0000);     	// Frame cycle control
 	TFT_WriteReg(0x000F,0x0000);      // Gate Scan Position 
	
	TFT_WriteReg(0x0041,0x0000);     	// Vertical Scroll Control
	TFT_WriteReg(0x0042,0x0000);     	// Vertical Scroll Control
	
	// 1st Screen driving position
	TFT_WriteReg(0x0048,0x0000);      	// Start position. 0
	TFT_WriteReg(0x0049,0x013F);      	// End position.   319
	
	TFT_WriteReg(0x0044,0xEF00);     	// Horizontal RAM address position  start/end setup
	TFT_WriteReg(0x0045,0x0000);     	// Vertical RAM address start position setting
	TFT_WriteReg(0x0046,0x013F);     	// Vertical RAM address end position setting
	
//	// 2nd Screen driving position
//	TFT_WriteReg(0x004A,0x0000);      	// Start position. 0
//	TFT_WriteReg(0x004B,0x0000);      	// End position.   319
	
	//gamma control 
	TFT_WriteReg(0x0030,0x0707);     
	TFT_WriteReg(0x0031,0x0204);     
	TFT_WriteReg(0x0032,0x0204);     
	TFT_WriteReg(0x0033,0x0502);     
	TFT_WriteReg(0x0034,0x0507);     
	TFT_WriteReg(0x0035,0x0204);     
	TFT_WriteReg(0x0036,0x0204);     
	TFT_WriteReg(0x0037,0x0502);     	
	TFT_WriteReg(0x003A,0x0302);     
	TFT_WriteReg(0x003B,0x0302);     
	
	TFT_WriteReg(0x0023,0x0000);     
 	TFT_WriteReg(0x0024,0x0000);     
	TFT_WriteReg(0x0025,0x8000);     
	TFT_WriteReg(0x004f,0);					 
	TFT_WriteReg(0x004e,0);					 
}
