#include "controller.h"

void PID_rc_pass_command(attitude_t* attitude,attitude_stablizer_pid_t* PID_roll,attitude_stablizer_pid_t* PID_pitch,attitude_stablizer_pid_t* PID_heading,vertical_pid_t* PID_Z,vertical_pid_t* PID_Zd,nav_pid_t* PID_nav,radio_controller_t* rc_command){

	/* Attitude Controller command */
	PID_roll -> setpoint = (rc_command -> roll_control_input) + (PID_nav -> output_roll);
	PID_pitch -> setpoint = (rc_command -> pitch_control_input) + (PID_nav -> output_pitch);

		if( rc_command -> safety == ENGINE_ON) {
			/* If throttle is smaller than 10%, stop integrator */

			if( rc_command -> throttle_control_input < 10.0f){
				PID_roll -> integral = 0.0f;
				PID_pitch -> integral = 0.0f;
			}
		}else{
			
			/* Always stop integrator when safety is on */
			PID_roll -> integral = 0.0f;
			PID_pitch -> integral = 0.0f;

		}





	/* Heading Controller command */
	if(CONTROLLER_YAW_MODE  == YAW_MODE_MAGNETO){

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

	}else if(CONTROLLER_YAW_MODE == YAW_MODE_GYRO){
		
			PID_heading -> setpoint = rc_command -> yaw_rate_control_input;

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

	/* at this point all gain data are being updated by  PID_control_parameter_update() function */

	PID_roll -> kp =0.20f;
	PID_roll -> kd =0.07f;
	PID_roll -> ki =0.0;
	PID_roll -> setpoint =0.0;

	PID_pitch -> kp =0.20f;
	PID_pitch -> kd =0.07f;
	PID_pitch -> ki =0.0;
	PID_pitch -> setpoint =0.0;

	PID_yaw_rate -> kp =0.65f;
	PID_yaw_rate -> kd =0.0f;
	PID_yaw_rate -> ki =0.0;
	PID_yaw_rate -> setpoint =0.0;
	PID_yaw_rate -> out_max = 30.0f;
	PID_yaw_rate -> out_min = -30.0f;

	PID_heading -> kp = 2.5f;
	PID_heading -> kd = 0.0f;
	PID_heading -> ki = 0.0;
	PID_heading -> out_max = 50.0f;
	PID_heading -> out_min = -50.0f;
	PID_heading -> setpoint = 0.0;

	PID_Zd -> kp =0.3f;
	PID_Zd -> kd =0.0;
	PID_Zd -> ki =0.03;
	PID_Zd -> out_max = +20.0f;
	PID_Zd -> out_min = -20.0f;
	PID_Zd -> setpoint =0.0;

	PID_Z -> kp =1.4f;//1.8f;
	PID_Z -> kd =0.0;
	PID_Z -> ki =0.0;
	PID_Z -> out_max = +50.0f;
	PID_Z -> out_min = -50.0f;
	PID_Z -> setpoint =0.0;


	PID_nav -> kp =0.045f;//0.045f;
	PID_nav -> kd =0.06f;//0.06;
	PID_nav -> ki =0.0001f;
	PID_nav -> out_max = +22.0f;
	PID_nav -> out_min = -22.0f;

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


void PID_control_parameter_update(attitude_stablizer_pid_t* PID_roll,attitude_stablizer_pid_t* PID_pitch,attitude_stablizer_pid_t* PID_yaw_rate,attitude_stablizer_pid_t* PID_heading,vertical_pid_t* PID_Z,vertical_pid_t* PID_Zd,nav_pid_t* PID_nav){



	/* Roll axis PID parameter */
	if(check_global_data_update_flag(ROLL_KP) == true){
		read_global_data_value(ROLL_KP, DATA_POINTER_CAST(&(PID_roll -> kp)));
	
		reset_global_data_update_flag(ROLL_KP);

	}

	if(check_global_data_update_flag(ROLL_KI) == true){
		read_global_data_value(ROLL_KI, DATA_POINTER_CAST(&(PID_roll -> ki)));
	
		reset_global_data_update_flag(ROLL_KI);

	}

	if(check_global_data_update_flag(ROLL_KD) == true){
		read_global_data_value(ROLL_KD, DATA_POINTER_CAST(&(PID_roll -> kd)));
	
		reset_global_data_update_flag(ROLL_KD);

	}


	/* Pitch axis PID parameter */
	if(check_global_data_update_flag(PITCH_KP) == true){
		read_global_data_value(PITCH_KP, DATA_POINTER_CAST(&(PID_pitch -> kp)));
	
		reset_global_data_update_flag(PITCH_KP);

	}

	if(check_global_data_update_flag(PITCH_KI) == true){
		read_global_data_value(PITCH_KI, DATA_POINTER_CAST(&(PID_pitch -> ki)));
	
		reset_global_data_update_flag(PITCH_KI);

	}

	if(check_global_data_update_flag(PITCH_KD) == true){
		read_global_data_value(PITCH_KD, DATA_POINTER_CAST(&(PID_pitch -> kd)));
	
		reset_global_data_update_flag(PITCH_KD);

	}

	/* Yaw rate PID parameter */
	if(check_global_data_update_flag(YAW_KP) == true){
		read_global_data_value(YAW_KP, DATA_POINTER_CAST(&(PID_yaw_rate -> kp)));
	
		reset_global_data_update_flag(YAW_KP);

	}

	if(check_global_data_update_flag(YAW_KI) == true){
		read_global_data_value(YAW_KI, DATA_POINTER_CAST(&(PID_yaw_rate -> ki)));
	
		reset_global_data_update_flag(YAW_KI);

	}

	if(check_global_data_update_flag(YAW_KD) == true){
		read_global_data_value(YAW_KD, DATA_POINTER_CAST(&(PID_yaw_rate -> kd)));
	
		reset_global_data_update_flag(YAW_KD);

	}

	/* Heading PID parameter */
	if(check_global_data_update_flag(HEADING_KP) == true){
		read_global_data_value(HEADING_KP, DATA_POINTER_CAST(&(PID_heading -> kp)));
	
		reset_global_data_update_flag(HEADING_KP);

	}

	if(check_global_data_update_flag(HEADING_KI) == true){
		read_global_data_value(HEADING_KI, DATA_POINTER_CAST(&(PID_heading -> ki)));
	
		reset_global_data_update_flag(HEADING_KI);

	}

	if(check_global_data_update_flag(HEADING_KD) == true){
		read_global_data_value(HEADING_KD, DATA_POINTER_CAST(&(PID_heading -> kd)));
	
		reset_global_data_update_flag(HEADING_KD);

	}

	/* Z PID parameter */
	if(check_global_data_update_flag(Z_KP) == true){
		read_global_data_value(Z_KP, DATA_POINTER_CAST(&(PID_Z -> kp)));
	
		reset_global_data_update_flag(Z_KP);

	}

	if(check_global_data_update_flag(Z_KI) == true){
		read_global_data_value(Z_KI, DATA_POINTER_CAST(&(PID_Z -> ki)));
	
		reset_global_data_update_flag(Z_KI);

	}

	if(check_global_data_update_flag(Z_KD) == true){
		read_global_data_value(Z_KD, DATA_POINTER_CAST(&(PID_Z -> kd)));
	
		reset_global_data_update_flag(Z_KD);

	}


	/* Zd PID parameter */
	if(check_global_data_update_flag(ZD_KP) == true){
		read_global_data_value(ZD_KP, DATA_POINTER_CAST(&(PID_Zd -> kp)));
	
		reset_global_data_update_flag(ZD_KP);

	}

	if(check_global_data_update_flag(ZD_KI) == true){
		read_global_data_value(ZD_KI, DATA_POINTER_CAST(&(PID_Zd -> ki)));
	
		reset_global_data_update_flag(ZD_KI);

	}

	if(check_global_data_update_flag(ZD_KD) == true){
		read_global_data_value(ZD_KD, DATA_POINTER_CAST(&(PID_Zd -> kd)));
	
		reset_global_data_update_flag(ZD_KD);

	}

	/* Navigation PID parameter */
	if(check_global_data_update_flag(NAV_KP) == true){
		read_global_data_value(NAV_KP, DATA_POINTER_CAST(&(PID_nav -> kp)));
	
		reset_global_data_update_flag(NAV_KP);

	}

	if(check_global_data_update_flag(NAV_KI) == true){
		read_global_data_value(NAV_KI, DATA_POINTER_CAST(&(PID_nav -> ki)));
	
		reset_global_data_update_flag(NAV_KI);

	}

	if(check_global_data_update_flag(NAV_KD) == true){
		read_global_data_value(NAV_KD, DATA_POINTER_CAST(&(PID_nav -> kd)));
	
		reset_global_data_update_flag(NAV_KD);

	}



}

//__attribute__((unused))