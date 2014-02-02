#include "../../include/quadcopter.h"

int main(int argc, const char *argv[])
{
	init ();

	QC_IO_UsbInit ();

	stdout = &QC_UsbSTDOUT;
	stdin = &QC_UsbSTDIN;

	uint8_t i = 0;
	uint8_t arr[4];
	
	for ( ;; )
	{
		while ( !QC_SerialHasData( &QC_UsbSerial ));
		
		arr[i++] = QC_SerialGetChar ( &QC_UsbSerial );
	
		printf ( "%c", arr[i-1] );

		if ( i == 3 ) {
			arr[i++] = '\0';
			i = 0;

			QC_IO_AnalogWrite ( &QC_Pins.D5, atoi(arr) );

			printf("\n");
		}
	}

	return 0;
}
