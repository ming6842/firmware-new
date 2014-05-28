#ifndef __IMU_H
#define __IMU_H

#include "mpu9250.h"

#ifdef USE_IMU_MPU9250

#define imu_update() mpu9250_read_accel_temp_gyro()

#endif

typedef struct imu_raw_data_t
{
	int16_t acc[3];
	int16_t gyro[3];
	int16_t temp;
}imu_raw_data_t;

#endif