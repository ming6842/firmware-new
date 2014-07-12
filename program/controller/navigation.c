//navigation.c
#include "navigation.h"

// NED -> XYZ so, N~x, E~y
// lat=N/S -> x, lon=E/W -> y


void PID_Nav(nav_pid_t *PID_control,attitude_t *attitude,UBXvelned_t *UBXvelned, UBXposLLH_t *UBXposLLH){

	float S_heading= arm_sin_f32(attitude->yaw * (0.01745329252392f));
	float C_heading= arm_cos_f32(attitude->yaw * (0.01745329252392f));


	if( PID_control -> controller_status == CONTROLLER_ENABLE){

		(PID_control -> error.x) = (float)((PID_control -> setpoint.x) -(UBXposLLH->lat));
		(PID_control -> error.y) = (float)((PID_control -> setpoint.y) -(UBXposLLH->lon));

		float P_lat = (PID_control -> error.x)*(PID_control -> kp);
		float P_lon = (PID_control -> error.y)*(PID_control -> kp);

		float D_N = -(float)(UBXvelned -> velN) * (PID_control -> kd);
		float D_E = -(float)(UBXvelned -> velE) * (PID_control -> kd);


		float P_roll_control = -P_lat*S_heading + P_lon*C_heading;
		float P_pitch_control = 0.0f -P_lat*C_heading - P_lon*S_heading;

		float D_roll_control = -D_N*S_heading + D_E*C_heading;
		float D_pitch_control = 0.0f -D_N*C_heading - D_E*S_heading;


		D_roll_control   = bound_float(D_roll_control,-40.0f,40.0f);
		D_pitch_control  = bound_float(D_pitch_control ,-40.0f,40.0f);

		P_roll_control   = bound_float(P_roll_control,-40.0f,40.0f);
		P_pitch_control  = bound_float(P_pitch_control,-40.0f,40.0f);

		(PID_control -> output_roll) = P_roll_control+D_roll_control;
		(PID_control -> output_pitch) = P_pitch_control+D_pitch_control;

		(PID_control -> output_roll) = bound_float(PID_control -> output_roll,PID_control -> out_min,PID_control -> out_max);
		(PID_control -> output_pitch) = bound_float(PID_control -> output_pitch,PID_control -> out_min,PID_control -> out_max);
	}else{

		PID_control -> setpoint.x = UBXposLLH->lat;
		PID_control -> setpoint.y = UBXposLLH->lon;

		PID_control -> integral.x = 0.0f;
		PID_control -> integral.y = 0.0f;
		PID_control -> output_roll =0.0f;
		PID_control -> output_pitch =0.0f;
	}

} 

// typedef struct navigation_info_t
// {
// 	waypoint_navigation_t wp_info[WAYPOINT_MAX_SIZE];
// 	lla_pos_t home_wp_info;
// 	lla_pos_t instant_wp;
// 	lla_pos_t current_pos;
// 	uint8_t navigation_mode;
// 	uint8_t busy_flag;
// }navigation_info_t;


// typedef struct waypoint_navigation_t
// {
// 	lla_pos_t position;
// 	uint8_t autocontinue;
// 	float tol_radius;
// 	uint8_t data_available;
// }waypoint_navigation_t;


navigation_info_t navigation_info = {

	.wp_info[0 ... (WAYPOINT_MAX_SIZE-1)] = {

		.position = {

			.lat = 0,
			.lon = 0,
			.alt = 0.0f

		},

	.autocontinue = 0,
	.tol_radius = 5.0f,
	.data_available = 0

	},

	.home_wp ={

		.lat = 0,
		.lon = 0,
		.alt = 0.0f

	},

	.instant_wp ={
		.lat = 0,
		.lon = 0,
		.alt = 0.0f
	},

	.current_pos ={
		.lat = 0,
		.lon = 0,
		.alt = 0.0f
	},

	.current_wp_id = 0,
	.navigation_mode = NAVIGATION_MODE_GO_HOME,
	.busy_flag = ACCESS_CLEAR,
	.max_dist_from_home = 100.0f


};

float get_elasped_time(uint32_t start_time_i32_s,float start_time_remainder){

	uint32_t current_sec = get_system_time_sec();
	float current_remainder = get_system_time_sec_remainder();
	float time_elasped = (float)(current_sec - start_time_i32_s)+(current_remainder - start_time_remainder);

	return time_elasped;
}

#define NAVIGATION_TASK_PERIOD_MS 200

void navigation_task(void){

	uint8_t buffer[100];


 	/* Generate  vTaskDelayUntil parameters */
	portTickType xLastWakeTime;
	const portTickType xFrequency = (uint32_t)NAVIGATION_TASK_PERIOD_MS/(1000.0 / configTICK_RATE_HZ);

    // Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount();

	while(1){




			if (DMA_GetFlagStatus(DMA1_Stream6, DMA_FLAG_TCIF6) != RESET) {

				buffer[7] = 0;buffer[8] = 0;buffer[9] = 0;buffer[10] = 0;buffer[11] = 0;buffer[12] = 0;	buffer[13] = 0;


				sprintf((char *)buffer, "%ld,\r\n",

		 			(uint32_t)(000.0f));

				usart2_dma_send(buffer);
			}	


		vTaskDelayUntil( &xLastWakeTime, xFrequency );
	}

}