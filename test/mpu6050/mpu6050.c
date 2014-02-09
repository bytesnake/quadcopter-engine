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

	c = MPU6050_GetPowerMode ();
	
	if ( c != 0x00 )
		MPU6050_SetPowerMode ( 0x00 );

	MPU6050_SetAccelRange ( MPU6050_ACCEL_4G );
	MPU6050_SetGyroRange ( MPU6050_GYRO_2000ds );

	MPU6050_SetFilterMode ( 0x00 );

	MPU6050_GetGyroOffset ( &offset );

	printf ( "Power Mode at Init: %02X\n\n", c );

	for ( ;; )
	{
		msleep ( 10 );

		MPU6050_ReadValues ( &data, MPU6050_GYRO_2000ds, MPU6050_ACCEL_4G, &offset );
		
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
