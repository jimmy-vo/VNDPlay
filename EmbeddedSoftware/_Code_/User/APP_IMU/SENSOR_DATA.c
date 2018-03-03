#include "MPU6050.h"
#include "Timing_Manage.h"
#include "Dev_DIGITAL.h"
#include "SENSOR_DATA.h"
#include "Serial_Link.h"
#include "Parameter.h"
#include "Dev_I2C.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "ff.h"


#define EKF_RESTRICT_PITCH

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////Variable
//
static float beta = 0.05f;								// 2 * proportional gain (Kp)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////EKF	Driver
//
static void IMU_delay(u32 nCount)
{
	Delay_ms(nCount);
}

IMU_Status Initialize_IMU_Bus (void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	/* GPIO Periph clock enable */
	RCC_AHB1PeriphClockCmd(I2C1_GPIO_CLK, ENABLE);

	/* Configure I2C2 pins: SCL and SDA */
	GPIO_InitStructure.GPIO_Pin 		=  	I2C1_SCL_PIN | I2C1_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode 		= 	GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType 	= 	GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd  	= 	GPIO_PuPd_NOPULL;
	GPIO_Init(I2C1_GPIO_PORT, &GPIO_InitStructure);

	/* Connect GPIO pins to peripheral */
	GPIO_PinAFConfig(I2C1_GPIO_PORT, I2C1_SCL_SOURCE, I2C1_AF); 
	GPIO_PinAFConfig(I2C1_GPIO_PORT, I2C1_SDA_SOURCE, I2C1_AF);

	IMU.TX.DMA									= I2C_DMA_Enable;
	IMU.TX.DMA_Channel					=	I2C1_TX_DMA_CH;
	IMU.TX.DMA_Stream						=	I2C1_TX_DMA_ST;
	IMU.TX.DMA_Flag							=	I2C1_TX_DMA_FLAG;
	IMU.TX.DMA_NVIC_IRQ					=	I2C1_TX_DMA_IRQn;
	IMU.TX.SubPri								=	NVIC_Subpriority_SENSOR;
	IMU.TX.preemtionPri					=	NVIC_Prepriority_SENSOR;

	IMU.RX.DMA									= I2C_DMA_Enable;
	IMU.RX.DMA_Channel					=	I2C1_RX_DMA_CH;
	IMU.RX.DMA_Stream						=	I2C1_RX_DMA_ST;
	IMU.RX.DMA_Flag							=	I2C1_RX_DMA_FLAG;
	IMU.RX.DMA_NVIC_IRQ					=	I2C1_RX_DMA_IRQn;
	IMU.TX.SubPri								=	NVIC_Subpriority_SENSOR;
	IMU.TX.preemtionPri					=	NVIC_Prepriority_SENSOR+1;
	
	IMU.TimeOut 									= 100;		//10ms
	IMU.I2C											= I2C1;		
	
	Initialize_I2C(&IMU,400000);			//400kHz
			
	IMU_delay(100);		
	

	if(MPU6050_ConnectionCheck()	!=	MPU6050_STATUS_OK)	
		return IMU_Initialize_Fail;	
		
	else
		{	 				
		if	(MPU6050_Set_PWR_MGMT_1(EKF_MPU6050_SleepMode, EKF_MPU6050_Temperature, EKF_MPU6050_Clock)!= MPU6050_STATUS_OK)
			return IMU_Initialize_Fail;		
			
		if	(	MPU6050_Set_GYRO_Config(EKF_GYRO_FullScaleRange)!= MPU6050_STATUS_OK) 
			return IMU_Initialize_Fail;		
			
		if	(MPU6050_Set_ACCEL_Config(EKF_ACCEL_FullScaleRange)!= MPU6050_STATUS_OK) 
			return IMU_Initialize_Fail;		
			
		if	(MPU6050_Set_Configuration(EKF_MPU6050_DLPF_CFG)!= MPU6050_STATUS_OK)	
			return IMU_Initialize_Fail;		
			
		if	(	MPU6050_Set_SMPLRT_DIV(EKF_MPU6050_SMPLRT_DIV)!= MPU6050_STATUS_OK)	
			return IMU_Initialize_Fail;		
		}	
		
	return IMU_OK;
}

