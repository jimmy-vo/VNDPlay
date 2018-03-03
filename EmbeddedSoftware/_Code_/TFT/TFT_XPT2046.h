#include "stm32f4xx.h"



#define	CHX 	0x90
#define	CHY 	0xD0

#define XPT2046_SPI		  			SPI2
#define XPT2046_SPI_AF		  	GPIO_AF_SPI2
#define XPT2046_SPI_CLK				RCC_APB1Periph_SPI2

#define XPT2046_CS_PORT		 		GPIOB
#define XPT2046_CS_PIN		 	 	GPIO_Pin_12
#define XPT2046_CS_CLK				RCC_AHB1Periph_GPIOB

#define XPT2046_SCK_PORT		 	GPIOB
#define XPT2046_SCK_PIN		  	GPIO_Pin_13
#define XPT2046_SCK_source		GPIO_PinSource13
#define XPT2046_SCK_CLK				RCC_AHB1Periph_GPIOB

#define XPT2046_MISO_PORT		 	GPIOB
#define XPT2046_MISO_PIN	  	GPIO_Pin_14
#define XPT2046_MISO_source		GPIO_PinSource14
#define XPT2046_MISO_CLK			RCC_AHB1Periph_GPIOB

#define XPT2046_MOSI_PORT		 	GPIOB
#define XPT2046_MOSI_PIN	  	GPIO_Pin_15
#define XPT2046_MOSI_source		GPIO_PinSource15
#define XPT2046_MOSI_CLK			RCC_AHB1Periph_GPIOB

#define XPT2046_IRQ_PORT		 	GPIOD
#define XPT2046_IRQ_PIN		  	GPIO_Pin_12
#define XPT2046_IRQ_CLK				RCC_AHB1Periph_GPIOD



uint8_t Read_XPT2046(int32_t *x,int32_t *y);
void		XPT2046_Initialize(void);
uint8_t XPT2046_IRQRead(void);
