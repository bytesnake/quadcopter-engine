#ifndef QC_TIME
#define QC_TIME

#include <avr/interrupt.h>
#include "./qc_datatypes.h"

unsigned long millis ();
unsigned long micros ();

void msleep ( unsigned long ms );
void usleep ( unsigned long us );

#endif