IMU_Status IMU_Update_Data (int16_3D *Acce, int16_3D *Gyro)
{
	/////////////////////////////////////////////////
	//	Sampling
	//
	if (MPU6050_Get_rawMotion (&Gyro->x, &Gyro->y, &Gyro->z, &Acce->x, &Acce->y, &Acce->z) != MPU6050_STATUS_OK)
		return IMU_Read_Fail;
	/////////////////////////////////////////////////
	//	Gyro
	//Convert to degree/s
	Gyro->x	/= EKF_GYRO_Sensitivity;
	Gyro->y	/= EKF_GYRO_Sensitivity;
	Gyro->z	/= EKF_GYRO_Sensitivity;	
	
	return IMU_OK;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////SENSORFormulas
//
Quaternion IMU_EulerToQuaternion(EulerAngle Angle)
{
	Quaternion Q;

  double Roll  =  (double)(Angle.Roll	* DEG_TO_RAD / 2);
  double Pitch =  (double)(Angle.Pitch * DEG_TO_RAD / 2);
  double Yaw   =  (double)(Angle.Yaw * DEG_TO_RAD / 2);
	
	Q.q1 = cos(Roll)*cos(Pitch)*cos(Yaw) + sin(Roll)*sin(Pitch)*sin(Yaw);
	Q.q2 = sin(Roll)*cos(Pitch)*cos(Yaw) - cos(Roll)*sin(Pitch)*sin(Yaw);
	Q.q3 = cos(Roll)*sin(Pitch)*cos(Yaw) + sin(Roll)*cos(Pitch)*sin(Yaw);
	Q.q4 = cos(Roll)*cos(Pitch)*sin(Yaw) - sin(Roll)*sin(Pitch)*cos(Yaw);
	
	return Q;
}

EulerAngle IMU_QuaternionToEuler(Quaternion Q)
{    
	EulerAngle Angle;
		
	Angle.Roll  	= atan2 (2*(Q.q1*Q.q2 + Q.q3*Q.q4), 1 - 2*(Q.q2*Q.q2 + Q.q3*Q.q3)) 	* RAD_TO_DEG ;
  Angle.Pitch  	= asin  (-2*(Q.q2*Q.q4 - Q.q1*Q.q3)) 																* RAD_TO_DEG ;
  Angle.Yaw 		= atan2 (2*(Q.q1*Q.q4 + Q.q2*Q.q3), 1 - 2*(Q.q3*Q.q3 + Q.q4*Q.q4)) 	* RAD_TO_DEG ;
	return Angle;
}

EulerAngle IMU_Compensate(int16_3D *Acce, int16_3D *Gyro, EulerAngle Angle,  float dt)
{
	EulerAngle tmp, Comp;	
#ifdef EKF_RESTRICT_PITCH
	tmp.Roll = atan2(Acce->y, Acce->z) ;
	tmp.Pitch = atan(-Acce->x / sqrt(Acce->y * Acce->y + Acce->z * Acce->z)) ;
#else
	tmp.Roll = atan(Acce->y / sqrt(Acce->x * Acce->x + Acce->z * Acce->z)) ;
	tmp.Pitch = atan2(-Acce->x, Acce->z) ;
#endif
	
	tmp.Roll    = tmp.Roll 	* RAD_TO_DEG	;
	tmp.Pitch   = tmp.Pitch * RAD_TO_DEG;
	tmp.Yaw     = tmp.Yaw 	* RAD_TO_DEG 	;
	
	Comp.Roll = Comp.Pitch = Comp.Yaw = 0.98f;
	
	// Calculate the angle using a Complimentary filter	
	if (abs(tmp.Yaw - Angle.Yaw) >90)//fix accidental transition yaw
	{			
		Angle.Roll 	= Comp.Roll 	* (Angle.Roll 	+ Gyro->x * dt) 	+ (1-Comp.Roll) * tmp.Roll;
		Angle.Pitch = Comp.Pitch 	* (Angle.Pitch 	+ Gyro->y * dt) 	+ (1-Comp.Pitch)* tmp.Pitch;
		Angle.Yaw  -= (float)Gyro->z 			* dt;
	}
	else
	{
		Angle.Roll 	= Comp.Roll 	* (Angle.Roll 	+ Gyro->x * dt) 	+ (1-Comp.Roll) * tmp.Roll;
		Angle.Pitch = Comp.Pitch 	* (Angle.Pitch 	+ Gyro->y * dt) 	+ (1-Comp.Pitch)* tmp.Pitch;
		Angle.Yaw		= Comp.Yaw 		* (Angle.Yaw		+ Gyro->z * dt) 	+ (1-Comp.Yaw)  * tmp.Yaw ;
	}
	return Angle;
}

Quaternion IMU_AHRS (int16_3D *Acce, int16_3D *Gyro, Quaternion *Qua, float *AHRS_dt)
{
	float norm;	
	
	float qDot1, qDot2, qDot3, qDot4;
	
	float ax, ay, az;
	float gx, gy, gz;		
	float q1 = Qua->q1, q2 = Qua->q2, q3 = Qua->q3, q4 = Qua->q4;   // short name local variable for readability
	float	q1q1, q2q2, q3q3, q4q4;
	float _2q4, _2q1, _2q2, _2q3, _4q3, _4q1, _4q2 ,_8q3, _8q2;	
	float s4, s1, s2, s3;
	
	// Update Data
	ax = Acce->x; 						ay = Acce->y; 						az = Acce->z; 
	gx = Gyro->x*DEG_TO_RAD; 	gy = Gyro->y*DEG_TO_RAD; 	gz = Gyro->z*DEG_TO_RAD; 

	// Rate of change of quaternion from gyroscope
	qDot1 = 0.5f * (-q2 * gx - q3 * gy - q4 * gz);
	qDot2 = 0.5f * (q1 * gx + q3 * gz - q4 * gy);
	qDot3 = 0.5f * (q1 * gy - q2 * gz + q4 * gx);
	qDot4 = 0.5f * (q1 * gz + q2 * gy - q3 * gx);

	// Normalise accelerometer measurement
	norm = (float)sqrt(ax * ax + ay * ay + az * az);
	if (norm == 0.0f) return *Qua; // handle NaN
	norm = 1 / norm;        // use reciprocal for division
	ax *= norm;
	ay *= norm;
	az *= norm;

	// Auxiliary variables to avoid repeated arithmetic
	_2q1 = 2.0f * q1;
	_2q2 = 2.0f * q2;
	_2q3 = 2.0f * q3;
	_2q4 = 2.0f * q4;
	_4q1 = 4.0f * q1;
	_4q2 = 4.0f * q2;
	_4q3 = 4.0f * q3;
	_8q2 = 8.0f * q2;
	_8q3 = 8.0f * q3;
	q1q1 = q1 * q1;
	q2q2 = q2 * q2;
	q3q3 = q3 * q3;
	q4q4 = q4 * q4;

	// Gradient decent algorithm corrective step	
	s1 = _4q1 * q3q3 + _2q3 * ax + _4q1 * q2q2 - _2q2 * ay;
	s2 = _4q2 * q4q4 - _2q4 * ax + 4.0f * q1q1 * q2 - _2q1 * ay - _4q2 + _8q2 * q2q2 + _8q2 * q3q3 + _4q2 * az;
	s3 = 4.0f * q1q1 * q3 + _2q1 * ax + _4q3 * q4q4 - _2q4 * ay - _4q3 + _8q3 * q2q2 + _8q3 * q3q3 + _4q3 * az;
	s4 = 4.0f * q2q2 * q4 - _2q2 * ax + 4.0f * q3q3 * q4 - _2q3 * ay;
	
	// normalise step magnitude
	norm = (float) sqrt( s1 * s1 + s2 * s2 + s3 * s3 + s4 * s4); 
	if (norm == 0.0f) return *Qua; // handle NaN
	norm = 1 / norm;        // use reciprocal for division
	s1 *= norm;
	s2 *= norm;
	s3 *= norm;
	s4 *= norm;

	// Apply feedback step
	qDot1 -= beta * s1;
	qDot2 -= beta * s2;
	qDot3 -= beta * s3;
	qDot4 -= beta * s4;

	
	// Integrate rate of change of quaternion to yield quaternion
	q1 += qDot1 * (*AHRS_dt);
	q2 += qDot2 * (*AHRS_dt);
	q3 += qDot3 * (*AHRS_dt);
	q4 += qDot4 * (*AHRS_dt);

	
	// Normalise quaternion
	norm = (float)sqrt(q1 * q1 + q2 * q2 + q3 * q3 + q4 * q4);
	if (norm == 0.0f) return *Qua; // handle NaN
	norm = 1.0f / norm;
	Qua->q1 = q1 * norm;
	Qua->q2 = q2 * norm;
	Qua->q3 = q3 * norm;
	Qua->q4 = q4 * norm;

	return *Qua;
}


IMU_Status IMU_Initialize (Quaternion *target, Quaternion *unit, EulerAngle *Angle)
{
	if (Initialize_IMU_Bus() != IMU_OK) 	return IMU_Initialize_Fail;
	else 		
	{		
		target->q1 = unit->q1 =  1;
		target->q2 = unit->q2 =  0;
		target->q3 = unit->q3 =  0;
		target->q4 = unit->q4 =  0;
		
		Angle->Pitch = Angle->Roll = Angle->Yaw =0;		
		
		return IMU_OK;
	}		
}



