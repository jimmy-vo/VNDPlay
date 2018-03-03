/* Includes ------------------------------------------------------------------*/
#include "TFT_TouchPanel.h"
#include "TFT_SSD1289.h"
#include "TFT_XPT2046.h"


typedef	struct  
{
   float x;
   float y;
}Coordinate;

Coordinate TouchPanel_Point;


uint8_t XPT2046_IRQRead(void)
{
  return !GPIO_ReadInputDataBit(XPT2046_IRQ_PORT, XPT2046_IRQ_PIN);
}

static void WR_CMD (uint16_t cmd)  
{ 
  /* Wait for SPI3 Tx buffer empty */ 
  while (SPI_I2S_GetFlagStatus(XPT2046_SPI, SPI_I2S_FLAG_TXE) == RESET); 
  /* Send SPI3 data */ 
  SPI_I2S_SendData(XPT2046_SPI,cmd); 
  /* Wait for SPI3 data reception */ 
  while (SPI_I2S_GetFlagStatus(XPT2046_SPI, SPI_I2S_FLAG_RXNE) == RESET); 
  /* Read SPI3 received data */ 
  SPI_I2S_ReceiveData(XPT2046_SPI); 
} 

static uint16_t RD_AD(void)  
{ 
  uint16_t buf, temp; 
	
  while (SPI_I2S_GetFlagStatus(XPT2046_SPI, SPI_I2S_FLAG_TXE) == RESET); 
	
  SPI_I2S_SendData(XPT2046_SPI,0x0000); 
	
  while (SPI_I2S_GetFlagStatus(XPT2046_SPI, SPI_I2S_FLAG_RXNE) == RESET); 
	
  temp=SPI_I2S_ReceiveData(XPT2046_SPI); 
  buf=temp<<8; 	
	
  while (SPI_I2S_GetFlagStatus(XPT2046_SPI, SPI_I2S_FLAG_TXE) == RESET); 
	
  SPI_I2S_SendData(XPT2046_SPI,0x0000); 
	
  while (SPI_I2S_GetFlagStatus(XPT2046_SPI, SPI_I2S_FLAG_RXNE) == RESET); 
	
  temp=SPI_I2S_ReceiveData(XPT2046_SPI); 
  buf |= temp; 
  buf>>=3; 
  buf&=0xfff; 
  return buf; 
}


static void XPT2046_GetAdXY(int32_t *x,int32_t *y)  
{   
	GPIO_ResetBits(XPT2046_CS_PORT, XPT2046_CS_PIN);	//reset CS pin
  WR_CMD(CHX); 
  *x = RD_AD(); 	
	
  WR_CMD(CHY); 
  *y = RD_AD();
  GPIO_SetBits(XPT2046_CS_PORT, XPT2046_CS_PIN); //set CS pin		
}
	

#define THRESHOLD 2
uint8_t Read_XPT2046(int32_t *x,int32_t *y)
{
  int32_t m0,m1,m2,TP_X,TP_Y,temp[3];
  uint8_t count=0;
  int32_t buffer[2][9]={{0},{0}};
  int32_t screenx = 0, screeny = 0;
  do
  {		   
    XPT2046_GetAdXY(&TP_X,&TP_Y);  
	  buffer[0][count]=TP_X;  
	  buffer[1][count]=TP_Y;
	  count++;  
  }
  while(XPT2046_IRQRead()&& count<9);  /* TP_INT_IN  */
	
  if(count==9)   /* Average X Y  */ 
  {
		/* Average X  */
		temp[0]=(buffer[0][0]+buffer[0][1]+buffer[0][2])/3;
		temp[1]=(buffer[0][3]+buffer[0][4]+buffer[0][5])/3;
		temp[2]=(buffer[0][6]+buffer[0][7]+buffer[0][8])/3;

		m0=temp[0]-temp[1];
		m1=temp[1]-temp[2];
		m2=temp[2]-temp[0];

		m0=m0>0?m0:(-m0);
		m1=m1>0?m1:(-m1);
		m2=m2>0?m2:(-m2);

		if ( m0>THRESHOLD  &&  m1>THRESHOLD  &&  m2>THRESHOLD ) return 0;
		
		if(m0<m1)
		{
			if(m2<m0) 
				screenx=(temp[0]+temp[2])/2;
			else 
				screenx=(temp[0]+temp[1])/2;	
		}
		else if(m2<m1) 
			screenx=(temp[0]+temp[2])/2;
		else 
			screenx=(temp[1]+temp[2])/2;
		
		/* Average Y  */
		temp[0]=(buffer[1][0]+buffer[1][1]+buffer[1][2])/3;
		temp[1]=(buffer[1][3]+buffer[1][4]+buffer[1][5])/3;
		temp[2]=(buffer[1][6]+buffer[1][7]+buffer[1][8])/3;
		m0=temp[0]-temp[1];
		m1=temp[1]-temp[2];
		m2=temp[2]-temp[0];
		m0=m0>0?m0:(-m0);
		m1=m1>0?m1:(-m1);
		m2=m2>0?m2:(-m2);
		
		
		if(m0>THRESHOLD&&m1>THRESHOLD&&m2>THRESHOLD)				return 0;
		
		if(m0<m1)
		{
			if(m2<m0) 
				screeny=(temp[0]+temp[2])/2;
			else 
				screeny=(temp[0]+temp[1])/2;	
			}
		else if(m2<m1) 
			 screeny=(temp[0]+temp[2])/2;
		else
			 screeny=(temp[1]+temp[2])/2;
		
		*x = screenx;
		*y = screeny;
		
		return 1;
  }  
	return 0;
}


