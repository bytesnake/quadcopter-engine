#include "../../include/quadcopter.h"

uint32_t QC_MeasurePulseWidth ( qc_pin_t *pin, uint8_t state, uint8_t timeout )
{
	// Set state
	(void) QC_IO_DigitalRead ( pin );

	uint32_t width = 0;

	while ( !(*(port_to_input [ pin->port ]) & pin->bit));
	while ( !(*(port_to_input [ pin->port ]) &~pin->bit));

	while ( !(*(port_to_input [ pin->port ]) & pin->bit))
	{
		width ++;
	}

	return clockCyclesToMicroseconds ( width * 21 );
}
