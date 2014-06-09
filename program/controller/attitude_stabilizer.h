#ifndef FILE_ATTITUDE_STABLILZER_H
#define FILE_ATTITUDE_STABLILZER_H

#include "imu.h"
#include "attitude_estimator.h"
#include "pwm.h"
#include "radio_control.h"

typedef struct attitude_stablizer_pid_t {
	float kp,ki,kd;
	float integral;
	float setpoint;
	float output;
	const float out_max, out_min;
	const float integral_max;

} attitude_stablizer_pid_t;

void PID_attitude_roll(attitude_stablizer_pid_t* ,imu_data_t* ,attitude_t* );
void PID_attitude_pitch(attitude_stablizer_pid_t* ,imu_data_t* ,attitude_t* );
void PID_attitude_yaw(attitude_stablizer_pid_t* ,imu_data_t* ,attitude_t* );
void PID_output(radio_controller_t* ,attitude_stablizer_pid_t* ,attitude_stablizer_pid_t* ,attitude_stablizer_pid_t* );
void PID_attitude_rc_pass_command(attitude_stablizer_pid_t* ,attitude_stablizer_pid_t* ,attitude_stablizer_pid_t* ,radio_controller_t* );
#endif