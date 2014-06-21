#ifndef FILE_NAVIGATION_H
#define FILE_NAVIGATION_H

#include "lea6h_ubx.h" // Should link to higher level like GPS.h but so far we have one GPS receiver only.
#include "estimator.h"
#include "bound.h"

#define CONTROLLER_DISABLE 0
#define CONTROLLER_ENABLE 1

typedef struct nav_pid_t {
	float kp,ki,kd;
	vector2d_f_t error;
	vector2d_f_t integral;
	vector2d_i32_t setpoint;
	float output_roll,output_pitch;
	float out_max, out_min;
	float integral_max;
	uint8_t controller_status;

} nav_pid_t;


void PID_Nav(nav_pid_t *,attitude_t *,UBXvelned_t *, UBXposLLH_t *);

#endif