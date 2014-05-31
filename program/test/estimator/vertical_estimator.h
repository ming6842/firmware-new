// vertical_estimator.h
#ifndef __VERTICAL_ESTMATOR_H
#define __VERTICAL_ESTMATOR_H


#include "basic_filter.h"
#include "barometer.h"
#include "attitude_estimator.h"

typedef struct vertical_data
{
	float Z ;
	float Zd ;
	float Zdd ;

}vertical_data;

void vertical_estimator_init(vertical_data* ,vertical_data* );
void vertical_sense(vertical_data* ,vertical_data* ,attitude_t* );
#endif