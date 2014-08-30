//navigation.c
#include "navigation.h"
#include "mission.h"
// NED -> XYZ so, N~x, E~y
// lat=N/S -> x, lon=E/W -> y

extern waypoint_info_t waypoint_info;

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
	.loiter_time = 5.0f,
	.waypoint_state=WAYPOINT_STATUS_PENDING,
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

	.hold_wp ={
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
	.max_dist_from_home = 100.0f


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

		update_current_state();

		/* test area */

		/* --------------- */

		/* Keep monitoring position when not halt */
		if(navigation_info.halt_flag == 0){

			navigation_info.hold_wp = navigation_info.current_pos;

		}

		/* command interpreter and decision (required connection with MAVLink) */
		navigation_info.navigation_mode = NAVIGATION_MODE_HOLD_POINT; // Dummy command
		/* copy mavlink waypoints to navigation info struct*/
		/* check the waypoints have been updated */
		if (navigation_info.waypoint_status == NOT_HAVE_BEEN_UPDATED) {
			/*Resources is availabe*/
			if (waypoint_info.is_busy == false)
			{
				/*lock the resources*/
				waypoint_info.is_busy = true;
				/*copying*/
				int i;
				waypoint_t* wp_ptr;
				for ( i=0; i < waypoint_info.waypoint_count; i++){

					wp_ptr = get_waypoint(waypoint_info.waypoint_list, i);
					navigation_info.wp_info[i].position.lat = (int32_t)(wp_ptr->data.x * 1E7f);
					navigation_info.wp_info[i].position.lon = (int32_t)(wp_ptr->data.y * 1E7f);
					navigation_info.wp_info[i].position.alt = wp_ptr->data.z;
					navigation_info.wp_info[i].tol_radius = wp_ptr->data.param2;
					navigation_info.wp_info[i].autocontinue = wp_ptr->data.autocontinue;
					navigation_info.wp_info[i].data_available = 1;
					
					
				}
				navigation_info.waypoint_status = HAVE_BEEN_UPDATED;
				/*unlock the resources*/
				waypoint_info.is_busy = false;
			}
		}

		if(navigation_info.navigation_mode != NAVIGATION_MODE_HOLD_POINT){ 

			/* if not in HALT mode */
			navigation_info.halt_flag = 0;

		}else{

			/* HALTed */
			navigation_info.halt_flag = 1;

		}

			/* Proceed navigation with respect to received command */
			switch(navigation_info.navigation_mode) {

				/* hold at current position */
			    case NAVIGATION_MODE_HOLD_POINT:
			    	navigation_info.target_pos = navigation_info.hold_wp;
			    break;

				/* Go back to home position */
			    case NAVIGATION_MODE_GO_HOME:
			    	navigation_info.target_pos = navigation_info.home_wp;
			    break;

				/* Go to specific coordinate */
			    case NAVIGATION_MODE_GO_SPECIFIED_POS:
			    	navigation_info.target_pos = navigation_info.instant_wp;

			    break;

				/* Follow the waypoint list */
			    case NAVIGATION_MODE_WAYPOINT:

			    /* Measure the current state of current waypoint */

			    switch (navigation_info.wp_info[navigation_info.current_wp_id].waypoint_state){

			    	/* first time to come to this waypoint */
			    	case WAYPOINT_STATUS_PENDING:

			    			/* check if this waypoint is valid */
			    		if(navigation_info.wp_info[navigation_info.current_wp_id].data_available==1){
					    	navigation_info.wp_info[navigation_info.current_wp_id].waypoint_state = WAYPOINT_STATUS_ACTIVE;
							
					    	/* Guide aircraft to target */
							navigation_info.target_pos = navigation_info.wp_info[navigation_info.current_wp_id].position;
						}else{

							/* something is wrong, go to halt */
							navigation_info.navigation_mode = NAVIGATION_MODE_HOLD_POINT;
							
						}

			    	break;

			    	/* this waypoint is in process */
			    	case WAYPOINT_STATUS_ACTIVE:

				    	/* estimate distance_to_target */
				    	navigation_info.current_distance_to_target = calc_distance_two_wp(navigation_info.current_pos.lat,navigation_info.current_pos.lon, navigation_info.wp_info[navigation_info.current_wp_id].position.lat,  navigation_info.wp_info[navigation_info.current_wp_id].position.lon);

				    	if(navigation_info.current_distance_to_target < navigation_info.wp_info[navigation_info.current_wp_id].tol_radius){

				    		/* if aircraft is in tolerance circle, go to next state */
				    		navigation_info.wp_info[navigation_info.current_wp_id].waypoint_state = WAYPOINT_STATUS_LOITERING;
				    	
				    		/* start loiter time counter */
				    		start_sec = get_system_time_sec();
				    		start_remainder = get_system_time_sec_remainder();
				    	}

				    	/* Guide aircraft to target */
						navigation_info.target_pos = navigation_info.wp_info[navigation_info.current_wp_id].position;

			    	break;


			    	case WAYPOINT_STATUS_LOITERING:

			    		/* get loitering time */	
						mission_time= get_elasped_time(start_sec,start_remainder);

						if(mission_time > navigation_info.wp_info[navigation_info.current_wp_id].loiter_time){

							/* loitered more than specified time -> go to next stage*/
							navigation_info.wp_info[navigation_info.current_wp_id].waypoint_state = WAYPOINT_STATUS_DONE;
				    	

						}

						/* Maintain aircraft position at the loitering point*/
						navigation_info.target_pos = navigation_info.wp_info[navigation_info.current_wp_id].position;


			    	break;

			    	case WAYPOINT_STATUS_DONE:

			    		/* waypoint has run to the end, need to check autocontinue flag */
			    		if(navigation_info.wp_info[navigation_info.current_wp_id].autocontinue==1){

			    			/* Auto-continue to next waypoint */

			    			/* check if next waypoint is available or not */
			    			if(navigation_info.wp_info[navigation_info.current_wp_id+1].data_available==1){

			    				/* go to next one */
			    				navigation_info.current_wp_id++;

			    			}else{

			    				/* stay here at last waypoint*/
			    				navigation_info.target_pos = navigation_info.wp_info[navigation_info.current_wp_id].position;

			    			}


			    		}


			    	break;



			    }


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

#define TO_RAD 0.017453292519943f
#define R_EARTH 6378140.0f //R of earth in meter

#include "arm_math.h"
#include <math.h>

float calc_distance_two_wp(int32_t lat1,int32_t lon1, int32_t lat2, int32_t lon2){

float dLat = (float)(lat2-lat1) * 0.0000001f * TO_RAD;
float dLon = (float)(lon2-lon1) * 0.0000001f * TO_RAD;

float Lat1_f = (float)lat1 * 0.0000001f * TO_RAD;

float Lat2_f = (float)lat2 * 0.0000001f * TO_RAD;

float sin_dlat_div_2 = arm_sin_f32(dLat*0.5f);
float sin_dlon_div_2 = arm_sin_f32(dLon*0.5f);

float a = sin_dlat_div_2*sin_dlat_div_2 + arm_cos_f32(Lat1_f) * arm_cos_f32(Lat2_f) *sin_dlon_div_2 * sin_dlon_div_2;
float c = 2.0f * atan2f(sqrtf(a), sqrtf(1.0f-a));

return R_EARTH*c;
}