void XPT2046_Initialize(void) 
{ 
  GPIO_InitTypeDef GPIO_InitStruct;
  SPI_InitTypeDef  SPI_InitStructure;


  /* Configure SPI3 pins: SCK, MISO and MOSI ---------------------------------*/ 	
  RCC_AHB1PeriphClockCmd(XPT2046_SCK_CLK, ENABLE);
  RCC_AHB1PeriphClockCmd(XPT2046_MISO_CLK, ENABLE);
  RCC_AHB1PeriphClockCmd(XPT2046_MOSI_CLK, ENABLE);
	
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
		GPIO_InitStruct.GPIO_Pin = XPT2046_SCK_PIN ;
		GPIO_Init(XPT2046_SCK_PORT, &GPIO_InitStruct);
		GPIO_PinAFConfig(XPT2046_SCK_PORT, 	XPT2046_SCK_source, XPT2046_SPI_AF);
		
		GPIO_InitStruct.GPIO_Pin = XPT2046_MISO_PIN ;
		GPIO_Init(XPT2046_MISO_PORT, &GPIO_InitStruct);
		GPIO_PinAFConfig(XPT2046_MISO_PORT, XPT2046_MISO_source, XPT2046_SPI_AF);
		
		GPIO_InitStruct.GPIO_Pin = XPT2046_MOSI_PIN ;
		GPIO_Init(XPT2046_MOSI_PORT, &GPIO_InitStruct);
		GPIO_PinAFConfig(XPT2046_MOSI_PORT, XPT2046_MOSI_source, XPT2046_SPI_AF);

  /* TP_CS */
  RCC_AHB1PeriphClockCmd(XPT2046_CS_CLK, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStruct.GPIO_Pin = XPT2046_CS_PIN;
	GPIO_Init(XPT2046_CS_PORT, &GPIO_InitStruct);

  /* TP_IRQ */
  RCC_AHB1PeriphClockCmd(XPT2046_IRQ_CLK, ENABLE);
	
  GPIO_InitStruct.GPIO_Pin =  XPT2046_IRQ_PIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(XPT2046_IRQ_PORT, &GPIO_InitStruct);
    
	//set CS pin
  GPIO_SetBits(XPT2046_CS_PORT, XPT2046_CS_PIN); 

  /* Configure SPI -----------------------------------------------------------*/
  RCC_APB1PeriphClockCmd(XPT2046_SPI_CLK, ENABLE);
	
  SPI_Cmd(XPT2046_SPI, DISABLE); 
  SPI_InitStructure.SPI_Direction 					= SPI_Direction_2Lines_FullDuplex; 
  SPI_InitStructure.SPI_Mode 								= SPI_Mode_Master; 
  SPI_InitStructure.SPI_DataSize 						= SPI_DataSize_8b; 
  SPI_InitStructure.SPI_CPOL 								= SPI_CPOL_Low; 
  SPI_InitStructure.SPI_CPHA 								= SPI_CPHA_1Edge; 
  SPI_InitStructure.SPI_NSS 								= SPI_NSS_Soft; 
  SPI_InitStructure.SPI_BaudRatePrescaler 	= SPI_BaudRatePrescaler_128; 
  SPI_InitStructure.SPI_FirstBit 						= SPI_FirstBit_MSB; 
  SPI_InitStructure.SPI_CRCPolynomial 			= 7; 
  SPI_Init(XPT2046_SPI, &SPI_InitStructure); 
  SPI_Cmd(XPT2046_SPI, ENABLE); 
	
} 

