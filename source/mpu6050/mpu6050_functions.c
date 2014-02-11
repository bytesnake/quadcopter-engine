#include "../../include/quadcopter.h"

bool MPU6050_PerformSelfTest ()
{
	if ( MPU6050_GetVersion () != MPU6050_I2C_ADDRESS )
		return false;

	// calculate self test response
	mpu6050_values_t data1;
	mpu6050_values_t data2;

	MPU6050_SetAccelConfig ( MPU6050_STARTSELFTEST | MPU6050_ACCEL_8G );
	MPU6050_SetGyroConfig ( MPU6050_STARTSELFTEST | MPU6050_GYRO_250ds );

	msleep ( 200 );

	MPU6050_ReadValues ( &data1, MPU6050_GYRO_NOSCALING, MPU6050_ACCEL_NOSCALING );

	MPU6050_SetAccelConfig ( MPU6050_ACCEL_8G );
	MPU6050_SetGyroConfig ( MPU6050_GYRO_250ds );

	msleep ( 200 );

	MPU6050_ReadValues ( &data2, MPU6050_GYRO_NOSCALING, MPU6050_ACCEL_NOSCALING );
	
	data1.x_gyro -= data2.x_gyro; data1.y_gyro -= data2.y_gyro; data1.z_gyro -= data2.z_gyro;
	data1.x_accel -= data2.x_accel; data1.y_accel -= data2.y_accel; data1.z_accel -= data2.z_accel;

	// calculate factory trim
	mpu6050_selftest_t fm;
	
	MPU6050_GetSelfTestRegister ( &fm );

	float fm_XGYRO = 3275 * pow(1.046f, (fm.x_gyro-1));
	float fm_YGYRO = -3275 * pow(1.046f, (fm.y_gyro-1));
	float fm_ZGYRO = 3275 * pow(1.046f, (fm.z_gyro-1));

	float fm_XACCE = 1392.64f * pow(0.92/0.34, fm.x_accel);
	float fm_YACCE = 1392.64f * pow(0.92/0.34, fm.y_accel);
	float fm_ZACCE = 1392.64f * pow(0.92/0.34, fm.z_accel);
	
	return (abs((data1.x_gyro - fm_XGYRO)/fm_XGYRO) < 0.14 && 
	        abs((data1.y_gyro - fm_YGYRO)/fm_YGYRO) < 0.14 &&
	        abs((data1.z_gyro - fm_ZGYRO)/fm_ZGYRO) < 0.14);
}

void MPU6050_ReadMemoryBlock ( uint8_t *data, uint16_t size, uint8_t bank, uint8_t addr )
{
	uint16_t i;
	uint8_t chunkSize;

	MPU6050_SetMemoryBank ( bank );
	MPU6050_SetMemoryStartAddr ( addr );

	for(i = 0; i < size;)
	{
		chunkSize = MPU6050_DMP_MEMORY_CHUNK_SIZE;
	
		if (i + chunkSize > size) chunkSize = size - i;
		if (chunkSize > 256 - addr) chunkSize = 256 - addr;
	
		MPU6050_Read ( MPU6050_RA_MEM_R_W, data + i, chunkSize );

		i += chunkSize;

		// WTTF ?!?!
		addr += chunkSize;

		// if we aren't done, update bank (if necessary) and address
		if ( i < size ) {
			if ( addr == 0 ) bank ++;
			MPU6050_SetMemoryBank ( bank );
			MPU6050_SetMemoryStartAddr ( addr );
		}
	}
}

void MPU6050_WriteMemoryBlock ( const void *data, uint16_t size, uint8_t bank, uint8_t addr, bool useProgMem)
{
	uint16_t i;
	uint8_t chunkSize, j;
	uint8_t progMemBuffer[MPU6050_DMP_MEMORY_CHUNK_SIZE];

	const uint8_t *buf = (uint8_t*) data;
	
	MPU6050_SetMemoryBank ( bank );
	MPU6050_SetMemoryStartAddr ( addr );

	for ( i = 0; i < size;)
	{
		chunkSize = MPU6050_DMP_MEMORY_CHUNK_SIZE;
		
		if (i + chunkSize > size) chunkSize = size - i;
		if (chunkSize > 256 - addr) chunkSize = 256 - addr;

		// translate to homogene memory
		if ( useProgMem ) {
			j = chunkSize;
			while ( j -- )
				progMemBuffer[j] = pgm_read_byte ( data + i + j );

			MPU6050_Write ( MPU6050_RA_MEM_R_W, &progMemBuffer, chunkSize );
		}
		else
			MPU6050_Write ( MPU6050_RA_MEM_R_W, buf + i, chunkSize );

		i+= chunkSize;

		// Immer noch WTTF
		addr += chunkSize;

		// if we aren't done, update bank (if necessary) and address
		if ( i < size ) {
			if ( addr == 0 ) bank ++;
			MPU6050_SetMemoryBank ( bank );
			MPU6050_SetMemoryStartAddr ( addr );
		}

	}
		
}
