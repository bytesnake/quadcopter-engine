#include "../../include/quadcopter.h"

qc_serial_t QC_InternalSerialOut;
qc_serial_t QC_InternalSerialIn;

FILE QC_InternalSTDOUT = FDEV_SETUP_STREAM(QC_IO_InternSerialWriteStream, NULL, _FDEV_SETUP_WRITE);

void QC_IO_InternSerialInit ()
{
	// Set baudrate
	UBRR1H = UBRRH_VALUE;
	UBRR1L = UBRRL_VALUE;

	#if USE_2X
		UCSR1A |= (1 << U2X1);
	#else
		UCSR1A &=~(1 << U2X1);
	#endif

	// Enable Input/Output
	UCSR1B = (1 << RXEN1) | (1 << TXEN1);

	// Enable interrupts
	UCSR1C |= (1 << RXCIE1) | (1 << TXCIE1);

	QC_InternalSerialOut.tail = QC_InternalSerialIn.tail = 0;
	QC_InternalSerialOut.head = QC_InternalSerialIn.head = 0;
}

bool QC_IO_InternSerialHasData ()
{ 
	return QC_SerialHasData ( &QC_InternalSerialIn );
}

uint8_t QC_IO_InternSerialRead ()
{
	if ( QC_SerialHasData ( &QC_InternalSerialIn ))
		return QC_SerialGetChar ( &QC_InternalSerialIn );
	else
		return 0;
}

void QC_IO_InternSerialWrite ( uint8_t byte )
{
	// TODO: wait if ring buffer has no space to save characters

	QC_SerialPutChar ( &QC_InternalSerialOut, byte );

	// Activate interrupts -> there is something to send
	UCSR1B |= (1 << UDRIE1);
	UCSR1A |= (1 << TXC1);
}

void QC_IO_InternSerialWriteStream ( uint8_t byte, FILE stream )
{
	QC_IO_InternSerialWrite ( byte );
}

ISR(USART1_UDRE_vect)
{
	if ( !QC_SerialHasData ( &QC_InternalSerialOut ))
		UCSR1B &= ~(1 << UDRIE1); // Disable this interrupt
	else
		UDR1 = QC_SerialGetChar ( &QC_InternalSerialOut );
}

ISR(USART1_RX_vect)
{
	if ( bit_is_clear ( UCSR1A, UPE1 )) 
		QC_SerialPutChar ( &QC_InternalSerialIn, UDR1 );
}
