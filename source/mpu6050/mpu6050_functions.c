#include "../../include/quadcopter.h"

bool MPU6050_PerformSelfTest ()
{
	if ( MPU6050_GetVersion () != MPU6050_I2C_ADDRESS )
		return false;

	// calculate self test response
	mpu6050_values_t data1;
	mpu6050_values_t data2;

	MPU6050_SetAccelConfig ( MPU6050_STARTSELFTEST | MPU6050_ACCEL_8G );
	MPU6050_SetGyroConfig ( MPU6050_STARTSELFTEST | MPU6050_GYRO_250ds );

	msleep ( 200 );

	MPU6050_ReadValues ( &data1, MPU6050_GYRO_NOSCALING, MPU6050_ACCEL_NOSCALING );

	MPU6050_SetAccelConfig ( MPU6050_ACCEL_8G );
	MPU6050_SetGyroConfig ( MPU6050_GYRO_250ds );

	msleep ( 200 );

	MPU6050_ReadValues ( &data2, MPU6050_GYRO_NOSCALING, MPU6050_ACCEL_NOSCALING );
	
	data1.x_gyro -= data2.x_gyro; data1.y_gyro -= data2.y_gyro; data1.z_gyro -= data2.z_gyro;
	data1.x_accel -= data2.x_accel; data1.y_accel -= data2.y_accel; data1.z_accel -= data2.z_accel;

	// calculate factory trim
	mpu6050_selftest_t fm;
	
	MPU6050_GetSelfTestRegister ( &fm );

	float fm_XGYRO = 3275 * pow(1.046f, (fm.x_gyro-1));
	float fm_YGYRO = -3275 * pow(1.046f, (fm.y_gyro-1));
	float fm_ZGYRO = 3275 * pow(1.046f, (fm.z_gyro-1));

	float fm_XACCE = 1392.64f * pow(0.92/0.34, fm.x_accel);
	float fm_YACCE = 1392.64f * pow(0.92/0.34, fm.y_accel);
	float fm_ZACCE = 1392.64f * pow(0.92/0.34, fm.z_accel);
	
	return (abs((data1.x_gyro - fm_XGYRO)/fm_XGYRO) < 0.14 && 
	        abs((data1.y_gyro - fm_YGYRO)/fm_YGYRO) < 0.14 &&
	        abs((data1.z_gyro - fm_ZGYRO)/fm_ZGYRO) < 0.14);
}
