#include "../../include/quadcopter.h"

void MPU6050_PerformSlaveIO ( uint8_t id, uint8_t mode_addr, uint8_t from, uint8_t desc )
{
	MPU6050_WriteByte ( 0x25 + 3*id, mode_addr );
	MPU6050_WriteByte ( 0x26 + 3*id, from );
	MPU6050_WriteByte ( 0x27 + 3*id, desc );
}

void MPU6050_WriteToSlave ( uint8_t id, uint8_t data )
{
	MPU6050_WriteByte ( 0x63 + id, data );
}

uint8_t MPU6050_ReadFromSlave ( uint8_t id )
{
	uint8_t c;
	
	MPU6050_Read ( 0x49 + id, &c, 1 );

	return c;

	// TODO!
}

uint8_t MPU6050_GetIntStatus ()
{
	uint8_t c;
		
	MPU6050_Read ( MPU6050_INT_STATUS, &c, 1 );

	return c;
}

uint16_t MPU6050_FIFOCount ()
{
	uint8_t a[2];

	MPU6050_Read ( MPU6050_FIFO_COUNTH, &a[0], 1 );
	MPU6050_Read ( MPU6050_FIFO_COUNTL, &a[1], 1 );

	return (a[0] << 8) | a[1];
}

void MPU6050_ReadFIFO ( void *data, uint8_t length )
{
	uint8_t *buf = (uint8_t*) data;

	MPU6050_Read ( MPU6050_FIFO_R_W, buf, length );
}

void MPU6050_GetSelfTestRegister ( mpu6050_selftest_t *test )
{
	uint8_t a, b, c, d;

	MPU6050_Read ( MPU6050_SELFTEST + 0, &a, 1 );
	MPU6050_Read ( MPU6050_SELFTEST + 1, &b, 1 );
	MPU6050_Read ( MPU6050_SELFTEST + 2, &c, 1 );
	MPU6050_Read ( MPU6050_SELFTEST + 3, &d, 1 );

	test->x_accel = ((a << 3) & 0b00011100) | ((d << 4) & 0b00000011);
	test->y_accel = ((b << 3) & 0b00011100) | ((d << 2) & 0b00000011);
	test->z_accel = ((c << 3) & 0b00011100) | ((d << 0) & 0b00000011);

	test->x_gyro = (a & 0b00011111);
	test->y_gyro = (b & 0b00011111);
	test->z_gyro = (c & 0b00011111);
}

uint8_t MPU6050_GetVersion ()
{
        uint8_t c;

        MPU6050_Read ( MPU6050_WHO_AM_I, &c, 1 );

        return c;
}

uint8_t MPU6050_GetPowerMode ()
{
        uint8_t c;
    
        MPU6050_Read ( MPU6050_PWR_MGMT_1, &c, 1 );
    
        return c;
}
