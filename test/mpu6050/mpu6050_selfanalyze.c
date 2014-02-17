#include "../../include/quadcopter.h"

int main(int argc, const char *argv[])
{
	init ();
	QC_IO_UsbInit ();
	QC_IO_TwiInit ();

	stdout = &QC_UsbSTDOUT;

	MPU6050_SetPowerMGMT1 ( MPU6050_CLOCK_PLL_ZGYRO );
	
	MPU6050_SetGyroConfig ( MPU6050_GYRO_250ds );
	MPU6050_SetAccelConfig ( MPU6050_ACCEL_8G );

	MPU6050_SetConfig ( 0x06 );

	for ( ;; )
	{

		if ( MPU6050_PerformSelfTest () )
			printf("PASS!\n");
		else
			printf(":(((\n");

	}

	return 0;
}
