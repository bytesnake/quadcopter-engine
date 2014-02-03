#include "../../include/quadcopter.h"

int main(int argc, const char *argv[])
{
	init ();
	QC_IO_UsbInit ();
	QC_IO_TwiInit ();

	stdout = &QC_UsbSTDOUT;

	uint8_t c;

	c = MPU6050_GetPowerMode ();

	if ( c != 0x00 )
		MPU6050_SetPowerMode ( 0x00 );

	printf ( "Power Mode at Init: %02X\n\n", c );

	accel_t_gyro_union data;

	for ( ;; )
	{
		msleep ( 100 );

		MPU6050_Read (MPU6050_ACCEL_XOUT_H, &data, sizeof(data));
		
		if ( QC_GetLastError () != QC_ERROR_SUCCESS ) {
			printf ( "ERROR!\n");
			continue;
		}

		uint8_t swap;

		#define SWAP(x,y) swap = x; x = y; y = swap
		SWAP (data.reg.x_accel_h, data.reg.x_accel_l);
		SWAP (data.reg.y_accel_h, data.reg.y_accel_l);
		SWAP (data.reg.z_accel_h, data.reg.z_accel_l);
		SWAP (data.reg.t_h, data.reg.t_l);
		SWAP (data.reg.x_gyro_h, data.reg.x_gyro_l);
		SWAP (data.reg.y_gyro_h, data.reg.y_gyro_l);
		SWAP (data.reg.z_gyro_h, data.reg.z_gyro_l);
	
		double temp = ( (double) data.value.temperature + 12412.0) / 340.0;

		printf ( "Acce: %.2f G\t%.2f G\t%.2f G\n", 
			((float)data.value.x_accel / 17203.0f),
			((float)data.value.y_accel / 17203.0f), 
			((float)data.value.z_accel / 17203.0f));
	
		printf ( "Gyro: %.2f d/s\t\t%.2f d/s\t\t%.2f d/s\n", 
			(data.value.x_gyro * 0.068f), 
			(data.value.y_gyro * 0.068f),
			(data.value.z_gyro * 0.068f));
		
		printf ( "Temp: %.2f °C\n\n", temp );

	}

	return 0;
}
