#include "../../include/quadcopter.h"

int main(int argc, const char *argv[])
{
	init ();

	QC_IO_UsbInit ();

	for ( ;; )
	{
		QC_GeneratePulseUs ( &QC_Pins.D5, HIGH, 500 );
		QC_GeneratePulseUs ( &QC_Pins.D5, LOW , 500 );
	}	

	return 0;
}
