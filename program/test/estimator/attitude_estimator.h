/* Attitude filter .h*/

#ifndef __ATTITUDE_ESTIMATOR_H
#define __ATTITUDE_ESTIMATOR_H

#include "imu.h"

uint8_t estimator_trigger_flag;

typedef struct attitude_t
{
	float roll;
	float pitch;
	float yaw;
}attitude_t;

typedef struct vector3d_t
{
	float x;
	float y;
	float z;
} vector3d_t;



float lowpass_float(float* ,float* , float );



#endif