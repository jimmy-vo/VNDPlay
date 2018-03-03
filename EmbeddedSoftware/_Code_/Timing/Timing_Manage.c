#include "Timing_Manage.h"
#include "Dev_TIMER.h"
#include "Dev_DIGITAL.h"
#include "Voltage_FB.h"
#include "Serial_Link.h"
#include <stdio.h>

__IO uint64_t TickCnt = 0;


RCT LCD_Clock;
uint32_t capture = 0;
uint32_t ms_Tick;			
RCC_ClocksTypeDef RCC_Clocks;

void Initialize_Timer(void)
	{				
		if (SysTick_Config(SystemCoreClock / 1000))
		{ 
			/* Capture error */ 
			while (1);
		}
		
		Initialize_TIMER 						(STATUS_TIM, STATUS_Period,	STATUS_Prescaler);
		Initialize_TIMER_INTERRUPT	(STATUS_TIM, TIMER_CH1 , STATUS_CCR1,NVIC_Prepriority_STATUS,NVIC_Subpriority_STATUS);		
		TIM_Cmd(STATUS_TIM, ENABLE);				
	
		Initialize_TIMER 						(RGBProcess_TIM, RGBProcess_Period,	RGBProcess_Prescaler);
		Initialize_TIMER_INTERRUPT	(RGBProcess_TIM, TIMER_CH1 , RGBProcess_CCR, NVIC_Prepriority_RGB, NVIC_Subpriority_RGB);		
		TIM_Cmd(RGBProcess_TIM, ENABLE);		
		
	}	
	

void Increase_RTC (void)
	{
		if (LCD_Clock.enable)
				{
						if (LCD_Clock.second  < 59) 
							{
								LCD_Clock.second ++;
							}
						else if (LCD_Clock.minute  < 59) 
							{
								LCD_Clock.second =0;
								LCD_Clock.minute ++;	
							}
						else if (LCD_Clock.hour  < 59) 
							{
								LCD_Clock.second =0;
								LCD_Clock.minute =0;	
								LCD_Clock.hour ++;
							} 
							else 
							{
								
								LCD_Clock.second =0;
								LCD_Clock.minute =0;	
								LCD_Clock.hour =0;
							}
				}
	}	
void Delay_ms (uint32_t ms)
{
	uint64_t CurTick = Get_msTick();
	while(!Check_msTick(CurTick,ms));
}
	
	
uint64_t Get_msTick(void)
{
    return ms_Tick;
}

uint8_t Check_msTick(uint64_t TickBase, uint64_t Time)
{
    uint64_t CurTick;
    
    CurTick = Get_msTick();
    
    if (CurTick > TickBase)
    {
        if (CurTick >= (TickBase + Time))
        {
            return 1;
        }
    }
    else
    {
        if (CurTick >= ((uint64_t)(TickBase + Time)))
        {
            return 1;
        }
    }
    return 0;
}	
	
void SysTick_Handler(void)
{	
	ms_Tick++;
}

