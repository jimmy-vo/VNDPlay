#include "BoardConfig.h"

#define STATUS_TIM																		TIM10
#define STATUS_Period																	0xffff 				
#define STATUS_Prescaler															168			//1us
#define STATUS_CCR1																		1000		//1ms				


#define RGBProcess_TIM																	TIM11
#define RGBProcess_Period																0xffff 				
#define RGBProcess_Prescaler														1680
#define RGBProcess_CCR																	3000						//min 4	 = max 2.5KHZ
////////////////////////////////////////////////////////////////////////////////
				#define FREQ_100kHz 10
				#define FREQ_30kHz  33
				#define FREQ_20kHz  50
				#define FREQ_10kHz  100
				#define FREQ_1kHz   1000
				#define FREQ_500Hz  2000
				#define FREQ_200Hz  5000

				#define FREQ_100Hz  10000
				#define FREQ_75Hz   15000
				#define FREQ_50Hz   20000
				#define FREQ_20Hz   50000
				#define FREQ_10Hz   100000
				#define FREQ_1Hz		1000000
////////////////////////////////////////////////////////////////////////////////

typedef struct {
								uint8_t 	enable ;		//1:enable 	0:disable
								uint8_t 	edit ;			//0: off 	1: hour		2:minite	3: second		
								int8_t 	day ;
								int8_t 	month ;
								int8_t 	year ;
								int8_t 	hour;
								int8_t 	minute;
								int8_t 	second;
								int16_t	milisecond;
								}	RCT;	
								

extern RCT LCD_Clock;

void Initialize_Timer(void);

void Delay_ms (uint32_t ms);
void Initialize_SystemTick (void );
uint64_t 	Get_msTick(void);
uint8_t 	Check_msTick(uint64_t TickBase, uint64_t Time);
