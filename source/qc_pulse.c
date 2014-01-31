#include "../include/quadcopter.h"

uint64_t QC_MeasurePulseWidth ( qc_pin_t *pin, uint8_t state, uint64_t timeout )
{
	// Set state
	(void) QC_IO_DigitalRead ( pin );

	uint64_t measured_width = 0;

	unsigned long i = 0;
	unsigned long cycles = microsecondsToClockCycles ( timeout );
 
	while ( !(*(port_to_input [ pin->port ]) & pin->bit))
		if ( ++ i == cycles )
			return 0;

	while ( (*(port_to_input [ pin->port ]) & pin->bit));
		if ( ++ i == cycles )
			return 0;

	while ( !(*(port_to_input [ pin->port ]) & pin->bit))
	{
		if ( ++ i == cycles )
			return 0;

		measured_width ++;
	}

	//printf ( "%lu\n", measured_width );
	return clockCyclesToMicroseconds ( measured_width * 140.16882958424801353471 + 16 );
}

void QC_GeneratePulseUs ( qc_pin_t *pin, uint8_t state, uint16_t width )
{
	QC_IO_DigitalWrite ( pin, state );

	usleep ( width - 8 );
}

void QC_GeneratePulseMs ( qc_pin_t *pin, uint8_t state, uint16_t width )
{
	QC_IO_DigitalWrite ( pin, state );

	msleep ( width );
}
