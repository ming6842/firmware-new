#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "_math.h"
#include "delay.h"

#include "usart.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "mavlink.h"

#include "global.h"

#include "communication.h"
#include "command_parser.h"
#include "parameter.h"
#include "mission.h"
#include "FreeRTOS.h"
#include "system_time.h"
#include "io.h"

extern int16_t __nav_roll,__nav_pitch;
extern uint32_t __pAcc,__numSV;
extern int32_t __altitude_Zd;

xSemaphoreHandle mavlink_msg_send_sem;

uint8_t receiver_task_buffer[MAVLINK_MAX_PAYLOAD_LEN];
uint8_t broadcast_task_buffer[MAVLINK_MAX_PAYLOAD_LEN];
uint32_t receiver_sleep_time;

void receiver_task_send_package(mavlink_message_t *msg)
{
	uint16_t len = mavlink_msg_to_send_buffer(receiver_task_buffer, msg);
	
	mavlink_receiver_serial_write(receiver_task_buffer, len);
}

static void broadcast_task_send_package(mavlink_message_t *msg)
{
	uint16_t len = mavlink_msg_to_send_buffer(broadcast_task_buffer, msg);

	status_mavlink_serial_write(broadcast_task_buffer, len);
}

static void send_heartbeat_info(void)
{
	mavlink_message_t msg;
	uint8_t current_flight_mode,current_safety_switch;
	uint8_t current_MAV_mode = MAV_MODE_PREFLIGHT;

	read_global_data_value(MODE_BUTTON, DATA_POINTER_CAST(&current_flight_mode));
	read_global_data_value(SAFTY_BUTTON, DATA_POINTER_CAST(&current_safety_switch));

	if(current_safety_switch == 0){
		/* ENGINE ON */

		if(current_flight_mode == 0){
			/* Mode 1 */
			current_MAV_mode = MAV_MODE_STABILIZE_ARMED;

		}else if(current_flight_mode == 1){
			/* Mode 2 */
			current_MAV_mode = MAV_MODE_GUIDED_ARMED;

		}else if(current_flight_mode == 2){
			/* Mode 3 */
			current_MAV_mode = MAV_MODE_AUTO_ARMED;

		}


	}else if(current_safety_switch == 1){
		/* ENGINE OFF */

		if(current_flight_mode == 0){
			/* Mode 1 */
			current_MAV_mode = MAV_MODE_STABILIZE_DISARMED;

		}else if(current_flight_mode == 1){
			/* Mode 2 */
			current_MAV_mode = MAV_MODE_GUIDED_DISARMED;

		}else if(current_flight_mode == 2){
			/* Mode 3 */
			current_MAV_mode = MAV_MODE_AUTO_DISARMED;

		}
	}

	mavlink_msg_heartbeat_pack(1, 200, &msg,
		MAV_TYPE_QUADROTOR, 
		MAV_AUTOPILOT_GENERIC, 
		current_MAV_mode, 
		0, MAV_STATE_ACTIVE
	);

	broadcast_task_send_package(&msg);
}

static void send_gps_info(void)
{
	int32_t latitude, longitude, altitude;
	int16_t gps_vx, gps_vy, gps_vz;
	float true_yaw;

	/* Prepare the GPS data */
	read_global_data_value(GPS_LAT, DATA_POINTER_CAST(&latitude));
	read_global_data_value(GPS_LON, DATA_POINTER_CAST(&longitude));
	read_global_data_value(GPS_ALT, DATA_POINTER_CAST(&altitude));
	read_global_data_value(GPS_VX, DATA_POINTER_CAST(&gps_vx));
	read_global_data_value(GPS_VY, DATA_POINTER_CAST(&gps_vy));
	read_global_data_value(GPS_VZ, DATA_POINTER_CAST(&gps_vz));
	read_global_data_value(TRUE_YAW, DATA_POINTER_CAST(&true_yaw));

	mavlink_message_t msg;

	mavlink_msg_global_position_int_pack(1, 220, &msg, 
		get_system_time_ms(),   		       //time 
		latitude ,  //Latitude
		longitude ,  //Longitude
		altitude, //Altitude
		0,
		gps_vx * 1,   //Speed-Vx
		gps_vy * 1,   //Speed-Vy
		gps_vz * 1,   //Speed-Vz
		(uint16_t)true_yaw
	);

	broadcast_task_send_package(&msg);
}

