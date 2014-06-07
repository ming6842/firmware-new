
#include "attitude_stabilizer.h"
void PID_attitude_roll(attitude_stablizer_pid_t* PID_control,imu_data_t* imu_raw_data,attitude_t* attitude){

	float error = (PID_control -> setpoint) - (attitude -> roll);

	float P = error * (PID_control -> kp);

	float D = -(imu_raw_data -> gyro[0]) * (PID_control -> kd);

	(PID_control -> output) = P+D;
 
}


void PID_attitude_pitch(attitude_stablizer_pid_t* PID_control,imu_data_t* imu_raw_data,attitude_t* attitude){

	float error = (PID_control -> setpoint) - (attitude -> pitch);

	float P = error * (PID_control -> kp);

	float D = -(imu_raw_data -> gyro[1]) * (PID_control -> kd);

	(PID_control -> output) = P+D;
 
}

void PID_attitude_yaw(attitude_stablizer_pid_t* PID_control,imu_data_t* imu_raw_data,attitude_t* attitude){

	float error = (PID_control -> setpoint) - (attitude -> yaw);

	float P = error * (PID_control -> kp);

	float D = -(PID_control -> setpoint - imu_raw_data -> gyro[2]) * (PID_control -> kd);

	(PID_control -> output) = P+D;
 
}

void PID_attitude_rc_pass_command(attitude_stablizer_pid_t* PID_roll,attitude_stablizer_pid_t* PID_pitch,attitude_stablizer_pid_t* PID_yaw,radio_controller_t* rc_command){

	PID_roll -> setpoint = rc_command -> roll_control_input;
	PID_pitch -> setpoint = rc_command -> pitch_control_input;
	PID_yaw -> setpoint = rc_command -> yaw_rate_control_input;

}




void PID_output(radio_controller_t* rc_command,attitude_stablizer_pid_t* PID_roll,attitude_stablizer_pid_t* PID_pitch,attitude_stablizer_pid_t* PID_yaw){

motor_output_t motor;

	motor. m1 =0.0;
	motor. m2 =0.0;
	motor. m3 =0.0;
	motor. m4 =0.0;
	motor. m5 =0.0;
	motor. m6 =0.0;
	motor. m7 =0.0;
	motor. m8 =0.0;
	motor. m9 =0.0;
	motor. m10 =0.0;
	motor. m11 =0.0;
	motor. m12 =0.0;
	if( rc_command -> safety == ENGINE_ON) {

	motor . m1 = -10.0f + (rc_command->throttle_control_input) - (PID_roll->output) + (PID_pitch -> output) - (PID_yaw -> output);
	motor . m2 = -10.0f + (rc_command->throttle_control_input) + (PID_roll->output) + (PID_pitch -> output) + (PID_yaw -> output);
	motor . m3 = -10.0f + (rc_command->throttle_control_input) + (PID_roll->output) - (PID_pitch -> output) - (PID_yaw -> output);
	motor . m4 = -10.0f + (rc_command->throttle_control_input) - (PID_roll->output) - (PID_pitch -> output) + (PID_yaw -> output);
	set_pwm_motor(&motor);

	}else{

	motor. m1 =0.0;
	motor. m2 =0.0;
	motor. m3 =0.0;
	motor. m4 =0.0;
	set_pwm_motor(&motor);
	}
}