#include "../../include/quadcopter.h"

uint8_t MPU6050_Read ( uint8_t start, void *buffer, uint8_t size )
{
	uint8_t n, i;
	uint8_t *buf = (uint8_t*) buffer;

	for ( i = 0; i < size; i += MIN(size, TWI_BUFFER_LENGTH ))
	{
		n = QC_IO_TwiWriteTo ( MPU6050_I2C_ADDRESS, &start, 1, true, true );
		if ( n != 0 )
			QC_SetLastError ( QC_ERROR_TWI_WRITE );

		n = QC_IO_TwiReadFrom ( MPU6050_I2C_ADDRESS, buf + i, MIN(size - i, TWI_BUFFER_LENGTH), true );
		if ( n == 0 )
			QC_SetLastError ( QC_ERROR_TWI_READ );
	}

	return n;
}

void MPU6050_Write ( uint8_t start, const void *buffer, uint8_t size )
{
	uint8_t n;
	const uint8_t *buf = (uint8_t*) buffer;

	uint8_t tmp[size+1];

	tmp[0] = start;

	for ( n = 0; n < size; n++ )
		tmp[n+1] = buf[n];

	n = QC_IO_TwiWriteTo ( MPU6050_I2C_ADDRESS, tmp, size+1, true, true );
	if ( n != 0 )
		QC_SetLastError ( QC_ERROR_TWI_WRITE );
}

void MPU6050_WriteByte ( uint8_t start, const uint8_t byte )
{
	uint8_t buf[2];
	uint8_t n;

	buf[0] = start;
	buf[1] = byte;

	n = QC_IO_TwiWriteTo ( MPU6050_I2C_ADDRESS, buf, 2, true, true );
	if ( n != 0 )
		QC_SetLastError ( QC_ERROR_TWI_WRITE );
}
