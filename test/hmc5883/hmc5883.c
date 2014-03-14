#include "../../include/quadcopter.h"

int main(int argc, const char *argv[])
{
	init ();
	QC_IO_UsbInit ();
	QC_IO_TwiInit ();
	
	stdout = &QC_UsbSTDOUT;

	MPU6050_Init ();

	uint8_t buf;

	// switch to hmc5883
	MPU6050_SetINTBypassConfig ( MPU6050_I2C_BYPASS_ENABLE );

	// configure
	HMC5883_WriteByte ( 0x00, 0x70 );
	HMC5883_WriteByte ( 0x01, 0xA0 );
	HMC5883_WriteByte ( 0x02, 0x00 );

	// switch back to mpu6050
	MPU6050_SetINTBypassConfig ( 0x00 );

    // configure mpu6050 to read from slave
    MPU6050_PerformSlaveIO ( 0, HMC5883L_DEFAULT_ADDRESS | 0x80 , HMC5883L_RA_DATAX_H , 0x80 | 0x02);
    MPU6050_PerformSlaveIO ( 2, HMC5883L_DEFAULT_ADDRESS | 0x80 , HMC5883L_RA_DATAY_H , 0x80 | 0x02);
    MPU6050_PerformSlaveIO ( 4, HMC5883L_DEFAULT_ADDRESS | 0x80 , HMC5883L_RA_DATAZ_H , 0x80 | 0x02);

    // enable master control
    MPU6050_SetUserControl ( MPU6050_ENABLE_FIFO | MPU6050_ENABLE_DMP | MPU6050_MASTER_CONTROL )


	/* nun musst du deine drei output register vom hmc in den mpu6050 mappen
	 * geht etwa so : http://www.aizac.info/arduino-2560-adk-with-a-9dof-sensor-board-mpu-6050/
	 *
	 * bei uns dann ( für einen ):
	 * MPU6050_PerformSlaveIO ( 0, HMC5883L_DEFAULT_ADDRESS | 0x80 <-- lesen, HMC5883L_RA_DATAX_H <-- register des hmc, 0x80 <-- aktivieren | 0x02 <-- 2byte länge)
	 *
	 * das muss man für jeden machen, wenn man den master control aktiviert hat
	 * MPU6050_SetUserControl ( MPU6050_ENABLE_FIFO | MPU6050_ENABLE_DMP | MPU6050_MASTER_CONTROL )
	 *
	 * kann man dann eine Achse mittels dem mpu6050 lesen:
	 * uint8_t a = MPU6050_ReadFromSlave ( 0 )
     * uint8_t b = MPU6050_ReadFromSlave ( 1 )
	 * uint16_t c = (a << 8) | b;
	 */
	uint8_t a,b;
    for ( ;;)
    {
    a = MPU6050_ReadFromSlave ( 0 )
    b = MPU6050_ReadFromSlave ( 1 )
    uint16_t x = (a << 8) | b;

    a = MPU6050_ReadFromSlave ( 2 )
    b = MPU6050_ReadFromSlave ( 3 )
    uint16_t y = (a << 8) | b;

    a = MPU6050_ReadFromSlave ( 4 )
    b = MPU6050_ReadFromSlave ( 5 )
    uint16_t z = (a << 8) | b;

    printf("%x  |  %y  |  %z \n", x, y, z);
    }

    /* 	for ( ;; )
	*{
	*	buf = HMC5883_ReadByte ( 0x00 );
	*	printf("%02X\n", buf);
    *	if ( buf != 0x70 || QC_GetLastError () != QC_ERROR_SUCCESS)
	*		printf("ERROR!\n");
    *}
	*/
}	

	return 0;
}