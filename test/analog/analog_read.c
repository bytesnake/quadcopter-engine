#include "../../include/quadcopter.h"

int main(int argc, const char *argv[])
{
	init ();

	QC_IO_UsbInit ();

	stdout = &QC_UsbSTDOUT;

	uint8_t value;

	for ( ;; )
	{
		value = QC_IO_AnalogRead ( &QC_Pins.A5 );

		printf ( "%u\n", value );
	}

	return 0;
}
