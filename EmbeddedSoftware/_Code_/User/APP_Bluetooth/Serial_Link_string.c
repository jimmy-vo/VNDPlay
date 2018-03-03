#include "Serial_Link_string.h"


char *String_I2C_Status[] = 
{	
		"I2C OK",
		"I2C Busy",	
		"I2C Timeout"	
};

char *String_HMC5883_Status[] = 
{	
		"HMC5883: OK",
		"HMC5883: Busy",	
		"HMC5883: Request Fail",	
		"HMC5883: Timeout"	
};

char *String_MPU6050_Status[] = 
{	
		"MPU6050: OK",
		"MPU6050: Busy",	
		"MPU6050: Request Fail",	
		"MPU6050: Timeout"	
};

char *String_EPPROM_Status[] = 
{
		"EPPROM: OK",
		"EPPROM: Busy",	
		"EPPROM: Request Fail",	
		"EPPROM: Timeout"	
};

char *String_PARAMETER_Status[] = 
{//PARAMETER				
	"PARAM: OK"    						,
	"PARAM: OverFloat"					,
	"PARAM: ModeConflict"			,
	"PARAM: EEPROM |-> Write Fail" ,
	"PARAM: EEPROM |-> Read Fail" ,
};

char *String_EKF_Status[] = 
{//EKF_Status			
	"EKF: OK"    									,
	"EKF: Fail to initialize"			,
	"EKF: Fail to load parameter"	,
	"EKF: Wrong Sampling time detected!"	,
	"EKF: Sampling time trial OK"	,
	"EKF: Fail"										,
};

char *String_AHRS_Status[] = 
{//AHRS_Status			
	"AHRS: OK"    									,
	"AHRS: Fail to initialize"			,
	"AHRS: Fail to load parameter"	,
	"AHRS: Wrong Sampling time detected!"	,
	"AHRS: Sampling time trial OK"	,
	"AHRS: Fail"										
};

char *String_IMU_Status[] = 
{//IMU_Status	
	"SENSOR: OK"													,
	///////////////////////////////////////
	"SENSOR: Setup |-> HMC5883 Fail"					,
	"SENSOR: Setup |-> MPU6050 Fail"					,
	///////////////////////////////////////		
	"SENSOR: Mag Gain Calib |-> PosBias Config Fail",
	"SENSOR: Mag Gain Calib |-> PosBias Read Fail"  ,
	"SENSOR: Mag Gain Calib |-> PosBias Timeout"		,
	
	"SENSOR: Mag Gain Calib |-> NegBias Config Fail",
	"SENSOR: Mag Gain Calib |-> NegBias Read Fail"  ,
	"SENSOR: Mag Gain Calib |-> NegBias Timeout"		,
	
	"SENSOR: Mag Gain Calib |-> Save Fail"			,
	///////////////////////////////////////		
	"SENSOR: Mag Offset Calib |-> Config Fail"			,
	"SENSOR: Mag Offset Calib |-> Read Fail"  			,
	"SENSOR: Mag Offset Calib |-> Timeout"					,
	///////////////////////////////////////	
	"SENSOR: Estimate Variance |-> Read fail",
	"SENSOR: Estimate Variance |-> Save fail",
	///////////////////////////////////////			 					,
	"SENSOR: READ |-> HMC5883 Fail"		 					,
	"SENSOR: READ |-> MPU6050 Fail"							,		
};

char *String_PARAMETER_Type[] = 
{//PARAM_TYPE
	"TYPE 0",	
	
	"Magn |-> OffsetX", 
	"Magn |-> OffsetY", 
	"Magn |-> OffsetZ", 
	
	"Magn |-> GainX", 
	"Magn |-> GainY", 
	"Magn |-> GainZ", 
	
	"EKF |-> R GyroX", 
	"EKF |-> R GyroY", 
	"EKF |-> R GyroZ", 	
	"EKF |-> R Qua1",
	"EKF |-> R Qua2",
	"EKF |-> R Qua3",
	"EKF |-> R Qua4",	
	"EKF |-> Q Gyro",
	
	"COMP |-> Roll",
	"COMP |-> Pitch",
	"COMP |-> Yaw",
	
	"Sample Time",
	
	"AHRS |-> Kp",
	"AHRS |-> Ki",	
};

char * String_FATFS_Status[]=
{//FRESULT
	"FATFS: Succeeded",								
	"FATFS: A hard error occured in the low level disk I/O layer",			
	"FATFS: Assertion failed",					
	"FATFS: The physical drive cannot work",					
	"FATFS: Could not find the file",					
	"FATFS: Could not find the path ",						
	"FATFS: The path name format is invalid",		
	"FATFS: Acces denied due to prohibited access or directory full",
	"FATFS: Acces denied due to prohibited access",								
	"FATFS: The file/directory object is invalid",	
	"FATFS: The physical drive is write protected",	
	"FATFS: The logical drive number is invalid",				
	"FATFS: The volume has no work area ",				
	"FATFS: There is no valid FAT volume ",
	"FATFS: The f_mkfs() aborted due to any parameter error ",
	"FATFS: Could not get a grant to access the volume within defined period",
	"FATFS: The operation is rejected according to the file shareing policy",
	"FATFS: LFN working buffer could not be allocated",
	"FATFS: Number of open files > _FS_SHARE",
	"FATFS: Given parameter is invalid"
};
