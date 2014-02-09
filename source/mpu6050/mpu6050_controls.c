#include "../../include/quadcopter.h"

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

void MPU6050_SetPowerMode ( uint8_t c ) 
{
        MPU6050_WriteByte ( MPU6050_PWR_MGMT_1, &c );
}

void MPU6050_SetFilterMode ( uint8_t c )
{
	c &= 0b00000111;

	MPU6050_WriteByte ( MPU6050_CONFIG, &c );
}

void MPU6050_SetGyroRange ( uint8_t c )
{
	c = c << 2;

	MPU6050_WriteByte ( MPU6050_GYRO_CONFIG, &c );
}

void MPU6050_SetAccelRange ( uint8_t c )
{
	c = c << 3;

	MPU6050_WriteByte ( MPU6050_ACCEL_CONFIG, &c );
}

void MPU6050_GetGyroOffset ( mpu6050_gyro_offset_t *offset )
{
	MPU6050_Read ( MPU6050_GOFFSET_X, &offset->x );
	MPU6050_Read ( MPU6050_GOFFSET_Y, &offset->y );
	MPU6050_Read ( MPU6050_GOFFSET_Z, &offset->z );

	//TODO!!
}
	
void MPU6050_StartSelfTest ()
{
	uint8_t c = 0b1110000;

	MPU6050_WriteByte ( MPU6050_GYRO_CONFIG, &c );
	MPU6050_WriteByte ( MPU6050_ACCEL_CONFIG, &c );
}

void MPU6050_EnableFifoValues ()
{
	uint8_t c = 0b01110000;

	MPU6050_WriteByte ( MPU6050_FIFO_EN, &c );
}

void MPU6050_DisableFifoValues ()
{
	MPU6050_WriteByte ( MPU6050_FIFO_EN, 0x00 ); //TODO
}


