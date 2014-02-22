#include "../../include/quadcopter.h"

int main(int argc, const char *argv[])
{
	init ();
	QC_IO_UsbInit ();
	QC_IO_TwiInit ();

	stdout = &QC_UsbSTDOUT;

	MPU6050_Init ();

	for ( ;; )
	{

		if ( MPU6050_PerformSelfTest () )
			printf("PASS!\n");
		else
			printf(":(((\n");

	}

	return 0;
}
