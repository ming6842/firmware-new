
#ifndef FILE_ESTIMATOR_H
#define FILE_ESTIMATOR_H

#include "arm_math.h"
typedef struct vector3d_t {
	float x;
	float y;
	float z;
} vector3d_t;

typedef struct euler_trigonometry_t {
	float C_roll;
	float C_pitch;
	float C_yaw;
	float S_roll;
	float S_pitch;
	float S_yaw;
} euler_trigonometry_t;




#include "attitude_estimator.h"
void inverse_rotation_trigonometry_precal(attitude_t* ,euler_trigonometry_t* );

#endif