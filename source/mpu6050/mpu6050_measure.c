#include "../../include/quadcopter.h"
	
#define SWAP(x,y) swap = x; x = y; y = swap

void MPU6050_ReadValues (mpu6050_values_t *data, uint8_t gyro_range, uint8_t accel_range ) {
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

	switch ( gyro_range ) {
		case 0x00: gyro_range_f = 1.0f/131.0f; break;
		case 0x01: gyro_range_f = 1.0f/65.5f; break;
		case 0x02: gyro_range_f = 1.0f/32.8f; break;
		case 0x03: gyro_range_f = 1.0f/16.4f; break;
	}

	data->x_gyro = (float) rawdata.value.x_gyro * gyro_range_f;
	data->y_gyro = (float) rawdata.value.y_gyro * gyro_range_f;
	data->z_gyro = (float) rawdata.value.z_gyro * gyro_range_f;

	data->x_accel = (float) rawdata.value.x_accel / 17203.0f * (accel_range + 1);
	data->y_accel = (float) rawdata.value.y_accel / 17203.0f * (accel_range + 1);
	data->z_accel = (float) rawdata.value.z_accel / 17203.0f * (accel_range + 1);
}
