#include "../include/quadcopter.h"

int main(int argc, const char *argv[])
{
	init ();

	QC_IO_UsbInit ();
	QC_IO_InternSerialInit ();

	stdout = &QC_UsbSTDOUT;

	uint64_t width;

	float freq;

	for ( ;; ) {
		//QC_GeneratePulseUs ( &QC_Pins.D5, HIGH, 500 );
		//QC_GeneratePulseUs ( &QC_Pins.D5, LOW , 500 );
		width = QC_MeasurePulseWidth ( &QC_Pins.D5, HIGH, 1000*10*1000UL);
		freq = 1.0f / (width/1000.0f/1000.0f);

		printf ( "%lu\n",  width );
		printf ( "%f\n", freq );

		/*uint16_t power_16bit = QC_IO_AnalogRead ( &QC_Pins.A5 );
		float power = power_16bit / 1024.0f;

		printf ( "Power: %.2fV\n", power );*/
	}

	return 0;
}
