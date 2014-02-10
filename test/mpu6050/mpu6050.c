#include "../../include/quadcopter.h"

int main(int argc, const char *argv[])
{
	init ();
	QC_IO_UsbInit ();
	QC_IO_TwiInit ();

	stdout = &QC_UsbSTDOUT;

	uint8_t c;
	mpu6050_gyro_offset_t offset;
	mpu6050_values_t data;

	MPU6050_SetPowerMGMT1 ( MPU6050_CLOCK_PLL_ZGYRO );

	MPU6050_SetAccelConfig ( MPU6050_ACCEL_4G );
	MPU6050_SetGyroConfig ( MPU6050_GYRO_250ds );

	MPU6050_SetConfig ( 0x06 );

	MPU6050_GetGyroOffset ( &offset );

	for ( ;; )
	{
		msleep ( 10 );

		MPU6050_ReadValues ( &data, MPU6050_GYRO_250ds, MPU6050_ACCEL_4G, &offset );
		
		if ( QC_GetLastError () != QC_ERROR_SUCCESS ) {
			QC_ResetError ();
			continue;
		}

		printf ( "%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n", 
			data.x_gyro, data.y_gyro, data.z_gyro,
			data.x_accel,data.y_accel,data.z_accel, atan2 ( data.x_accel, data.z_accel ) / PI * 180,
			-atan2 ( data.y_accel, data.z_accel ) / PI * 180);
	}

	return 0;
}
