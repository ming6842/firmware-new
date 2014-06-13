#ifndef FILE_ATTITUDE_STABLILZER_H
#define FILE_ATTITUDE_STABLILZER_H

#include "imu.h"
#include "attitude_estimator.h"
#include "radio_control.h"
#include "bound.h"

typedef struct attitude_stablizer_pid_t {
	float kp,ki,kd;
	float error;
	float integral;
	float setpoint;
	float output;
	float out_max, out_min;
	float integral_max;

} attitude_stablizer_pid_t;

void PID_attitude_roll(attitude_stablizer_pid_t* ,imu_data_t* ,attitude_t* );
void PID_attitude_pitch(attitude_stablizer_pid_t* ,imu_data_t* ,attitude_t* );
void PID_attitude_yaw_rate(attitude_stablizer_pid_t* ,imu_data_t*  );
void PID_attitude_rc_pass_command(attitude_stablizer_pid_t* ,attitude_stablizer_pid_t* ,attitude_stablizer_pid_t* ,radio_controller_t* );
void PID_attitude_heading(attitude_stablizer_pid_t* ,attitude_t* );

#endif