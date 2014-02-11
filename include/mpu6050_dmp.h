#ifndef MPU6050_DMP_H
#define MPU6050_DMP_H

#define MPU6050_DMP_CODE_SIZE       1929    // dmpMemory[]
#define MPU6050_DMP_CONFIG_SIZE     192     // dmpConfig[]
#define MPU6050_DMP_UPDATES_SIZE    47      // dmpUpdates[]

#define MPU6050_DMP_USERBANK 0x20
#define MPU6050_DMP_PREFETCH 0x40

extern const uint8_t dmpMemory[MPU6050_DMP_CODE_SIZE] PROGMEM;
extern const uint8_t dmpConfig[MPU6050_DMP_CONFIG_SIZE] PROGMEM;
extern const uint8_t dmpUpdates[MPU6050_DMP_UPDATES_SIZE] PROGMEM;

#endif
