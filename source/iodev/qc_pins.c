#include "../../include/quadcopter.h"

volatile uint8_t* port_to_mode[] = { 
        0,
	0,
	(uint8_t*) &DDRB,
        (uint8_t*) &DDRC,
        (uint8_t*) &DDRD,
        (uint8_t*) &DDRE,
        (uint8_t*) &DDRF,
};

volatile uint8_t* port_to_output[] = { 
        0,
	0,
	(uint8_t*) &PORTB,
        (uint8_t*) &PORTC,
        (uint8_t*) &PORTD,
        (uint8_t*) &PORTE,
        (uint8_t*) &PORTF,
};

volatile uint8_t* port_to_input[] = { 
        0,
	0,
	(uint8_t*) &PINB,
        (uint8_t*) &PINC,
        (uint8_t*) &PIND,
        (uint8_t*) &PINE,
        (uint8_t*) &PINF,
};

qc_pins_t QC_Pins = {
	{ NOTIMER, 7, 0,	ANALOG,  DISABLED }, // A0
	{ NOTIMER, 6, 0,	ANALOG,  DISABLED }, // A1
	{ NOTIMER, 5, 0,	ANALOG,  DISABLED }, // A2
	{ NOTIMER, 4, 0, 	ANALOG,  DISABLED }, // A3
	{ NOTIMER, 1, 0,	ANALOG,  DISABLED }, // A4
	{ NOTIMER, 0, 0,	ANALOG,  DISABLED }, // A5

	{ NOTIMER, 4, (1 << 2), DIGITAL, DISABLED }, // D0
	{ NOTIMER, 4, (1 << 3), DIGITAL, DISABLED }, // D1
	{ NOTIMER, 4, (1 << 1), DIGITAL, DISABLED }, // D2
	{ TIMER0B, 4, (1 << 0), PWM, 	 DISABLED }, // D3
	{ NOTIMER, 4, (1 << 4), DIGITAL, DISABLED }, // D4
	{ TIMER3A, 3, (1 << 6), PWM, 	 DISABLED }, // D5
	{ TIMER4D, 4, (1 << 7), PWM, 	 DISABLED }, // D6
	{ NOTIMER, 5, (1 << 6), DIGITAL, DISABLED }, // D7
	{ NOTIMER, 2, (1 << 4), DIGITAL, DISABLED }, // D8
	{ TIMER1A, 2, (1 << 5), PWM, 	 DISABLED }, // D9
	{ TIMER1B, 2, (1 << 6), PWM,	 DISABLED }, // D10
	{ TIMER0A, 2, (1 << 7), PWM, 	 DISABLED }, // D11
	{ NOTIMER, 4, (1 << 6), DIGITAL, DISABLED }, // D12
	{ TIMER4A, 3, (1 << 7), PWM, 	 DISABLED }  // D13
};

inline void QC_IO_PWMEnable ( qc_pin_t *pin )
{
	if ( pin->type != PWM || pin->state == PWM)
		return;

	switch ( pin->timer )
	{
		case TIMER0A: TCCR0A |= (1 << COM0A1); break;
		case TIMER0B: TCCR0B |= (1 << COM0B1); break;
		case TIMER1A: TCCR1A |= (1 << COM1A1); break;
		case TIMER1B: TCCR1A |= (1 << COM1B1); break;
		case TIMER3A: TCCR3A |= (1 << COM3A1); break;
		case TIMER4A: 
			TCCR4A |= (1 << COM4A1);
			TCCR4A &=~(1 << COM4D0);
			break;
		case TIMER4D:
			TCCR4C |= (1 << COM4D1);
			TCCR4C &=~(1 << COM4D0);
			break;
	}

	pin->state = PWM;
}

inline void QC_IO_PWMDisable ( qc_pin_t *pin )
{
	if ( pin->type != PWM || pin->state == DISABLED )
		return;

	switch ( pin->timer )
	{
		case TIMER0A: TCCR0A &=~(1 << COM0A1); break;
		case TIMER0B: TCCR0B &=~(1 << COM0B1); break;
		case TIMER1A: TCCR1A &=~(1 << COM1A1); break;
		case TIMER1B: TCCR1A &=~(1 << COM1B1); break;
		case TIMER3A: TCCR3A &=~(1 << COM3A1); break;
		case TIMER4A: TCCR4A &=~(1 << COM4A1); break;
		case TIMER4D: TCCR4C &=~(1 << COM4D1); break;
	}
	

	pin->state = DISABLED;
}

