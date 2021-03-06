#ifndef HARDWARE_SERIAL_H
#define HARDWARE_SERIAL_H

extern qc_serial_t QC_InternalSerialOut;
extern qc_serial_t QC_InternalSerialIn;

extern FILE QC_InternalSTDOUT;

void QC_IO_InternSerialInit ();
void QC_IO_InternSerialWrite ( uint8_t byte );
void QC_IO_InternSerialWriteStream ( uint8_t byte, FILE stream );
uint8_t QC_IO_InternSerialRead ();
bool QC_IO_InternSerialHasData ();

#endif

