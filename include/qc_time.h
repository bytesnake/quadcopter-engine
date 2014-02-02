#ifndef QC_TIME
#define QC_TIME

#include <avr/interrupt.h>
#include "./qc_datatypes.h"

uint64_t millis ();
uint64_t micros ();

void msleep ( uint64_t ms );
void usleep ( uint64_t us );

#endif