void QC_IO_DigitalEnable ( qc_pin_t *pin, uint8_t type )
{
	if ( pin->type != PWM && pin->type != DIGITAL || pin->state != DISABLED )
		return;

	uint8_t sreg;
	volatile uint8_t *reg;	
	volatile uint8_t *out;

	reg = port_to_mode [ pin->port ];
	out = port_to_output [ pin->port ];	

	if ( type == INPUT ) {
		sreg = QC_DisableISR ();
		*reg &=~pin->bit;
		*out &=~pin->bit;
		QC_RestoreISR ( sreg );
	}
	else if ( type == OUTPUT ) {
		sreg = QC_DisableISR ();
		*reg |= pin->bit;
		QC_RestoreISR ( sreg );
	}
	else 
		return; 

	pin->state = DIGITAL;
}

void QC_IO_DigitalDisable ( qc_pin_t *pin )
{
	if ( pin->type != PWM && pin->type != DIGITAL || pin->state == DISABLED )
		return;

	// anything to do? TODO

	pin->state = DISABLED;
}

uint8_t QC_IO_DigitalRead ( qc_pin_t *pin )
{
	if ( pin->type != PWM && pin->type != DIGITAL )
		return 0;

	if ( pin->type == PWM && pin->state == PWM )
		QC_IO_PWMDisable ( pin );
	else if ( pin->state == DISABLED )
		QC_IO_DigitalEnable ( pin, INPUT );

	if ( *(port_to_input [ pin->port ]) & pin->bit )
		return LOW;
	else
		return HIGH;
}

void QC_IO_DigitalWrite ( qc_pin_t *pin, uint8_t value )
{
	uint8_t sreg;	
	volatile uint8_t *out;

	if ( pin->type != PWM && pin->type != DIGITAL )
		return;

	if ( pin->type == PWM && pin->state == PWM )
		QC_IO_PWMDisable ( pin );
	else if ( pin->state == DISABLED )
		QC_IO_DigitalEnable ( pin, OUTPUT );

	out = port_to_output [ pin->port ];

	sreg = QC_DisableISR ( );

	if ( value == LOW )
		*out &=~pin->bit;
	else
		*out |= pin->bit;

	QC_RestoreISR ( sreg );
}

uint16_t QC_IO_AnalogRead ( qc_pin_t *pin )
{
	uint8_t low, high;

	if ( pin->type != ANALOG )
		return 0;

	// Set the channel	
	ADCSRB = (ADCSRB & ~(1 << MUX5)) | (((pin->port >> 3) & 0x01) << MUX5);

	 // Set the analog reference
	ADMUX = (1 << 6) | (pin->port & 0x07);

	// start conversion
	ADCSRA |= (1 << ADSC);

	// wait until the conversion is finished
	while ( bit_is_set ( ADCSRA, ADSC ));

	// Save the values to memory
	low = ADCL;
	high = ADCH;

	return (high << 8) | low;
}

void QC_IO_AnalogWrite ( qc_pin_t *pin, uint8_t value )
{
	if ( pin->type != PWM )
		return;

	if ( pin->state == DISABLED )
		QC_IO_DigitalEnable ( pin, OUTPUT );

	if ( pin->state == DIGITAL )
		QC_IO_PWMEnable ( pin );

	switch ( pin->timer )
	{
		case TIMER0A: OCR0A = value; break;
		case TIMER0B: OCR0B = value; break;
		case TIMER1A: OCR1A = value; break;
		case TIMER1B: OCR1B = value; break;
		case TIMER3A: OCR3A = value; break;
		case TIMER4A: OCR4A = value; break;
		case TIMER4D: OCR4D = value; break;
		default:
			// this should never happen ... so weird ... so alone ...
			break;
	}

}
