#include "../../include/quadcopter.h"

int main(int argc, const char *argv[])
{
	init ();
	QC_IO_UsbInit ();

	stdout = &QC_UsbSTDOUT;

	uint8_t byte;

	printf ( "Vor for!\n" );

	for ( ;; )
	{
		while ( !QC_SerialHasData ( &QC_UsbSerialIn ));
		
		byte = QC_SerialGetChar ( &QC_UsbSerialIn );
		printf ( "%c", byte );
	}	

	return 0;
}
