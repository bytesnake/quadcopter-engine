#include "../include/quadcopter.h"

int main(int argc, const char *argv[])
{
	init ();

	QC_IO_UsbInit ();
	QC_IO_InternSerialInit ();
	QC_SerialInit ();

	stdout = &QC_IO_UsbSTDOUT;

	for ( ;; ) {
		while ( !QC_SerialHasData ( &QC_UsbSerial ));

		uint8_t b = QC_SerialGetChar ( &QC_UsbSerial );

		printf ( "%c", b );

		QC_IO_DigitalWrite ( &QC_Pins.D11 , HIGH );		
	
		/*uint16_t power_16bit = QC_IO_AnalogRead ( &QC_Pins.A5 );
		float power = power_16bit / 1024.0f;

		printf ( "Power: %.2fV\n", power );*/
	}

	return 0;
}
