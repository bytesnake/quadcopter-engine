#ifndef MPU6050_DATATYPES_H
#define MPU6050_DATATYPES_H

typedef union {
struct
  {
    uint8_t x_accel_h;
    uint8_t x_accel_l;
    uint8_t y_accel_h;
    uint8_t y_accel_l;
    uint8_t z_accel_h;
    uint8_t z_accel_l;
    uint8_t t_h;
    uint8_t t_l;
    uint8_t x_gyro_h;
    uint8_t x_gyro_l;
    uint8_t y_gyro_h;
    uint8_t y_gyro_l;
    uint8_t z_gyro_h;
    uint8_t z_gyro_l;
  } reg;
  struct 
  {
    int16_t x_accel;
    int16_t y_accel;
    int16_t z_accel;
    int16_t temperature;
    int16_t x_gyro;
    int16_t y_gyro;
    int16_t z_gyro;
  } value;
} mpu6050_rawvalues_t;

typedef struct
{
	float temperature;
	float x_accel;
	float y_accel;
	float z_accel;
	float x_gyro;
	float y_gyro;
	float z_gyro;
} mpu6050_values_t;

typedef struct
{
	uint8_t x_gyro;
	uint8_t y_gyro;
	uint8_t z_gyro;

	uint8_t x_accel;
	uint8_t y_accel;
	uint8_t z_accel;
} mpu6050_selftest_t;

#endif
