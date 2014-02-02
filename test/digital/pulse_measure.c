#include "../../include/quadcopter.h"

int main(int argc, const char *argv[])
{
	init ();	

	QC_IO_UsbInit ();

	stdout = &QC_UsbSTDOUT;

	uint64_t width;
	float freq;

	for ( ;; )
	{
		width = QC_MeasurePulseWidth ( &QC_Pins.D5, HIGH, 1000UL*100UL /*10 sec */ );
		freq = 1.0f / (width/1000.0f/1000.0f);

		printf ( "Breite: %luns\tFrequenz: %.3fHz\n", width, freq );
	}

	return 0;
}
