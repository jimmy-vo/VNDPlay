#include "AHRS.h"
#include "MPU6050.h"
#include "Timing_Manage.h"
#include "Dev_DIGITAL.h"
#include "SDCARD.h"
#include "Serial_Link.h"
#include "Parameter.h"
#include "Dev_I2C.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "ff.h"
#include "SENSOR_DATA.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////Variable
//

//AHRS

static AHRS_Status AHRS_status = AHRS_NONE;	
static uint64_t AHRS_Lastime;
static uint8_t AHRS_Trial;







AHRS_Status AHRS_Process (void)
{
	switch (AHRS_status)
	{
		case AHRS_NONE:///////////////////////////////////////////
					if((AHRS_status = Initialize_AHRS()) != AHRS_OK)
					{						
//						SerialLink_Console(EventInfo, "%s",String_AHRS_Status[AHRS_status]);
						return AHRS_FAIL;
					}	
					if((AHRS_status = AHRS_Update_dt(SampleTime)) != AHRS_OK)		return AHRS_FAIL;
					
					return (AHRS_status = AHRS_Trial_dt);		// check initial dt config					
		case AHRS_Wrong_dt:///////////////////////////////////////						
//					SerialLink_Console(EventInfo, "%s", String_AHRS_Status[AHRS_status]);	
//					RGB_Set(RED_Toggle);							
					if((AHRS_status = AHRS_Update_dt(SampleTime)) != AHRS_OK)		return AHRS_FAIL;
					
					return (AHRS_status = AHRS_Trial_dt);		
		case AHRS_Trial_dt:///////////////////////////////////////
					if (Get_msTick() - AHRS_Lastime < AHRS_dtms)			return (AHRS_status = AHRS_Trial_dt);	
					else
					{
						AHRS_Lastime = Get_msTick();
						if (SENSOR_Update_Data (&Raw_Acce, &Raw_Gyro, &AHRS_dt) != SENSOR_OK)
						{		
//							SerialLink_Console(EventInfoDetail, "%s",String_SENSOR_Status[SENSOR]);
							return (AHRS_status = AHRS_FAIL);
						}
						else if ((AHRS_dt - SampleTime)> SampleTime*AHRS_dt_error)	
						{
							AHRS_Trial = 0;
							return (AHRS_status = AHRS_Trial_dt);	
						}
						else
						{
								AHRS_status = AHRS_Task();									 					
								if (AHRS_Trial ++ < AHRS_TRIAL)	return (AHRS_status = AHRS_Trial_dt);
								else
								{
//									SerialLink_Console(EventInfo, "%s", String_AHRS_Status[AHRS_Trial_dt]);		
//									RGB_Set(Circle);									
									return (AHRS_status = AHRS_OK);
								}
						}
					}
		case AHRS_OK://///////////////////////////////////////////
					if (Get_msTick() - AHRS_Lastime < AHRS_dtms)			return (AHRS_status = AHRS_OK);
					else
					{						
						AHRS_Lastime = Get_msTick();
						if (SENSOR_Update_Data (&Raw_Acce, &Raw_Gyro, &AHRS_dt) != SENSOR_OK)
						{		
//							SerialLink_Console(EventInfoDetail, "%s",String_SENSOR_Status[SENSOR]);
							return (AHRS_status = AHRS_FAIL);
						}
						else if ((AHRS_dt - SampleTime)> SampleTime*AHRS_dt_error)	return (AHRS_status = AHRS_Wrong_dt);
						else
						{
							AHRS_status = AHRS_Task();										
							return AHRS_status;		
						}
					}
		default://///////////////////////////////////////////			
//					RGB_Set(OFF);	
					return (AHRS_status = AHRS_FAIL);
	}			
}


