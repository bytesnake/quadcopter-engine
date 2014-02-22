#include "../../include/quadcopter.h"

int main(int argc, const char *argv[])
{
	init ();
	QC_IO_UsbInit ();
	QC_IO_TwiInit ();

	stdout = &QC_UsbSTDOUT;

	MPU6050_Init ();
	
	mpu6050_values_t data;

	for ( ;; )
	{
		msleep ( 10 );

		MPU6050_ReadRawValues ( &data, MPU6050_GYRO_2000ds, MPU6050_ACCEL_2G );
		
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
