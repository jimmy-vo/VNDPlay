#include <math.h>
#include "arm_math.h"  
#include "BoardConfig.h"
typedef struct 
{
    int16_t x;
    int16_t y;
    int16_t z;
}int16_3D;

typedef struct 
{
    int64_t x;
    int64_t y;
    int64_t z;
}int64_3D;

typedef struct 
{
    float x;
    float y;
    float z;
}float_3D;

typedef struct 
{
    float32_t Roll;
    float32_t Pitch;
    float32_t Yaw;
}EulerAngle;


typedef struct 
{
    int16_t q1;
    int16_t q2;
    int16_t q3;
    int16_t q4;
}int16_4D;

typedef struct 
{
    float32_t q1;
    float32_t q2;
    float32_t q3;
    float32_t q4;
}Quaternion;

typedef enum 
{	
	IMU_Normalize_Enable,
	IMU_Normalize_Disable,
}IMU_Norm;

typedef enum 
{//IMU_Status
		IMU_NONE									,
		IMU_OK										,
    IMU_Initialize_Fail				,
    IMU_Read_Fail							,		
		///////////////////////////////////////				
}IMU_Status;


/////////////////////////////////////////////////
//Constant
//
#define  DEG_TO_RAD  	0.0174532925f
#define  RAD_TO_DEG  	57.2957795f
/////////////////////////////////////////////////
// Calib Sample
//
#define NM_sample					 	3500
/////////////////////////////////////////////////
//	MPU6050
//
#define EKF_MPU6050_SleepMode							MPU6050_SLEEP_DISABLE
#define EKF_MPU6050_Temperature						MPU6050_TEMPERATURE_ENABLE
#define EKF_MPU6050_Clock									MPU6050_CLOCK_PLL_XGYRO

#define EKF_MPU6050_DLPF_CFG							DLPF_CFG_0
#define EKF_MPU6050_SMPLRT_DIV						0x01

#define EKF_GYRO_FullScaleRange						MPU6050_GYRO_FS_250
#define EKF_GYRO_Sensitivity							131.0f

#define EKF_ACCEL_FullScaleRange					MPU6050_ACCEL_FS_2
#define EKF_ACCEL_Sensitivity							16380.0f
/////////////////////////////////////////////////
//	Macro
//
#define max(x, y) (((x) > (y)) ? (x) : (y))
#define min(x, y) (((x) < (y)) ? (x) : (y))


IMU_Status IMU_Initialize (Quaternion *target, Quaternion *unit, EulerAngle *Angle);
Quaternion IMU_AHRS (int16_3D *Acce, int16_3D *Gyro, Quaternion *Qua, float *AHRS_dt);
EulerAngle IMU_QuaternionToEuler(Quaternion Q);
Quaternion IMU_EulerToQuaternion(EulerAngle Angle);
IMU_Status IMU_Update_Data (int16_3D *Acce, int16_3D *Gyro);
EulerAngle IMU_Compensate(int16_3D *Acce, int16_3D *Gyro,EulerAngle Angle,  float dt);

extern float_3D 		MagScalerGain, MagOffset;

extern float_3D 		EKF_R_Gyro; 
extern Quaternion 	EKF_R_Qua; 
extern float 				EKF_Q_Gyro;  
extern float 				EKF_TimeCst;

extern EulerAngle 	Comp; 

extern float 				SampleTime;

extern float 				AHRS_Kp, AHRS_Ki;
