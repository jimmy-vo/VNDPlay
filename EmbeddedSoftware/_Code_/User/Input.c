#include "Input.h"
#include "DEV_ADC.h"

typedef struct
{
	GPIO_TypeDef* 	PORT;
	uint32_t 				PIN;
}GPIO_DEV;

GPIO_DEV GPIOLIST[10];



void Setup_BUTTON(GPIO_TypeDef* PORT, uint32_t GPIO_Pin)
{
	GPIO_InitTypeDef   GPIO_InitStructure;
	  
	/* Enable lock */
	if (PORT == GPIOA)	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);
	if (PORT == GPIOB)	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB , ENABLE);
	if (PORT == GPIOC)	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC , ENABLE);
	if (PORT == GPIOD)	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD , ENABLE);
	
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
			GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN;
			GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
			GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin;
			GPIO_Init(PORT, &GPIO_InitStructure);
}

void Button_Initialize (void)
{
	uint8_t cnt;
	GPIOLIST[0].PORT =  GPIOA;
	GPIOLIST[0].PIN	 =  GPIO_Pin_3;
	GPIOLIST[1].PORT =  GPIOA;
	GPIOLIST[1].PIN	 =  GPIO_Pin_4;
	GPIOLIST[2].PORT =  GPIOA;
	GPIOLIST[2].PIN	 =  GPIO_Pin_5;
	GPIOLIST[3].PORT =  GPIOA;
	GPIOLIST[3].PIN	 =  GPIO_Pin_6;
	GPIOLIST[4].PORT =  GPIOA;
	GPIOLIST[4].PIN	 =  GPIO_Pin_7;
	GPIOLIST[5].PORT =  GPIOC;
	GPIOLIST[5].PIN	 =  GPIO_Pin_4;
	GPIOLIST[6].PORT =  GPIOC;
	GPIOLIST[6].PIN	 =  GPIO_Pin_5;
	GPIOLIST[7].PORT =  GPIOB;
	GPIOLIST[7].PIN	 =  GPIO_Pin_0;
	GPIOLIST[8].PORT =  GPIOB;
	GPIOLIST[8].PIN	 =  GPIO_Pin_1;
	GPIOLIST[9].PORT =  GPIOB;
	GPIOLIST[9].PIN	 =  GPIO_Pin_5;
	
	for(cnt = 0; cnt < sizeof (GPIOLIST); cnt ++)
		Setup_BUTTON(GPIOLIST[cnt].PORT, GPIOLIST[cnt].PIN);
}

///////////////////////////////////////////////////////////////////////////////////////////
// JOYSTICK
//
int16_t Joystick_AX(void)
	{			
		return  (ADC_Get(2, ADC_Average)*Joystick_MAX/Joystick_ADCMAX_LX - Joystick_OFFSET_LX);
	}
int16_t Joystick_AY(void)
	{			
		return  -(ADC_Get(3, ADC_Average)*Joystick_MAX/Joystick_ADCMAX_LY  - Joystick_OFFSET_LY);
	}
int16_t Joystick_BX(void)
	{			
		return  ADC_Get(1, ADC_Average)*Joystick_MAX/Joystick_ADCMAX_RX - Joystick_OFFSET_RX;
	}
int16_t Joystick_BY(void)
	{			
		return  -(ADC_Get(0, ADC_Average)*Joystick_MAX/Joystick_ADCMAX_RY - Joystick_OFFSET_RY);
	}
	
void Joystick_A(int16_t *x,int16_t *y)
{
	*x = Joystick_AX();
	*y = Joystick_AY();
}

void Joystick_B(int16_t *x,int16_t *y)
{
	*x = Joystick_BX();
	*y = Joystick_BY();
} 

///////////////////////////////////////////////////////////////////////////////////////////
// BUTTON
//
uint8_t INPUT_Read(uint8_t num)
{
	return !GPIO_ReadInputDataBit(GPIOLIST[num].PORT, GPIOLIST[num].PIN);
}

void INPUT_ReadList (uint8_t * INPUT)
{
	uint8_t cnt;
	for (cnt = 0; cnt < 10; cnt ++)
		INPUT[cnt] = !GPIO_ReadInputDataBit(GPIOLIST[cnt].PORT, GPIOLIST[cnt].PIN);
}

uint8_t ButtonA_Joystick(void) {return INPUT_Read(0);}

uint8_t ButtonA_Right		(void) {return INPUT_Read(1);}
uint8_t ButtonA_Up			(void) {return INPUT_Read(2);}
uint8_t ButtonA_Left		(void) {return INPUT_Read(3);}
uint8_t ButtonA_Down		(void) {return INPUT_Read(4);}

uint8_t ButtonB_Down		(void) {return INPUT_Read(5);}
uint8_t ButtonB_Right		(void) {return INPUT_Read(6);}
uint8_t ButtonB_Left		(void) {return INPUT_Read(7);}
uint8_t ButtonB_Up			(void) {return INPUT_Read(8);}

uint8_t ButtonB_Joystick(void) {return INPUT_Read(9);}

