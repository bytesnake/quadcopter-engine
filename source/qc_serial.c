#include "../include/quadcopter.h"

void QC_SerialAccept ( qc_serial_t *serial, const void *data, uint8_t count )
{
	const uint8_t *d = (const uint8_t*) data;

	while ( count -- )
		QC_SerialPutChar ( serial, *d++ );
}

void QC_SerialPutChar ( qc_serial_t *serial, uint8_t byte )
{
	if ( ++ serial->head == SERIAL_BUFFER_SIZE )
		serial->head = 0;

	serial->buffer [ serial->head ] = byte;
}

uint8_t QC_SerialGetChar ( qc_serial_t *serial )
{
	if ( !QC_SerialHasData ( serial ))
		return 0;

	if ( ++ serial->tail == SERIAL_BUFFER_SIZE )
		serial->tail = 0;

	return serial->buffer [ serial->tail ];
}

bool QC_SerialHasData ( qc_serial_t *serial )
{
	return serial->tail != serial->head;
}

uint8_t QC_SerialSize ( qc_serial_t *serial )
{
	if ( serial->tail > serial->head )
		return SERIAL_BUFFER_SIZE - serial->tail + serial->head;
	else
		return serial->head - serial->tail;
}
