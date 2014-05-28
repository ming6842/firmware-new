
#ifndef __MPU9250_H
#define __MPU9250_H


// #define MPU6050_I2C                  I2C2
// #define MPU6050_I2C_RCC_Periph       RCC_APB1Periph_I2C2
// #define MPU6050_I2C_Port             GPIOB
// #define MPU6050_I2C_SCL_Pin          GPIO_Pin_10
// #define MPU6050_I2C_SDA_Pin          GPIO_Pin_11
// #define MPU6050_I2C_RCC_Port         RCC_APB2Periph_GPIOB
//hardware abstraction layer
#define MPU6050_SPI_RCC_Periph SPI4
#define MPU9250_SPI_Port GPIOE
#define MPU9250_SPI_CS_PIN GPIO_Pin_4
#define MPU9250_SPI_MISO_PIN GPIO_Pin_5
#define MPU9250_SPI_MOSI_PIN GPIO_Pin_6
#define MPU9250_SPI_SCK_PIN GPIO_Pin_2

#define MPU9250_INT_ENABLE          ((uint8_t)0x38)
#define MPU9250_INT_STATUS          ((uint8_t)0x3A)
#define MPU9250_ACCEL_XOUT_H        ((uint8_t)0x3B)
#define MPU9250_ACCEL_XOUT_L        ((uint8_t)0x3C)
#define MPU9250_ACCEL_YOUT_H        ((uint8_t)0x3D)
#define MPU9250_ACCEL_YOUT_L        ((uint8_t)0x3E)
#define MPU9250_ACCEL_ZOUT_H        ((uint8_t)0x3F)
#define MPU9250_ACCEL_ZOUT_L        ((uint8_t)0x40)
#define MPU9250_TEMP_OUT_H          ((uint8_t)0x41)
#define MPU9250_TEMP_OUT_L          ((uint8_t)0x42)
#define MPU9250_GYRO_XOUT_H         ((uint8_t)0x43)
#define MPU9250_GYRO_XOUT_L         ((uint8_t)0x44)
#define MPU9250_GYRO_YOUT_H         ((uint8_t)0x45)
#define MPU9250_GYRO_YOUT_L         ((uint8_t)0x46)
#define MPU9250_GYRO_ZOUT_H         ((uint8_t)0x47)
#define MPU9250_GYRO_ZOUT_L         ((uint8_t)0x48)

#define MPU9250_USER_CTRL           ((uint8_t)0x6A)
#define MPU9250_PWR_MGMT_1          ((uint8_t)0x6B)
#define MPU9250_PWR_MGMT_2          ((uint8_t)0x6C)
#define MPU9250_FIFO_COUNTH         ((uint8_t)0x72)
#define MPU9250_FIFO_COUNTL         ((uint8_t)0x73)
#define MPU9250_FIFO_R_W            ((uint8_t)0x74)
#define MPU9250_WHO_AM_I            ((uint8_t)0x75)	// 0x68

uint8_t mpu9250_read_byte(uint8_t );
void mpu9250_write_byte(uint8_t ,uint8_t );
uint8_t mpu9250_read_who_am_i();
void mpu9250_reset();



#endif