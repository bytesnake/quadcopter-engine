#ifndef QC_PORTS_H
#define QC_PORTS_H

#define LOW  0
#define HIGH 1

#define INPUT  0
#define OUTPUT 1
#define PULLUP 2

#define NOTIMER 0
#define TIMER0A 1
#define TIMER0B 2
#define TIMER1A 3
#define TIMER1B 4
#define TIMER2  5
#define TIMER2A 6
#define TIMER2B 7

#define TIMER3A 8
#define TIMER3B 9
#define TIMER3C 10
#define TIMER4A 11
#define TIMER4B 12
#define TIMER4C 13
#define TIMER4D 14      
#define TIMER5A 15
#define TIMER5B 16
#define TIMER5C 17

typedef enum
{
	DISABLED,
	DIGITAL,
	PWM,
	ANALOG,
} qc_pinmode_e;

typedef struct
{
	uint8_t timer;
	uint8_t port;
	uint8_t bit;
	qc_pinmode_e type;
	qc_pinmode_e state;
} qc_pin_t;

typedef struct
{
	qc_pin_t A0;
	qc_pin_t A1;
	qc_pin_t A2;
	qc_pin_t A3;
	qc_pin_t A4;
	qc_pin_t A5;
	
	qc_pin_t D0;
	qc_pin_t D1;
	qc_pin_t D2;
	qc_pin_t D3;
	qc_pin_t D4;
	qc_pin_t D5;
	qc_pin_t D6;
	qc_pin_t D7;
	qc_pin_t D8;
	qc_pin_t D9;
	qc_pin_t D10;
	qc_pin_t D11;
	qc_pin_t D12;
	qc_pin_t D13;
} qc_pins_t;

extern volatile uint8_t *port_to_input[];
extern qc_pins_t QC_Pins;

uint8_t QC_IO_DigitalRead ( qc_pin_t *pin );
void QC_IO_DigitalWrite ( qc_pin_t *pin, uint8_t value );

uint16_t QC_IO_AnalogRead ( qc_pin_t *pin );
void QC_IO_AnalogWrite ( qc_pin_t *pin, uint8_t value );

#endif
