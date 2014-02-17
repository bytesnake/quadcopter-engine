#include "../../include/quadcopter.h"

void MPU6050_Init ()
{
	// reset entire device
	MPU6050_SetPowerMGMT1 ( MPU6050_DEVICE_RESET );
        msleep ( 30 );	

	// some configurations
	MPU6050_SetPowerMGMT1 ( MPU6050_CLOCK_PLL_ZGYRO );
        MPU6050_SetSampleRateDivider ( 4 ); // 1kHz/(1 + 4) = 200Hz
        MPU6050_SetConfig ( MPU6050_FSYNC_TEMP | 0x03 );        
        MPU6050_SetGyroConfig ( MPU6050_GYRO_2000ds );
        MPU6050_SetAccelConfig ( MPU6050_ACCEL_2G );
}

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

	MPU6050_ReadRawValues ( &data1, MPU6050_GYRO_NOSCALING, MPU6050_ACCEL_NOSCALING );

	MPU6050_SetAccelConfig ( MPU6050_ACCEL_8G );
	MPU6050_SetGyroConfig ( MPU6050_GYRO_250ds );

	msleep ( 200 );

	MPU6050_ReadRawValues ( &data2, MPU6050_GYRO_NOSCALING, MPU6050_ACCEL_NOSCALING );
	
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

void MPU6050_WriteMemoryBlock ( const void *data, uint16_t size, uint8_t bank, uint8_t addr, bool useProgMem)
{
	uint16_t i;
	uint8_t chunkSize, j;
	uint8_t progMemBuffer[MPU6050_DMP_MEMORY_CHUNK_SIZE];
	uint8_t verifyBuffer [MPU6050_DMP_MEMORY_CHUNK_SIZE];

	for ( j = 0; j < MPU6050_DMP_MEMORY_CHUNK_SIZE; j++ )
		verifyBuffer[ j ] = 0;
	
	const uint8_t *buf = (const uint8_t*) data;

	MPU6050_SetMemoryBank ( bank, 0x00 );
	MPU6050_SetMemoryAddr ( addr );

	i = 0;
	while ( i < size )
	{
		chunkSize = MPU6050_DMP_MEMORY_CHUNK_SIZE;
		
		if (i + chunkSize > size) chunkSize = size - i;
		if (chunkSize > 256 - addr) chunkSize = 256 - addr;

		// translate to a homogenues memory block
		if ( useProgMem ) {
			for ( j = 0; j < chunkSize; j++ )
				progMemBuffer[j] = pgm_read_byte ( data + i + j );

			MPU6050_Write ( MPU6050_RA_MEM_R_W, progMemBuffer, chunkSize );
		}
		else
			MPU6050_Write ( MPU6050_RA_MEM_R_W, buf + i, chunkSize );


		/*MPU6050_SetMemoryBank ( bank, 0x00 );
		MPU6050_SetMemoryAddr ( addr );

		MPU6050_Read ( MPU6050_RA_MEM_R_W, verifyBuffer, chunkSize );				

		j = chunkSize+1;
		while ( (j--) > 1 && progMemBuffer[j-1] == verifyBuffer[j-1] );
		
		if ( j > 0 || QC_GetLastError () == QC_ERROR_TWI_WRITE )
			for ( ;; ) printf("ERROR!\n");
		*/

		i += chunkSize;

		addr += chunkSize;

		// if we aren't done, update bank (if necessary) and address
		if ( i < size ) {
			if ( addr == 0 ) bank ++;
			MPU6050_SetMemoryBank ( bank, 0x00 );
			MPU6050_SetMemoryAddr ( addr );
		}

	}
		
}


