#ifndef MPU6050_H
#define MPU6050_H

extern const uint8_t dmpMemory[MPU6050_DMP_CODE_SIZE] PROGMEM;
extern const uint8_t dmpConfig[MPU6050_DMP_CONFIG_SIZE] PROGMEM;

extern volatile uint8_t data_avaible;

#define MPU6050_SetSampleRateDivider(c) MPU6050_WriteByte ( MPU6050_SMPLRT_DIV, c )
#define MPU6050_SetConfig(c) MPU6050_WriteByte ( MPU6050_CONFIG, c )
#define MPU6050_SetGyroConfig(c) MPU6050_WriteByte ( MPU6050_GYRO_CONFIG, c )
#define MPU6050_SetAccelConfig(c) MPU6050_WriteByte ( MPU6050_ACCEL_CONFIG, c )
#define MPU6050_SetMotionDetectionThreshold(c) MPU6050_WriteByte ( MPU6050_MPT_THRE, c )
#define MPU6050_SetFifoState(c) MPU6050_WriteByte ( MPU6050_FIFO_EN, c )
#define MPU6050_SetI2CMasterControl(c) MPU6050_WriteByte ( MPU6050_I2C_MST_CTRL, c )
#define MPU6050_SetINTBypassConfig(c) MPU6050_WriteByte ( MPU6050_INT_PIN_CFG, c )
#define MPU6050_SetInterruptState(c) MPU6050_WriteByte (MPU6050_INT_ENABLE, c )
#define MPU6050_SetMasterDelayControl(c) MPU6050_WriteByte ( MPU6050_I2C_MST_DELAY_CTRL, c )
#define MPU6050_ResetSignalPath(c) MPU6050_WriteByte ( MPU6050_SIGNAL_PATH_RESET, c )
#define MPU6050_SetMotionDetectionControl(c) MPU6050_WriteByte ( MPU6050_MOT_DETECT_CTRL, c )
#define MPU6050_SetUserControl(c) MPU6050_WriteByte ( MPU6050_USER_CTRL, c )
#define MPU6050_SetPowerMGMT1(c) MPU6050_WriteByte ( MPU6050_PWR_MGMT_1, c )
#define MPU6050_SetPowerMGMT2(c) MPU6050_WriteByte ( MPU6050_PWR_MGMT_2, c )
#define MPU6050_WriteFifo(c) MPU6050_WriteByte ( MPU6050_FIFO_R_W, c )
#define MPU6050_SetOTPBankValid(c) MPU6050_WriteByte ( MPU6050_GOFFSET_X, c & 0x01 )
#define MPU6050_SetSlaveAddr(c) MPU6050_WriteByte ( 0x25, c)
#define MPU6050_SetDMPConfig1(c) MPU6050_WriteByte ( MPU6050_RA_DMP_CFG_1, c)
#define MPU6050_SetMemoryAddr(c) MPU6050_WriteByte ( MPU6050_RA_MEM_START_ADDR, c)
#define MPU6050_SetMemoryBank(c) MPU6050_WriteByte ( MPU6050_RA_BANK_SEL, c & 0x1F)

// IO
uint8_t MPU6050_Read ( uint8_t reg, void *buffer, uint8_t size );
void MPU6050_Write ( uint8_t reg, const void *buffer, uint8_t size );
void MPU6050_WriteByte ( uint8_t reg, uint8_t byte );

// Basic register functions
void MPU6050_PerformSlaveIO ( uint8_t id, uint8_t mode_addr, uint8_t from, uint8_t desc );
void MPU6050_WriteToSlave ( uint8_t id, uint8_t data );
uint8_t MPU6050_ReadFromSlave ( uint8_t id );
uint16_t MPU6050_FIFOCount ();
void MPU6050_ReadFIFO ( void *buf, uint8_t size );
void MPU6050_GetSelfTestRegister ( mpu6050_selftest_t *test );
uint8_t MPU6050_GetVersion ();
uint8_t MPU6050_GetPowerMode ();
uint8_t MPU6050_GetINTStatus ();

// More advanced functions
void MPU6050_Init ();
bool MPU6050_PerformSelfTest ();
void MPU6050_WriteMemoryBlock ( const void *data, uint16_t size, uint8_t bank, uint8_t addr, bool useProgMem );

// DMP ...
void MPU6050_DmpInit ();
uint16_t MPU6050_WriteDMP ( const void *data, uint16_t i, bool useProgMem );
void MPU6050_WriteDMPConfig ( const void *data, uint16_t size, bool useProgMem );
uint8_t MPU6050_DmpHasData ();

// The real important functions
void MPU6050_ReadDMPValues ( QUATER *Q );
void MPU6050_ReadRawValues (mpu6050_values_t *data, uint8_t gyro_range, uint8_t accel_range );

#endif
