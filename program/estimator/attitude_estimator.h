/* Attitude filter .h*/

#ifndef FILE_ATTITUDE_ESTIMATOR_H
#define FILE_ATTITUDE_ESTIMATOR_H

#include "imu.h"
#include "tim.h"

typedef struct attitude_t {
	float roll;
	float pitch;
	float yaw;
} attitude_t;



#include "estimator.h"

void attitude_sense(attitude_t * , imu_data_t * , imu_data_t * , vector3d_t * );
void attitude_estimator_init(attitude_t* ,imu_data_t* , imu_data_t *,vector3d_t* );
void attitude_update(attitude_t *, imu_data_t *, vector3d_t *,imu_unscaled_data_t * ,imu_data_t * ,imu_calibrated_offset_t * );


#endif