static void send_attitude_info(void)
{
	mavlink_message_t msg;
	float attitude_roll, attitude_pitch, attitude_yaw;

	/* Prepare the attitude data */
	read_global_data_value(TRUE_ROLL, DATA_POINTER_CAST(&attitude_roll));
	read_global_data_value(TRUE_PITCH, DATA_POINTER_CAST(&attitude_pitch));
	read_global_data_value(TRUE_YAW, DATA_POINTER_CAST(&attitude_yaw));

	mavlink_msg_attitude_pack(1, 200, &msg,
		get_system_time_ms(),
		toRad(attitude_roll), 
		toRad(attitude_pitch), 
		toRad(attitude_yaw), 
		0.0, 0.0, 0.0
	);

	broadcast_task_send_package(&msg);
}

#if 0
static void send_system_info(void)
{
	mavlink_message_t msg;

	mavlink_msg_sys_status_pack(1, 0, &msg,
		0,
		0,
		0,
		0,
		12.5 * 1000, //Battery voltage
		-1,
		100,         //Battery remaining
		0,
		0,
		0,
		0,
		0,
		0
	);

	send_package(&msg);
}
#endif

static void send_reached_waypoint(void)
{
	if(mission_info.reached_waypoint.is_update == true) {
		mavlink_message_t msg;		

		/* Notice the ground station that the vehicle is reached at the 
	   	waypoint */
		mavlink_msg_mission_item_reached_pack(1, 0, &msg,
			mission_info.reached_waypoint.number);
		broadcast_task_send_package(&msg);

		mission_info.reached_waypoint.is_update = false;
	}
}

static void send_current_waypoint(void)
{
	if(mission_info.current_waypoint.is_update == true) {
		mavlink_message_t msg;		

		/* Update the new current waypoint */
		mavlink_msg_mission_current_pack(1, 0, &msg,
			mission_info.current_waypoint.number);
		broadcast_task_send_package(&msg);

		mission_info.current_waypoint.is_update = false;
	}
}

void send_status_text_message(char *text)
{
	mavlink_message_t msg;

	mavlink_msg_statustext_pack(1, 0, &msg, 0, text);
	broadcast_task_send_package(&msg);
}

void set_mavlink_receiver_delay_time(uint32_t time)
{
	receiver_sleep_time = time;
}

#define TIMER_1HZ  0
#define TIMER_20HZ 1

static void handle_message(mavlink_message_t *mavlink_message)
{
	if(generic_handle_message(mavlink_message) == true) {
		return;
	}
	
	if(mission_handle_message(mavlink_message) == true) {
		return;
	}

	/* If still return a false value, this is a parser undefined mavlink message */
	if(parameter_handle_message(mavlink_message) == false) {
		//TODO:Print unknown message error
	}
}

static void mavlink_mission_timeout_check(void)
{
	if(get_mavlink_mission_state() != MISSION_STATE_IDLE) 
	{
		handle_mission_write_timeout();
		handle_mission_read_timeout();
	}
}

void ground_station_task(void)
{
	int buffer;
	receiver_sleep_time = portMAX_DELAY; //Sleep until someone wake the task up
	
	mavlink_message_t mavlink_message;
	mavlink_status_t message_status;

	while(1) {
		//Try to receive a byte, and if there is no data, the task won't be wake up
		buffer = usart3_read(receiver_sleep_time);

		//Parse and handle the mavlink message if the data is available
		if(buffer != USART_NOT_AVAILABLE) {
			if(mavlink_parse_char(MAVLINK_COMM_0, buffer, &mavlink_message, &message_status)) {
				handle_message(&mavlink_message);
			}
		}

		mavlink_mission_timeout_check();
	}
}

void mavlink_send_task()
{
	uint32_t start_time[2] = {get_system_time_ms()};
	uint32_t current_time;

	while(1) {
		/* Send heartbeat message and gps message in 1hz */
		current_time = get_system_time_ms();
		if((current_time - start_time[TIMER_1HZ]) >= 1000) {
			send_heartbeat_info(); //Heartbeat message should be sent at anytime
			send_gps_info();

			start_time[TIMER_1HZ] = current_time;
		}

		/* Send attitude message and waypoint message in 20hz */
		if((current_time - start_time[TIMER_20HZ]) >= 50) {
			send_attitude_info();
			send_reached_waypoint();
			send_current_waypoint();

			start_time[TIMER_20HZ] = current_time;
		}

#if 0
		if(cnt == 5) {
			sprintf((char *)msg_buff, "Zd:%ld NAV: %d,%d,%ld,%ld",
				__altitude_Zd,
				__nav_roll,
				__nav_pitch,
				__pAcc,
				__numSV);

			mavlink_msg_statustext_pack(1,
					0,
					&msg,
					0,
					(const char *) &msg_buff);
			//send_package(&msg);
			
		}
#endif
	}
}
