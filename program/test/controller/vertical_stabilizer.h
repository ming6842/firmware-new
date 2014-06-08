#ifndef FILE_VERTICAL_STABLILZER_H
#define FILE_VERTICAL_STABLILZER_H


#include "bound.h"
#include "vertical_estimator.h"

typedef struct vertical_pid_t {
	float kp,ki,kd;
	float integral;
	float setpoint;
	float output;
	const float out_max, out_min;
	const float integral_max;

} vertical_pid_t;

void PID_vertical_Zd(vertical_pid_t* ,vertical_data * );

#endif