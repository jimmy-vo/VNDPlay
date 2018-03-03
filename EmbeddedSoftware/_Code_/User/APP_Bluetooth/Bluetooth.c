#include "Bluetooth.h"
#include "Bluetooth_Layout.h"
#include "Bluetooth_Event.h"
#include "Bluetooth_ATCMD.h"

#include "Dev_USART.h"
#include "Timing_Manage.h"
#include "Parameter.h"
#include "RGB.h"

//AT Mode 1: 
//AT mode 2: First set the PIN34 as high level, or
//while on powering the module set the PIN34 to be
//high level, the Baud rate used here is 38400 bps.
//Notice: All AT commands can be operated only

Bluetooth_DEVICE 				Bluetooth_DeviceList[Bluetooth_DeviceMax];
uint32_t 								Bluetooth_Baudrate = 19200;
uint16_t 								Bluetooth_StopBit 	= USART_StopBits_1;
uint16_t 								Bluetooth_Parity 	= USART_Parity_No;
Bluetooth_ATMODE  			Bluetooth_ATMode;
Bluetooth_CONNECTMODE 	Bluetooth_ConnectMode = Bluetooth_CMODE_Manual;
Bluetooth_ROLE				 	Bluetooth_Role = Bluetooth_ROLE_Slave;
///////////////////////////////////////////////////////////////////////////
////Terminal
//
uint16_t Bluetooth_RXIndex = 0;

void Bluetooth_SendString(char* addr)
{
	if (Bluetooth_GetPwrPin())
		while (*addr != '\0')  
		{
			while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
			USART_SendData(USART1, (uint8_t)*(addr++));
		}
}

void Bluetooth_Send(char* addr, uint16_t byte)
{
	if (Bluetooth_GetPwrPin())
		while (byte--)  
		{
			while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
			USART_SendData(USART1, (uint8_t)*(addr++));
		}
}

///////////////////////////////////////////////////////////////////////////
////Initialize HC05
//	
void Bluetooth_PrepareRX(char * buff, uint32_t OUTPUTSIZE)
{	
	uint32_t cnt;
	for (cnt = 0; cnt < OUTPUTSIZE; cnt++) buff[cnt] = 0;	
	USART_Interrupt_Set (&USART_1,USART_RX,	(uint8_t *)buff, OUTPUTSIZE, USART_MODE_Continuos);		
}

void Bluetooth_SetupUART (uint32_t BaudRate, uint16_t STOPBIT, uint16_t PARITY)
{
	USART_OpenPort(&USART_1,BaudRate, Bluetooth_StopBit, Bluetooth_Parity);		
	USART_Interrupt_Set (&USART_1,USART_RX,	0, 0, USART_MODE_Continuos);
}

