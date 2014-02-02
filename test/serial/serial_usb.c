#include "../../include/quadcopter.h"

int main(int argc, const char *argv[])
{
	init ();
	QC_IO_UsbInit ();

	stdout = &QC_UsbSTDOUT;

	uint8_t byte;

	for ( ;; )
	{
		while ( !QC_SerialHasData ( &QC_UsbSerial ));
		
		byte = QC_SerialGetChar ( &QC_UsbSerial );
		printf ( "%c", byte );
	}	

	return 0;
}
