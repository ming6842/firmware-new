
#ifndef FILE_MPU9250_H
#define FILE_MPU9250_H

#include "stm32f4xx_conf.h"
#include "imu.h"
#include "spi.h"


#define MPU9250_SPI SPI4
#define MPU9250_SELECT() 	GPIO_ResetBits(GPIOE,GPIO_Pin_4)
#define MPU9250_DESELECT() 	GPIO_SetBits(GPIOE,GPIO_Pin_4)


#define MPU6050_SPI_RCC_Periph SPI4
#define MPU9250_SPI_Port GPIOE
#define MPU9250_SPI_CS_PIN GPIO_Pin_4
#define MPU9250_SPI_MISO_PIN GPIO_Pin_5
#define MPU9250_SPI_MOSI_PIN GPIO_Pin_6
#define MPU9250_SPI_SCK_PIN GPIO_Pin_2


#define MPU9250_SMPLRT_DIV          ((uint8_t)0x19)
#define MPU9250_CONFIG              ((uint8_t)0x1A)
#define MPU9250_GYRO_CONFIG         ((uint8_t)0x1B)
#define MPU9250_ACCEL_CONFIG        ((uint8_t)0x1C)
#define MPU9250_MOT_THR             ((uint8_t)0x1F)
#define MPU9250_FIFO_EN             ((uint8_t)0x23)

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


#define MPU9250A_2g       ((float)0.0000610352f)  // 0.0000610352 g/LSB
#define MPU9250A_4g       ((float)0.0001220703f)  // 0.0001220703 g/LSB
#define MPU9250A_8g       ((float)0.0002441406f)  // 0.0002441406 g/LSB
#define MPU9250A_16g      ((float)0.0004882813f)  // 0.0004882813 g/LSB

#define MPU9250G_250dps   ((float)0.007633587786f)  // 0.007633587786 dps/LSB
#define MPU9250G_500dps   ((float)0.015267175572f)  // 0.015267175572 dps/LSB
#define MPU9250G_1000dps  ((float)0.030487804878f)  // 0.030487804878 dps/LSB
#define MPU9250G_2000dps  ((float)0.060975609756f)  // 0.060975609756 dps/LSB

#define MPU9250T_85degC   ((float)0.00294f)   // 0.00294 degC/LSB


uint8_t mpu9250_read_byte(uint8_t);
void mpu9250_write_byte(uint8_t , uint8_t);
uint8_t mpu9250_read_who_am_i(void);
void mpu9250_reset(void);
void mpu9250_read_accel_temp_gyro(imu_unscaled_data_t *);
void mpu9250_initialize_config(void);
void mpu9250_convert_to_scale(imu_unscaled_data_t *, imu_data_t *, imu_calibrated_offset_t *);
void mpu9250_delay(uint32_t count);
void mpu9250_calibrate_gyro_offset(imu_calibrated_offset_t *, uint16_t);
void mpu9250_apply_accel_calibration(imu_calibrated_offset_t *);
void mpu9250_initialize_system(imu_calibrated_offset_t *,uint16_t );
void mpu9250_measure_offset(imu_unscaled_data_t *imu_unscaledData,imu_data_t *imu_scaledData);

#endif