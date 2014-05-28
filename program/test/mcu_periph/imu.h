#ifndef __IMU_H
#define __IMU_H

#include "stm32f4xx_conf.h"

#ifdef USE_IMU_MPU9250
#define imu_update(rawdata) mpu9250_read_accel_temp_gyro(rawdata)
#define imu_initialize() mpu9250_initialize_config()
#define imu_scale_data(rawdata,scaled_data,offset) mpu9250_convert_to_scale(rawdata, scaled_data,offset)
#define imu_calibrate_gyro_offset(offset,count) mpu9250_calibrate_gyro_offset(offset,count)
#endif




typedef struct imu_unscaled_data_t
{
	int16_t acc[3];
	int16_t gyro[3];
	int16_t temp;
}imu_unscaled_data_t;


typedef struct imu_raw_data_t
{
	float acc[3];
	float gyro[3];
	float temp;
}imu_raw_data_t;

typedef struct imu_calibrated_offset_t
{
	int16_t acc[3];
	int16_t gyro[3];
}imu_calibrated_offset_t;

#include "mpu9250.h"

#endif