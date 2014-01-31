#ifndef QUADCOPTER_H
#define QUADCOPTER_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h> 
#include <math.h>

#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/setbaud.h>
#include <util/delay.h>

#include "qc_datatypes.h"
#include "qc_time.h"
#include "qc_serial.h"
#include "qc_pins.h"

#include "qc_usb.h"
#include "qc_cdc.h"
#include "qc_internserial.h"
#include "qc_wire.h"


#define clockCyclesPerMicrosecond() ( F_CPU / 1000000L )
#define clockCyclesToMicroseconds(a) ( (a) / clockCyclesPerMicrosecond() )
#define microsecondsToClockCycles(a) ( (a) * clockCyclesPerMicrosecond() )

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

#define DEBUG PORTD |= (1<<5); PORTB &= ~(1<<0);
#define DEBUG_OFF PORTD &= ~(1<<5); PORTB |= (1<<0);

uint8_t QC_DisableISR ();
void QC_RestoreISR ( uint8_t sreg );
void init ();

uint64_t QC_MeasurePulseWidth ( qc_pin_t *pin, uint8_t state, uint64_t timeout );

#endif
