#include "../../include/quadcopter.h"

uint8_t MPU6050_Read ( uint8_t start, void *buffer, uint8_t size )
{
	uint8_t n;
	uint8_t *buf = (uint8_t*) buffer;

	n = QC_IO_TwiWriteTo ( MPU6050_I2C_ADDRESS, &start, 1, true, false );
	if ( n != 0xFFFF )
		QC_SetLastError ( QC_ERROR_TWI_WRITE );

	n = QC_IO_TwiReadFrom ( MPU6050_I2C_ADDRESS, buf, size, true );
	if ( n != size )
		QC_SetLastError ( QC_ERROR_TWI_READ );
	
	return n;
}

void MPU6050_Write ( uint8_t start, const void *buffer, uint8_t size )
{
	uint8_t n;
	const uint8_t *buf = (uint8_t*) buffer;

	n = QC_IO_TwiWriteTo ( MPU6050_I2C_ADDRESS, &start, 1, true, false );
	if ( n != 0xFFFF )
		QC_SetLastError ( QC_ERROR_TWI_WRITE );

	n = QC_IO_TwiWriteTo ( MPU6050_I2C_ADDRESS, &buf, size, true, true );
	if ( n != 0xFFFF )
		QC_SetLastError ( QC_ERROR_TWI_WRITE );
}

void MPU6050_WriteByte ( uint8_t start, const uint8_t byte )
{
	uint8_t buf[2];
	uint8_t n;

	buf[0] = start;
	buf[1] = byte;

	n = QC_IO_TwiWriteTo ( MPU6050_I2C_ADDRESS, buf, 2, true, true );
	if ( n != 0xFFFF )
		QC_SetLastError ( QC_ERROR_TWI_WRITE );
}
