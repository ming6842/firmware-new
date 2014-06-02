
#include "attitude_stabilizer.h"
void PID_roll(attitude_stablizer_pid_t* PID_control,imu_raw_data_t* imu_raw_data,attitude_t* attitude){

	float error = (PID_control -> setpoint) - (attitude -> roll);

	float P = error * (PID_control -> kp);

	float D = (imu_raw_data -> gyro[1]) * (PID_control -> kd);

	(PID_control -> output) = P+D;
 
}


void PID_output(attitude_stablizer_pid_t* PID_roll,attitude_stablizer_pid_t* PID_pitch){




	
}