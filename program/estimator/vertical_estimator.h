// vertical_estimator.h
#ifndef FILE_VERTICAL_ESTMATOR_H
#define FILE_VERTICAL_ESTMATOR_H


#include "basic_filter.h"
#include "barometer.h"
#include "attitude_estimator.h"
#include "estimator.h"
#include "bound.h"
#include <stdlib.h>

typedef struct vertical_data_t
{
	float Z ;
	float Zd ;
	float Zdd ;

}vertical_data_t;

void vertical_estimator_init(vertical_data_t* ,vertical_data_t* );
void vertical_sense(vertical_data_t* ,vertical_data_t* ,imu_data_t* ,euler_trigonometry_t* );
#endif