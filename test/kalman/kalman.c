#include "../../include/quadcopter.h"

int main(int argc, const char *argv[])
{
	init ();
	QC_IO_UsbInit ();
	QC_IO_TwiInit ();

	stdout = &QC_UsbSTDOUT;

	mpu6050_values_t rawdata;

	math_kalman_data_t finaldataY;
	math_kalman_data_t finaldataP;
	math_kalman_data_t finaldataR;

	float acc_pitch, acc_yaw;

	MPU6050_SetPowerMode ( 0x00 );
	MPU6050_SetGyroRange ( MPU6050_GYRO_2000ds );
	MPU6050_SetAccelRange ( MPU6050_ACCEL_4G );
	MPU6050_SetFilterMode ( 0x02 );
	
	for ( ;; )
	{
		msleep ( 20 );
		
		MPU6050_ReadValues ( &rawdata, MPU6050_GYRO_2000ds, MPU6050_ACCEL_4G );

		if ( QC_GetLastError () != QC_ERROR_SUCCESS ) {
			QC_ResetError ();
			continue;
		}

		acc_pitch = atan2 ( rawdata.x_accel, -rawdata.z_accel );
		acc_yaw = -atan2 ( rawdata.y_accel, -rawdata.z_accel );

		Math_KalmanInvoke ( &finaldataP, acc_pitch, rawdata.x_gyro );
		Math_KalmanInvoke ( &finaldataY, acc_yaw, rawdata.y_gyro );
		// TODO! Kompass needed 
		// Math_KalmanInvoke ( &finaldataR, rawdata.z_gyro );

		printf ( "%.2f\t%.2f\t%.2f\n", finaldataY.x1, finaldataP.x1, finaldataR.x1 );
	}
	
	return 0;
}
