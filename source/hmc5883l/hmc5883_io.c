#include "../../include/quadcopter.h"

void HMC5883_WriteByte(uint8_t reg, uint8_t byte)
{
	uint8_t buf[2];
	uint8_t n;
	
	buf[0] = reg;
	buf[1] = byte;
	
	n = QC_IO_TwiWriteTo ( HMC5883_I2C_ADDR, buf, 2, true, true );
	if ( n !=0)
		QC_SetLastError( QC_ERROR_TWI_WRITE );
}

uint8_t HMC5883_ReadByte(uint8_t reg)
{
	uint8_t buf = 0xFF;
	uint8_t n;

	n = QC_IO_TwiWriteTo ( HMC5883_I2C_ADDR, &reg, 1, true, false);
	if ( n != 0 )
		QC_SetLastError ( QC_ERROR_TWI_WRITE );

	n = QC_IO_TwiReadFrom ( HMC5883_I2C_ADDR, &buf, 1, true );
	if ( n == 0 )
		QC_SetLastError ( QC_ERROR_TWI_READ );

	return buf;
}
