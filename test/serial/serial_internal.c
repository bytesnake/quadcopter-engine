#include "../../include/quadcopter.h"

int main(int argc, const char *argv[])
{
	init ();
	QC_IO_UsbInit ();

	stdout = &QC_InternalSTDOUT;

	uint8_t byte;

	for ( ;; )
	{
		while ( !QC_SerialHasData ( &QC_InternalSerialIn ));
		
		byte = QC_SerialGetChar ( &QC_InternalSerialIn );
		printf ( "%c", byte );
	}	

	return 0;
}
