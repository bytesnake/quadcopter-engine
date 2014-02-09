#include "../../include/quadcopter.h"

int main(int argc, const char *argv[])
{
	init ();
	QC_IO_UsbInit ();
	QC_IO_TwiInit ();

	stdout = &QC_UsbSTDOUT;

	MPU6050_SetPowerMode ( 0x00 );
	MPU6050_SetGyroRange ( MPU6050_GYRO_250ds );
	MPU6050_SetAccelRange ( MPU6050_ACCEL_4G );

	MPU6050_StartSelfTest ();

	for ( ;; )
	{
	}

	return 0;
}
