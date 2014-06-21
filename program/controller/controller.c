
#include "controller.h"

void PID_rc_pass_command(attitude_t* attitude,attitude_stablizer_pid_t* PID_roll,attitude_stablizer_pid_t* PID_pitch,attitude_stablizer_pid_t* PID_heading,vertical_pid_t* PID_Z,vertical_pid_t* PID_Zd,nav_pid_t* PID_nav,radio_controller_t* rc_command){

	PID_roll -> setpoint = rc_command -> roll_control_input;
	PID_pitch -> setpoint = rc_command -> pitch_control_input;

	if( rc_command -> safety == ENGINE_ON) {

		PID_heading -> setpoint = (PID_heading -> setpoint)+ (rc_command -> yaw_rate_control_input)*CONTROL_DT;

		if((PID_heading -> setpoint ) > 360.0f){

			PID_heading -> setpoint = PID_heading -> setpoint - 360.0f;
		}else if((PID_heading -> setpoint ) <0.0f){

			PID_heading -> setpoint = PID_heading -> setpoint + 360.0f;

		}

	}else{

		PID_heading -> setpoint = attitude -> yaw;
	}


	if((rc_command -> mode) == MODE_3){

		PID_Z -> controller_status = CONTROLLER_ENABLE ;
		PID_Zd -> controller_status = CONTROLLER_ENABLE ;
		PID_nav -> controller_status = CONTROLLER_ENABLE;

	}else if((rc_command -> mode) == MODE_2){

		PID_Z -> controller_status = CONTROLLER_ENABLE ;
		PID_Zd -> controller_status = CONTROLLER_ENABLE ;
		PID_nav -> controller_status = CONTROLLER_DISABLE;

	}else{ // MODE_1

		PID_Z -> controller_status = CONTROLLER_DISABLE ;
		PID_Zd -> controller_status = CONTROLLER_DISABLE ;
		PID_nav -> controller_status = CONTROLLER_DISABLE;

	}

}


void PID_init(attitude_stablizer_pid_t* PID_roll,attitude_stablizer_pid_t* PID_pitch,attitude_stablizer_pid_t* PID_yaw_rate,attitude_stablizer_pid_t* PID_heading,vertical_pid_t* PID_Z,vertical_pid_t* PID_Zd,nav_pid_t* PID_nav){


	PID_roll -> kp =0.20f;
	PID_roll -> kd =0.07f;
	PID_roll -> ki =0.0;
	PID_roll -> setpoint =0.0;

	PID_pitch -> kp =0.20f;
	PID_pitch -> kd =0.07f;
	PID_pitch -> ki =0.0;
	PID_pitch -> setpoint =0.0;

	PID_yaw_rate -> kp =0.8f;
	PID_yaw_rate -> kd =0.0f;
	PID_yaw_rate -> ki =0.0;
	PID_yaw_rate -> setpoint =0.0;
	PID_yaw_rate -> out_max = 30.0f;
	PID_yaw_rate -> out_min = -30.0f;

	PID_heading -> kp = 3.0f;
	PID_heading -> kd = 0.0f;
	PID_heading -> ki = 0.0;
	PID_heading -> out_max = 50.0f;
	PID_heading -> out_min = -50.0f;
	PID_heading -> setpoint = 0.0;

	PID_Zd -> kp =0.30f;
	PID_Zd -> kd =0.0;
	PID_Zd -> ki =0.0;
	PID_Zd -> out_max = +20.0f;
	PID_Zd -> out_min = -20.0f;
	PID_Zd -> setpoint =0.0;

	PID_Z -> kp =1.8f;
	PID_Z -> kd =0.0;
	PID_Z -> ki =0.0;
	PID_Z -> out_max = +50.0f;
	PID_Z -> out_min = -50.0f;
	PID_Z -> setpoint =0.0;


	PID_nav -> kp =0.5f;//0.045f;
	PID_nav -> kd =0.0f;//0.06;
	PID_nav -> ki =0.0;
	PID_nav -> out_max = +50.0f;
	PID_nav -> out_min = -50.0f;
}

void PID_output(radio_controller_t* rc_command,attitude_stablizer_pid_t* PID_roll,attitude_stablizer_pid_t* PID_pitch,attitude_stablizer_pid_t* PID_yaw_rate,vertical_pid_t* PID_Zd){

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

	motor . m1 = -10.0f + (rc_command->throttle_control_input) - (PID_roll->output) + (PID_pitch -> output) - (PID_yaw_rate -> output) + (PID_Zd -> output);
	motor . m2 = -10.0f + (rc_command->throttle_control_input) + (PID_roll->output) + (PID_pitch -> output) + (PID_yaw_rate -> output) + (PID_Zd -> output);
	motor . m3 = -10.0f + (rc_command->throttle_control_input) + (PID_roll->output) - (PID_pitch -> output) - (PID_yaw_rate -> output) + (PID_Zd -> output);
	motor . m4 = -10.0f + (rc_command->throttle_control_input) - (PID_roll->output) - (PID_pitch -> output) + (PID_yaw_rate -> output) + (PID_Zd -> output);
	set_pwm_motor(&motor);

	LED_ON(LED3);

	}else{

	motor. m1 =0.0;
	motor. m2 =0.0;
	motor. m3 =0.0;
	motor. m4 =0.0;
	set_pwm_motor(&motor);

	LED_OFF(LED3);
	}
}