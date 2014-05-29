/* Attitude filter .h*/

#ifndef __ATTITUDE_ESTIMATOR_H
#define __ATTITUDE_ESTIMATOR_H

#include "imu.h"

typedef struct attitude_t {
	float roll;
	float pitch;
	float yaw;
} attitude_t;

typedef struct vector3d_t {
	float x;
	float y;
	float z;
} vector3d_t;



float lowpass_float(float *, float *, float);
void attitude_sense(attitude_t *, imu_raw_data_t *, vector3d_t *, vector3d_t *);


#endif