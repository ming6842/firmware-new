//navigation.c
#include "navigation.h"
#include "mission.h"
// NED -> XYZ so, N~x, E~y
// lat=N/S -> x, lon=E/W -> y
extern int waypoint_cnt;
extern waypoint_t *mission_wp_list;
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

		//PID_control -> setpoint.x = UBXposLLH->lat;
		//PID_control -> setpoint.y = UBXposLLH->lon;

		/* cancelled manual holding -> forwarded to navigation task */

		PID_control -> integral.x = 0.0f;
		PID_control -> integral.y = 0.0f;
		PID_control -> output_roll =0.0f;
		PID_control -> output_pitch =0.0f;
	}

} 

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

	.halt_wp ={
		.lat = 0,
		.lon = 0,
		.alt = 0.0f
	},

	.current_pos ={
		.lat = 0,
		.lon = 0,
		.alt = 0.0f
	},

	.target_pos ={
		.lat = 0,
		.lon = 0,
		.alt = 0.0f
	},

	.current_wp_id = 0,
	.navigation_mode = NAVIGATION_MODE_GO_HOME,
	.busy_flag = ACCESS_CLEAR,
	.halt_flag = 0,
	.max_dist_from_home = 100.0f,
	.waypoint_status = NOT_HAVE_BEEN_UPDATED 

};

float get_elasped_time(uint32_t start_time_i32_s,float start_time_remainder){

	uint32_t current_sec = get_system_time_sec();
	float current_remainder = get_system_time_sec_remainder();
	float time_elasped = (float)(current_sec - start_time_i32_s)+(current_remainder - start_time_remainder);

	return time_elasped;
}

void update_current_state(void){

	/* GPS localizer initialization */
	//UBXvelned_t NAV_GPS_velocity_NED;
	//UBXsol_t NAV_GPS_solution_info;
	UBXposLLH_t NAV_GPS_position_LLH;

	vertical_data_t NAV_altitude_data;

	//NAV_GPS_velocity_NED = get_UBXvelned_data();
	//NAV_GPS_solution_info = get_UBXsol_data();
	NAV_GPS_position_LLH = get_UBXposLLH_data();
	NAV_altitude_data = get_vertical_data();

	navigation_info.current_pos.lat = NAV_GPS_position_LLH.lat;
	navigation_info.current_pos.lon = NAV_GPS_position_LLH.lon;
	navigation_info.current_pos.lon = NAV_altitude_data.Z*0.01f; //Convert to meter unit

}

#define NAVIGATION_TASK_PERIOD_MS 200

void navigation_task(void){

	uint8_t buffer[100];


	uint32_t start_sec = get_system_time_sec();
	float start_remainder = get_system_time_sec_remainder();
	float mission_time=0.0f;
	uint32_t current_sec = get_system_time_sec();
	float current_remainder = get_system_time_sec_remainder();

 	/* Generate  vTaskDelayUntil parameters */
	portTickType xLastWakeTime;
	const portTickType xFrequency = (uint32_t)NAVIGATION_TASK_PERIOD_MS/(1000.0 / configTICK_RATE_HZ);

    // Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount();

	while(1){

		current_sec = get_system_time_sec();
		current_remainder = get_system_time_sec_remainder();

		mission_time= get_elasped_time(start_sec,start_remainder);
		update_current_state();

		/* Keep monitoring position when not halt */
		if(navigation_info.halt_flag == 0){

			navigation_info.halt_wp = navigation_info.current_pos;

		}
		
		/* command interpreter and decision (required connection with MAVLink) */
		navigation_info.navigation_mode = NAVIGATION_MODE_HALT; // Dummy command
		/* copy mavlink waypoints to navigation info struct*/
		/* check the waypoints have been updated */
		if (navigation_info.waypoint_status == NOT_HAVE_BEEN_UPDATED) {
			/*Resources is availabe*/
			if (Is_MAVLink_WP_Busy == UNBUSY)
			{
				/*lock the resources*/
				Is_MAVLink_WP_Busy = BUSY;
				/*copying*/
				int i;
				waypoint_t* wp_ptr;
				for ( i=0; i < waypoint_cnt; i++){

					wp_ptr = get_waypoint(mission_wp_list, i);
					navigation_info.wp_info[i].position.lat = (int32_t)(wp_ptr->data.x * 1E7f);
					navigation_info.wp_info[i].position.lon = (int32_t)(wp_ptr->data.y * 1E7f);
					navigation_info.wp_info[i].position.alt = wp_ptr->data.z;
					navigation_info.wp_info[i].tol_radius = wp_ptr->data.param2;
					navigation_info.wp_info[i].autocontinue = wp_ptr->data.autocontinue;
					navigation_info.wp_info[i].data_available = 1;
					
					
				}
				navigation_info.waypoint_status = HAVE_BEEN_UPDATED;
				/*unlock the resources*/
				Is_MAVLink_WP_Busy = UNBUSY;
			}
		}

		if(navigation_info.navigation_mode != NAVIGATION_MODE_HALT){ 

			/* if not in HALT mode */
			navigation_info.halt_flag = 0;

		}else{

			/* HALTed */
			navigation_info.halt_flag = 1;

		}

			/* Proceed navigation with respect to received command */
			switch(navigation_info.navigation_mode) {

				/* hold at current position */
			    case NAVIGATION_MODE_HALT:
			    	navigation_info.target_pos = navigation_info.halt_wp;
			    break;

				/* Go back to home position */
			    case NAVIGATION_MODE_GO_HOME:

			    break;

				/* Go to specific coordinate */
			    case NAVIGATION_MODE_GO_SPECIFIED_POS:

			    break;

				/* Follow the waypoint list */
			    case NAVIGATION_MODE_WAYPOINT:

			    break;
			 }


			if (DMA_GetFlagStatus(DMA1_Stream6, DMA_FLAG_TCIF6) != RESET) {

				buffer[7] = 0;buffer[8] = 0;buffer[9] = 0;buffer[10] = 0;buffer[11] = 0;buffer[12] = 0;	buffer[13] = 0;


				sprintf((char *)buffer, "%lu,%lu,%lu,\r\n",

		 			(uint32_t)(current_sec),
		 			(uint32_t)(current_remainder*1000.0f),
		 			(uint32_t)(mission_time*1000000.0f));

				usart2_dma_send(buffer);
			}	


		vTaskDelayUntil( &xLastWakeTime, xFrequency );
	}

}


void pass_navigation_setpoint(nav_pid_t *PID_nav_info,vertical_pid_t *PID_Z){

	PID_nav_info -> setpoint.x = navigation_info.target_pos.lat;
	PID_nav_info -> setpoint.y = navigation_info.target_pos.lon;

	PID_Z -> setpoint = navigation_info.target_pos.alt*100.0f;

}