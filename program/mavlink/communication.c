#include <stddef.h>
#include <stdlib.h>
#include "mavlink.h"

#include "usart.h"

#include "_math.h"
#include "global.h"
#include "communication.h"
#include "mission.h"
#include "parameter.h"

#define CMD_LEN(list) (sizeof(list) / sizeof(struct mavlink_cmd))
#define MAV_CMD_DEF(name, id) [name ## _ID] = {.cmd_handler = name, .msgid = id}

mavlink_message_t received_msg;
mavlink_status_t received_status;

/*
 * Define the Mavlink command enumeration at here then initialize the
 * array.
 */
enum MAV_CMD_ID {
	mission_read_waypoint_list_ID,
	mission_write_waypoint_list_ID,
	mission_clear_waypoint_ID,
	mission_set_new_current_waypoint_ID,
	parameter_read_value_ID,
	parameter_read_single_value_ID,
	parameter_write_value_ID,
	MAV_CMD_CNT
};

/*
 * To handle a mavlink command, just create a function which follow the 
 * protocol of the mavlink and fill in the message id.
 */
struct mavlink_cmd cmd_list[] = {
	/* Mission */
	MAV_CMD_DEF(mission_read_waypoint_list, 43),
	MAV_CMD_DEF(mission_write_waypoint_list, 44),
	MAV_CMD_DEF(mission_clear_waypoint, 45),
	MAV_CMD_DEF(mission_set_new_current_waypoint, 41),
	/* Onboard parameter */
	MAV_CMD_DEF(parameter_read_value, 21),
	MAV_CMD_DEF(parameter_read_single_value, 20),
	MAV_CMD_DEF(parameter_write_value, 23)
};

void send_package(mavlink_message_t *msg)
{
	uint8_t buf[MAVLINK_MAX_PAYLOAD_LEN];
	uint16_t len = mavlink_msg_to_send_buffer(buf, msg);

	int i;
	for(i = 0; i < len; i++)
		usart3_send(buf[i]);
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
	mavlink_message_t msg;

	mavlink_msg_global_position_int_pack(1, 220, &msg, 
		get_boot_time(),   		       //time 
		(double)read_global_data_float(GPS_LAT) * 1E7,  //Latitude
		(double)read_global_data_float(GPS_LON) * 1E7,  //Longitude
		(double)read_global_data_float(GPS_ALT) * 1000, //Altitude
		10 * 1000,
		read_global_data_float(GPS_VX) * 100,   //Speed-Vx
		read_global_data_float(GPS_VY) * 100,   //Speed-Vy
		read_global_data_float(GPS_VZ) * 100,   //Speed-Vz
		45
	);

	send_package(&msg);
}

static void send_attitude_info(void)
{
	mavlink_message_t msg;

	mavlink_msg_attitude_pack(1, 200, &msg,
		get_boot_time(),
		toRad(read_global_data_float(TRUE_ROLL)), 
		toRad(read_global_data_float(TRUE_PITCH)), 
		toRad(read_global_data_float(TRUE_YAW)), 
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

static void mavlink_parse_received_cmd(mavlink_message_t *msg)
{
	int i;
	for(i = 0; i < (signed int)CMD_LEN(cmd_list); i++) {
		if(msg->msgid == cmd_list[i].msgid)
			cmd_list[i].cmd_handler();
	}
}

void ground_station_send_task(void)
{
	while(1) {
		send_heartbeat_info();
		//send_system_info();
		send_attitude_info();
		send_gps_info();

		mavlink_parse_received_cmd(&received_msg);

		vTaskDelay(80*100);
	}
}

void ground_station_receive_task(void)
{
	uint8_t buffer;

	while(1) {
		buffer = usart3_read();

		mavlink_parse_char(MAVLINK_COMM_0, buffer, &received_msg, &received_status); 
		vTaskDelay(10);
	}
}
