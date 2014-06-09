
#include "attitude_stabilizer.h"

void PID_attitude_roll(attitude_stablizer_pid_t* PID_control,imu_data_t* imu_filtered_data,attitude_t* attitude){

	float error = (PID_control -> setpoint) - (attitude -> roll);

	float P = error * (PID_control -> kp);

	float D = -(imu_filtered_data -> gyro[0]) * (PID_control -> kd);

	(PID_control -> output) = P+D;
 
}


void PID_attitude_pitch(attitude_stablizer_pid_t* PID_control,imu_data_t* imu_filtered_data,attitude_t* attitude){

	float error = (PID_control -> setpoint) - (attitude -> pitch);

	float P = error * (PID_control -> kp);

	float D = -(imu_filtered_data -> gyro[1]) * (PID_control -> kd);

	(PID_control -> output) = P+D;
 
}

void PID_attitude_yaw(attitude_stablizer_pid_t* PID_control,imu_data_t* imu_filtered_data,attitude_t* attitude){

	float error = (PID_control -> setpoint) - (attitude -> yaw);

	float P = error * (PID_control -> kp);

	float D = -(PID_control -> setpoint - imu_filtered_data -> gyro[2]) * (PID_control -> kd);

	(PID_control -> output) = P+D;
 
}


