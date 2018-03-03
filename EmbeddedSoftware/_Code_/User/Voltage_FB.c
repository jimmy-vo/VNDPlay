#include "Voltage_FB.h"
#include "Timing_Manage.h"
#include "DEV_ADC.h"

uint16_t Battery_Cell=2;

float Get_Voltage(void)
	{			
		uint16_t	ADCVAl=0;
		float Volt;
		
		ADCVAl = ADC_Get(4, ADC_Average);
		Volt =  ((float)ADCVAl) * ADCtoVolt * VF_SlopeScale + VF_SlopeShift;	
		return Volt;
	}
	
uint16_t CELL;
uint16_t Cell_Detect(void)
	{
		float New_VOLTAGE=0;
		int Loop=10;			// limit cell detect loop
		
		CELL=0;
		while (New_VOLTAGE==0) New_VOLTAGE = Get_Voltage();
		if (New_VOLTAGE<=5) 	
				return 0;
			else					
				while ((New_VOLTAGE>max_cell*CELL || New_VOLTAGE<min_cell*CELL) && Loop-- )
					{
						if 			(New_VOLTAGE>max_cell*(float)CELL)										CELL++;
						else if (New_VOLTAGE<min_cell*(float)CELL) 										CELL--;
						else																													Loop=0;
					}
		return CELL;
	}

uint8_t Battery_Check(void)
	{
		float New_VOLTAGE = Get_Voltage();
		if ((CELL!=0)&&(New_VOLTAGE<=3.3f*(float)CELL)) 				return 0;
			else 																								return 1;
	}
