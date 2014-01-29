#ifndef SERIAL_H
#define SERIAL_H

#include "quadcopter.h"

#define SERIAL_BUFFER_SIZE 64

typedef struct
{
	uint8_t head;
	uint8_t tail;

	uint8_t buffer[SERIAL_BUFFER_SIZE];
} qc_serial_t;

void QC_SerialInit ();
void QC_SerialAccept ( qc_serial_t *serial, const void *data, uint8_t count );
void QC_SerialPutChar ( qc_serial_t *serial, uint8_t byte );
uint8_t QC_SerialGetChar ( qc_serial_t *serial );
bool QC_SerialHasData ( qc_serial_t *serial );

#endif
