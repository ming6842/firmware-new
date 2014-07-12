#include <stddef.h>
#include <stdlib.h>

#include "_math.h"

#include "usart.h"

#include "FreeRTOS.h"
#include "task.h"

#include "mavlink.h"

#include "global.h"

#include "communication.h"
#include "command_parser.h"
#include "FreeRTOS.h"
#include "system_time.h"
mavlink_message_t received_msg;
mavlink_status_t received_status;

void send_package(mavlink_message_t *msg)
{
	uint8_t buf[MAVLINK_MAX_PAYLOAD_LEN];
	uint16_t len = mavlink_msg_to_send_buffer(buf, msg);

	int i;
	for(i = 0; i < len; i++)
		usart3_send(buf[i]);
}

void clear_message_id(mavlink_message_t *message)
{
	message->msgid = 0;
}

static void send_heartbeat_info(void)
{
	mavlink_message_t msg;
	
	mavlink_msg_heartbeat_pack(1, 200, &msg,
		MAV_TYPE_QUADROTOR, 
		MAV_AUTOPILOT_GENERIC, 
		MAV_MODE_GUIDED_ARMED, 
		0, MAV_STATE_ACTIVE
	);

	send_package(&msg);
}

static void send_gps_info(void)
{
	float latitude, longitude, altitude;
	float gps_vx, gps_vy, gps_vz;

	/* Prepare the GPS data */
	read_global_data_float(GPS_LAT, &latitude);
	read_global_data_float(GPS_LON, &longitude);
	read_global_data_float(GPS_ALT, &altitude);
	read_global_data_float(GPS_VX, &gps_vx);
	read_global_data_float(GPS_VY, &gps_vy);
	read_global_data_float(GPS_VZ, &gps_vz);

	mavlink_message_t msg;

	mavlink_msg_global_position_int_pack(1, 220, &msg, 
		get_boot_time(),   		       //time 
		(double)latitude * 1E7,  //Latitude
		(double)longitude * 1E7,  //Longitude
		(double)altitude * 1000, //Altitude
		10 * 1000,
		gps_vx * 100,   //Speed-Vx
		gps_vy * 100,   //Speed-Vy
		gps_vz * 100,   //Speed-Vz
		45
	);

	send_package(&msg);
}

static void send_attitude_info(void)
{
	mavlink_message_t msg;
	float attitude_roll, attitude_pitch, attitude_yaw;

	/* Prepare the attitude data */
	read_global_data_float(TRUE_ROLL, &attitude_roll);
	read_global_data_float(TRUE_PITCH, &attitude_pitch);
	read_global_data_float(TRUE_YAW, &attitude_yaw);

	mavlink_msg_attitude_pack(1, 200, &msg,
		get_boot_time(),
		toRad(attitude_roll), 
		toRad(attitude_pitch), 
		toRad(attitude_yaw), 
		0.0, 0.0, 0.0
	);

	send_package(&msg);
}

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

void ground_station_task(void)
{
	uint32_t delay_t =(uint32_t) 100.0/(1000.0 / configTICK_RATE_HZ);
	uint32_t cnt = 0;
	
	while(1) {
		if(cnt == 10) {
			send_heartbeat_info();
			//send_system_info();
			send_attitude_info();
			send_gps_info();
			cnt = 0;
		}

		vTaskDelay(delay_t);

		mavlink_parse_received_cmd(&received_msg);
		cnt++;
		
	}
}

void mavlink_receiver_task(void)
{
	uint8_t buffer;

	while(1) {
		buffer = usart3_read();

		mavlink_parse_char(MAVLINK_COMM_0, buffer, &received_msg, &received_status); 
	}
}
