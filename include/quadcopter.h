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

// even before room and his mathematical structures were some axioms ..
#include "qc_datatypes.h"
#include "math.h"

// without time everyting is really boring ..
#include "qc_time.h"

// now lets do some communication to achieve change ..
#include "qc_serial.h"
#include "qc_pins.h"

#include "qc_usb_datatypes.h"
#include "qc_usb_constants.h"
#include "qc_usb.h"
#include "qc_cdc.h"
#include "qc_internserial.h"
#include "qc_wire.h"

#include "mpu6050_datatypes.h"
#include "mpu6050_constants.h"
#include "mpu6050.h"

#include "hmc5883.h"

#define clockCyclesPerMicrosecond() ( F_CPU / 1000000L )
#define clockCyclesToMicroseconds(a) ( (a) / clockCyclesPerMicrosecond() )
#define microsecondsToClockCycles(a) ( (a) * clockCyclesPerMicrosecond() )

#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) < (y)) ? (y) : (y))

#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

#define DEBUG PORTD |= (1<<5); PORTB &= ~(1<<0);
#define DEBUG_OFF PORTD &= ~(1<<5); PORTB |= (1<<0);

#define PI 3.14

uint8_t QC_DisableISR ();
void QC_RestoreISR ( uint8_t sreg );
void init ();

uint64_t QC_MeasurePulseWidth ( qc_pin_t *pin, uint8_t state, uint64_t timeout );
void QC_GeneratePulseUs ( qc_pin_t *pin, uint8_t state, uint16_t width );
void QC_GeneratePulseMs ( qc_pin_t *pin, uint8_t state, uint16_t width );

#endif
