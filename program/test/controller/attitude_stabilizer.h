#ifndef FILE_ATTITUDE_STABLILZER_H
#define FILE_ATTITUDE_STABLILZER_H

typedef struct attitude_stablizer_pid {
	float kp,ki,kd;
	float integral;
	float output;
	const float out_max, out_min;
	const float integral_max,

} attitude_stablizer_pid;



#endif