void Bluetooth_Initialize(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
			
	/* KEY Pin */
	RCC_AHB1PeriphClockCmd(HC05_KEY_CLK, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin 		= HC05_KEY_PIN;
	GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 		= GPIO_PuPd_DOWN;
	GPIO_Init(HC05_KEY_PORT, &GPIO_InitStructure); 	
		
	/* POWER Pin */
	RCC_AHB1PeriphClockCmd(HC05_PWR_CLK, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin 		= HC05_PWR_PIN;
	GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 		= GPIO_PuPd_DOWN;
	GPIO_Init(HC05_PWR_PORT, &GPIO_InitStructure); 		
		
	
	/* STATUS Pin */
	RCC_AHB1PeriphClockCmd(HC05_STA_CLK, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin 		= HC05_STA_PIN;
	GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 		= GPIO_PuPd_UP;
	GPIO_Init(HC05_STA_PORT, &GPIO_InitStructure); 		
	
	
	/* Uart Pin */
	RCC_AHB1PeriphClockCmd(UART1_GPIO_CLK, ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = UART1_RX_PIN | UART1_TX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(UART1_GPIO_PORT, &GPIO_InitStructure);
 
  GPIO_PinAFConfig(UART1_GPIO_PORT, UART1_TX_SOURCE, GPIO_AF_USART3);
  GPIO_PinAFConfig(UART1_GPIO_PORT, UART1_RX_SOURCE, GPIO_AF_USART3);
	
	
	USART_1.USART 							= USART1;
	USART_1.IT_NVIC_IRQ					= USART1_IRQn;
	
	USART_1.TX.USART_DMA				= USART_DMA_Disable;
	USART_1.TX.DMA_Channel 			= UART1_TX_DMA_CH;
	USART_1.TX.DMA_Stream	 			= UART1_TX_DMA_ST;
	USART_1.TX.DMA_NVIC_IRQ 		= UART1_TX_DMA_IRQn;
	USART_1.TX.preemtionPri 		= NVIC_Prepriority_SerialLink;
	USART_1.TX.SubPri 					= NVIC_Subpriority_SerialLink;

	USART_1.RX.USART_DMA				= USART_DMA_Disable;
	USART_1.RX.DMA_Channel 			= UART1_RX_DMA_CH;
	USART_1.RX.DMA_Stream	 			= UART1_RX_DMA_ST;
	USART_1.RX.DMA_NVIC_IRQ 		= UART1_RX_DMA_IRQn;	
	USART_1.RX.preemtionPri 		= NVIC_Prepriority_SerialLink;
	USART_1.RX.SubPri 					= NVIC_Subpriority_SerialLink;
}


///////////////////////////////////////////////////////////////////////////
////Pins access
//	
uint8_t Bluetooth_SetPwrPin (uint8_t turn)
{
	GPIO_WriteBit(HC05_PWR_PORT, HC05_PWR_PIN, (BitAction) turn);
	return 	GPIO_ReadOutputDataBit(HC05_PWR_PORT, HC05_PWR_PIN);	
}

uint8_t Bluetooth_GetPwrPin (void)
{
	return 	GPIO_ReadOutputDataBit(HC05_PWR_PORT, HC05_PWR_PIN);	
}

uint8_t Bluetooth_SetKeyPin(uint8_t turn)
{
	GPIO_WriteBit(HC05_KEY_PORT, HC05_KEY_PIN, (BitAction) turn);
	return 	GPIO_ReadOutputDataBit(HC05_KEY_PORT, HC05_KEY_PIN);	
}
uint8_t Bluetooth_GetKeyPin(void)
{
	return 	GPIO_ReadOutputDataBit(HC05_KEY_PORT, HC05_KEY_PIN);	
}
uint8_t Bluetooth_GetSttPin(void)
{
	return 	GPIO_ReadInputDataBit(HC05_STA_PORT, HC05_STA_PIN);	
}


uint8_t Bluetooth_SwitchMode (Bluetooth_ATMODE mode)
{
	switch (mode)
	{
		case Bluetooth_ATMODE_AT://////////////////////////////////////////////////////////
									if 	(Bluetooth_ATMode == Bluetooth_ATMODE_AT)		return 0;
									Bluetooth_ATMode = Bluetooth_ATMODE_AT;	
									if  (Bluetooth_GetPwrPin())
									{ 	
										Bluetooth_SetPwrPin(0);
										Delay_ms (500);
									}
									Bluetooth_SetKeyPin(1);
									Bluetooth_SetPwrPin(1);			
									Bluetooth_SetupUART(38400, USART_StopBits_1, USART_Parity_No);
									break;
		default ://////////////////////////////////////////////////////////
									if 	(Bluetooth_ATMode == Bluetooth_ATMODE_COM)	return 0;
									Bluetooth_ATMode = Bluetooth_ATMODE_COM;								
									if  (Bluetooth_GetPwrPin())
									{ 	
										Bluetooth_SetPwrPin(0);
										Delay_ms (500);
									}
									Bluetooth_SetKeyPin(0);
									Bluetooth_SetPwrPin(1);			
									Bluetooth_SetupUART(Bluetooth_Baudrate, Bluetooth_StopBit, Bluetooth_Parity);
									break;
	}
	Bluetooth_Render_Status();
	return 1;
}

//////////////////////////////////////////////////////////////////////////////////
// Sudoku global
//
void Bluetooth_Window (void)
{	
	Bluetooth_InitLayout ();
	Bluetooth_Render_Main();	
	
	while (Bluetooth_TouchEvent_Process());
}
