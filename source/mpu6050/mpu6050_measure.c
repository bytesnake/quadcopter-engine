#include "../../include/quadcopter.h"

void MPU6050_ReadDMPValues ( QUATERN *Q )
{
	uint8_t data[MPU6050_DMP_PACKET_SIZE];

	 while ( MPU6050_FIFOCount() < 42 );

	MPU6050_ReadFIFO ( data, MPU6050_DMP_PACKET_SIZE );

	Q->W = (float)((data[0] << 8) + data[1]) / 16384.0f;
	Q->X = (float)((data[4] << 8) + data[5]) / 16384.0f;
	Q->Y = (float)((data[8] << 8) + data[9]) / 16384.0f;
	Q->Z = (float)((data[12]<< 8) + data[13])/ 16384.0f;
}

#define SWAP(x,y) swap = x; x = y; y = swap

void MPU6050_ReadRawValues (mpu6050_values_t *data, uint8_t gyro_range, uint8_t accel_range, mpu6050_gyro_offset_t *offset ) {
	mpu6050_rawvalues_t rawdata;
	uint8_t swap;
	float gyro_range_f;	

	MPU6050_Read (MPU6050_ACCEL_XOUT_H, &rawdata, sizeof(rawdata));	
	
	SWAP (rawdata.reg.x_accel_h, rawdata.reg.x_accel_l);
	SWAP (rawdata.reg.y_accel_h, rawdata.reg.y_accel_l);
	SWAP (rawdata.reg.z_accel_h, rawdata.reg.z_accel_l);
	SWAP (rawdata.reg.t_h, rawdata.reg.t_l);
	SWAP (rawdata.reg.x_gyro_h, rawdata.reg.x_gyro_l);
	SWAP (rawdata.reg.y_gyro_h, rawdata.reg.y_gyro_l);
	SWAP (rawdata.reg.z_gyro_h, rawdata.reg.z_gyro_l);
	
	data->temperature = ( (float) rawdata.value.temperature + 12412.0) / 340.0;

	switch ( (gyro_range >> 3) ) {
		case 0: gyro_range_f = 1.0f/131.0f; break;
		case 1: gyro_range_f = 1.0f/65.5f; break;
		case 2: gyro_range_f = 1.0f/32.8f; break;
		case 3: gyro_range_f = 1.0f/16.4f; break;
		case 4: gyro_range_f = 1.0f; break;
	}

	data->x_gyro = (float) rawdata.value.x_gyro * gyro_range_f;
	data->y_gyro = (float) rawdata.value.y_gyro * gyro_range_f;
	data->z_gyro = (float) rawdata.value.z_gyro * gyro_range_f;

	data->x_accel = (float) rawdata.value.x_accel / ((accel_range >> 3 != 4 ) ? 16384.0f / ((accel_range >> 3) + 1) : 1.0f);
	data->y_accel = (float) rawdata.value.y_accel / ((accel_range >> 3 != 4 ) ? 16384.0f / ((accel_range >> 3) + 1) : 1.0f);
	data->z_accel = (float) rawdata.value.z_accel / ((accel_range >> 3 != 4 ) ? 16384.0f / ((accel_range >> 3) + 1) : 1.0f);
}
