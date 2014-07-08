#ifndef FILE_VERTICAL_STABLILZER_H
#define FILE_VERTICAL_STABLILZER_H


#include "bound.h"
#include "vertical_estimator.h"


#define CONTROLLER_DISABLE 0
#define CONTROLLER_ENABLE 1

typedef struct vertical_pid_t {
	float kp,ki,kd;
	float integral;
	float setpoint;
	float output;
	float out_max, out_min;
	float integral_max;
	uint8_t controller_status;

} vertical_pid_t;

void PID_vertical_Zd(vertical_pid_t* ,vertical_data * );
void PID_vertical_Z(vertical_pid_t* ,vertical_data * );
#endif