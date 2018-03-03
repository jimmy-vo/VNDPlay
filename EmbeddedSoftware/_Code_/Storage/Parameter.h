#include "BoardConfig.h"

typedef enum
{
	PARAM_OK    						 ,
	PARAM_OverFloat					 ,
	PARAM_ModeConflict			 ,
	PARAM_EEPROM_WRITE_FAIL  ,
	PARAM_EEPROM_READ_FAIL 	 ,
}PARAMETER;

typedef enum
{	
	MAGN_OFFSET_X    			= 1, 
	MAGN_OFFSET_Y    			= 2, 
	MAGN_OFFSET_Z    			= 3,  
	
	MAGN_SCALEGAIN_X    	= 4, 
	MAGN_SCALEGAIN_Y    	= 5, 
	MAGN_SCALEGAIN_Z    	= 6, 
		
	EKF_R_Gyro_x					= 7, 
	EKF_R_Gyro_y    			= 8, 
	EKF_R_Gyro_z    			= 9, 
			
	EKF_R_Qua1						= 10,
	EKF_R_Qua2						= 11, 
	EKF_R_Qua3    				= 12, 
	EKF_R_Qua4   					= 13, 	
	
	EKF_Q_GYRO		    		= 14, 	
	
	EKF_T_cst 						= 15, 
	
	COMP_Roll  						= 16, 
	COMP_Pitch     				= 17, 
	COMP_Yaw     					= 18, 
	
	SAMPLETIME 						= 19, 
	
	AHRS_KP    						= 20, 
	AHRS_KI    						= 21, 
	
	PARA_TYPE_MIN   			= 1, 
	PARA_TYPE_MAX    			= 22, 
	PARA_TYPE_NONE  			= 23, 	
	
	MAGN_OFFSET	    			= 100, 	
	MAGN_SCALEGAIN	    	= 101, 
	EKF_R_Quaternion   		= 102,
	EKF_R_GYRO    				= 103, 
	EKF_R_Gyr_Qua		  	 	= 104,
	COMP		   						= 105,
}PARAM_TYPE;

	#define AddrOffset 3
typedef enum
{
	PARA_MODE_0 =	0,		//  significand bytes 
	PARA_MODE_1 =	1 + AddrOffset,		//1 byte							//+-127
	PARA_MODE_2 =	2 + AddrOffset,		//2 bytes							//+=32767
	PARA_MODE_3 =	3 + AddrOffset,		//3 bytes							//+-8388607
	PARA_MODE_4 =	4 + AddrOffset		//4 bytes							//+-214748364
}BYTE_MODE;

#define PARA_RANGE_MODE_1				127
#define PARA_RANGE_MODE_2				32767
#define PARA_RANGE_MODE_3				8388607
#define PARA_RANGE_MODE_4				2147483647

#define PARA_RANGE_EXP					127
typedef struct 
{
	float	 			*Pointer;
	BYTE_MODE	 	mode;
	uint8_t     *BufAdd;
}FLOAT;

uint8_t PARAM_Buff_READ (PARAM_TYPE type, uint8_t pos);
extern int16_t PARAM_Read (uint8_t 	*Addr);
extern int16_t PARAM_Write (PARAM_TYPE type, uint8_t *Addr);
PARAMETER PARAM_EEPROM_WRITE (PARAM_TYPE Paratype);
PARAMETER PARAM_EEPROM_READ (void);
void Initialize_PARAMETER (void);

