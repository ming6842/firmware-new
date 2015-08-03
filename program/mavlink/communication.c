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
#include "mission.h"
#include "FreeRTOS.h"
#include "system_time.h"
#include "io.h"

#define TRANSACTION_TIMEOUT 500
#define TIMEOUT_COUNT_MAX 5

xSemaphoreHandle mavlink_msg_send_sem;

mavlink_message_t received_msg;
mavlink_status_t received_status;

bool exist_pending_transaction;
int transaction_type = -1;
int transaction_timeout_count = 0;
uint32_t receiver_sleep_time;
uint32_t tranaction_start_time;

extern int16_t __nav_roll,__nav_pitch;
extern uint32_t __pAcc,__numSV;
extern int32_t __altitude_Zd;

void send_package(mavlink_message_t *msg)
{
	/* Try to take the semaphore, if the semaphore is not available now,then delay 1us */
	xSemaphoreTake(mavlink_msg_send_sem, 1 * MICRO_SECOND_TICK);

	/* TODO: Enable DMA mode */
	uint8_t buf[MAVLINK_MAX_PAYLOAD_LEN];
	uint16_t len = mavlink_msg_to_send_buffer(buf, msg);

	int i;
	for(i = 0; i < len; i++)
		usart3_send(buf[i]);

	/* Release the semaphore */
	xSemaphoreGive(mavlink_msg_send_sem);
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

	send_package(&msg);
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

	send_package(&msg);
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

	send_package(&msg);
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
	if(waypoint_info.reached_waypoint.is_update == true) {
		mavlink_message_t msg;		

		/* Notice the ground station that the vehicle is reached at the 
	   	waypoint */
		mavlink_msg_mission_item_reached_pack(1, 0, &msg,
			waypoint_info.reached_waypoint.number);
		send_package(&msg);

		waypoint_info.reached_waypoint.is_update = false;
	}
}

static void send_current_waypoint(void)
{
	if(waypoint_info.current_waypoint.is_update == true) {
		mavlink_message_t msg;		

		/* Update the new current waypoint */
		mavlink_msg_mission_current_pack(1, 0, &msg,
			waypoint_info.current_waypoint.number);
		send_package(&msg);

		waypoint_info.current_waypoint.is_update = false;
	}
}

void transaction_begin(int type)
{
	exist_pending_transaction = true;
	transaction_type = type;
	receiver_sleep_time = 10 * MILLI_SECOND_TICK;
}

void transaction_end(void)
{
	exist_pending_transaction = false;
	receiver_sleep_time = portMAX_DELAY;
}

void reset_transaction_timer(void)
{
	tranaction_start_time = get_system_time_ms();
}

#define TIMER_1HZ  0
#define TIMER_20HZ 1

void ground_station_task(void)
{
	int buffer;
	receiver_sleep_time = portMAX_DELAY; //Sleep until someone wake the task up

	send_heartbeat_info();

	while(1) {
		//Try to receive a byte, and if there is no data, the task won't be wake up
		buffer = usart3_read(receiver_sleep_time);

		/* Try to get the data from usart port if it is available */
		if(buffer != USART_NOT_AVAILABLE) {

			if(mavlink_parse_char(MAVLINK_COMM_0, buffer, &received_msg, &received_status)) {
				printf("%d\n\r", received_msg.msgid);
			
				mavlink_parse_received_cmd(&received_msg);
			}
		}

		/* Transaction timeout handling */
		if(exist_pending_transaction == true) {
			if((get_system_time_ms() - tranaction_start_time) > TRANSACTION_TIMEOUT) {
				/* Resend a request */
				switch(transaction_type) {
				    case WAYPOINT_WRITE_PROTOCOL:
					resend_mission_write_waypoint_list();
					break;
				    case WAYPOINT_READ_PROTOCOL:
					break;
				    case PARAMETER_WRITE_PROTOCOL:
					break;
				    case PARAMETER_READ_PROTOCOL:
					break;
				    case PARAMETER_READ_SINGLE_PROTOCOL:
					break;
				    default:
					break;
				}

				tranaction_start_time = get_system_time_ms();
			}

			transaction_timeout_count++;

			if(transaction_timeout_count == TIMEOUT_COUNT_MAX) {
				transaction_end();
			}
		}
	}
}

void mavlink_send_task()
{
	uint32_t start_time[2] = {get_system_time_ms()};
	uint32_t current_time;

	uint8_t msg_buff[50];
	mavlink_message_t msg;

	while(1) {
		/* Send heartbeat message and gps message in 1hz */
		current_time = get_system_time_ms();
		if((current_time - start_time[TIMER_1HZ]) >= 1000) {
			send_heartbeat_info(); //Heartbeat message should be sent at anytime
			if(exist_pending_transaction == false) {
				send_gps_info();
			}

			start_time[TIMER_1HZ] = current_time;
		}

		/* Send attitude message and waypoint message in 20hz */
		if(exist_pending_transaction == false) {
			if((current_time - start_time[TIMER_20HZ]) >= 50) {
				send_attitude_info();
				send_reached_waypoint();
				send_current_waypoint();

				start_time[TIMER_20HZ] = current_time;
			}
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
