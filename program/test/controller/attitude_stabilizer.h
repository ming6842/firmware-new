#ifndef FILE_ATTITUDE_STABLILZER_H
#define FILE_ATTITUDE_STABLILZER_H

#include "imu.h"
#include "attitude_estimator.h"
#include "pwm.h"

typedef struct attitude_stablizer_pid_t {
	float kp,ki,kd;
	float integral;
	float setpoint;
	float output;
	const float out_max, out_min;
	const float integral_max;

} attitude_stablizer_pid_t;

void PID_roll(attitude_stablizer_pid_t* ,imu_raw_data_t* ,attitude_t* );

#endif