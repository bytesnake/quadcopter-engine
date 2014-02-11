#include "../../include/quadcopter.h"

int main(int argc, const char *argv[])
{
	QC_IO_UsbInit ();
	QC_IO_TwiInit ();

	stdout = &QC_UsbSTDOUT;

	MPU6050_DmpInit ();

	for ( ;; )
	{
		uint16_t count = MPU6050_FIFOCount ();

		printf("%d\n", count);
	}
	return 0;
}
