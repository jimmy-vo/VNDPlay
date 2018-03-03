#include <math.h>
#include "arm_math.h"  
#include "BoardConfig.h"

typedef enum 
{
		AHRS_OK													,
    AHRS_NONE 			 								,
    AHRS_LoadParamFail 			 				,
    AHRS_Wrong_dt 			 						,
    AHRS_Trial_dt 			 						,
    AHRS_FAIL 			 								,
}AHRS_Status;

#define AHRS_dt_error		2/5
#define AHRS_TRIAL					5

#define AHRS_Cutoff_HPF_PosEst					0.1f

#define AHRS_Stationary_Thr									0.977f

AHRS_Status AHRS_Process (void);
