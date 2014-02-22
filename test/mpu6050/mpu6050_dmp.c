#include "../../include/quadcopter.h"

int main(int argc, const char *argv[])
{
	init ();

	QC_IO_UsbInit ();
	QC_IO_TwiInit ();

	stdout = &QC_UsbSTDOUT;

	msleep ( 50 );

	MPU6050_Init ();
	
	if ( !MPU6050_PerformSelfTest () ) {
		printf ( "MPU6050 Self-Test not passed! Hardware malfunction! :(\n" );
		for ( ;; );
	}

	MPU6050_DmpInit ();

	MPU6050_SetGyroConfig ( MPU6050_GYRO_2000ds );
        MPU6050_SetAccelConfig ( MPU6050_ACCEL_2G );

	uint16_t count;
	uint8_t intstatus;

	QUATER q;
	VECTOR v;
	
	for ( ;; )
	{
		while(!MPU6050_DmpHasData ());

		intstatus = MPU6050_GetIntStatus ();
		count = MPU6050_FIFOCount ();

		if ( intstatus & 0x10 || count == 1024 ) {
			MPU6050_SetUserControl ( MPU6050_ENABLE_DMP | MPU6050_ENABLE_FIFO | MPU6050_RESET_FIFO );
			printf ( "Reset: %u\n", count);
		}
	
		else if ( intstatus & 0x02 ) {
			MPU6050_ReadDMPValues ( &q );
		
			Q_convert_E ( &q, &v );

			printf("%.2f\t%.2f\t%.2f\n", v.X, v.Y, v.Z);
		}
	}

	return 0;
}
