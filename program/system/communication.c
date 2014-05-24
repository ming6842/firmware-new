#include <stddef.h>
#include <stdlib.h>

#include "QuadCopterConfig.h"

#define MAV_MAX_LEN 263
#define CMD_LEN(list) (sizeof(list) / sizeof(struct mavlink_cmd))

#define MAV_CMD_DEF(name, id) [name ## _ID] = {.cmd_handler = name, .msgid = id}

/* Mavlink message handlers */
void mission_read_waypoint_list();
void mission_write_waypoint_list();
void mission_clear_waypoint();
void mission_set_new_current_waypoint();

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
	MAV_CMD_CNT
};

/*
 * To handle a mavlink command, just create a function which follow the 
 * protocol of the mavkink and fill in the message id.
 */
struct mavlink_cmd cmd_list[] = {
	/* flight mission clear command */
	MAV_CMD_DEF(mission_read_waypoint_list, 43),
	MAV_CMD_DEF(mission_write_waypoint_list, 44),
	MAV_CMD_DEF(mission_clear_waypoint, 45),
	MAV_CMD_DEF(mission_set_new_current_waypoint, 41)
};

void send_package(mavlink_message_t *msg)
{
	uint8_t buf[MAV_MAX_LEN];
	uint16_t len = mavlink_msg_to_send_buffer(buf, msg);

	int i;
	for(i = 0; i < len; i++)
		serial.putc(buf[i]);
}

void send_heartbeat_info()
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

void send_gps_info()
{
	mavlink_message_t msg;

	mavlink_msg_global_position_int_pack(1, 220, &msg, 
		get_boot_time(),   		       //time 
		read_global_data_float(GPS_LAT) * 1E7,  //Latitude
		read_global_data_float(GPS_LON) * 1E7,  //Longitude
		read_global_data_float(GPS_ALT) * 1000, //Altitude
		10 * 1000,
		read_global_data_float(GPS_VX) * 100,   //Speed-Vx
		read_global_data_float(GPS_VY) * 100,   //Speed-Vy
		read_global_data_float(GPS_VZ) * 100,   //Speed-Vz
		45
	);

	send_package(&msg);
}

void send_attitude_info()
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

void send_system_info()
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

/*
 * The function "send_global_info" is designed for transmit and show up
 * the data in order to improve the filter.
 * This function will be dropped after all things is fine.
 */
void send_global_info()
{
	mavlink_message_t msg;
	uint8_t buf[MAV_MAX_LEN], *pbuf = buf;
	
	/* QuadCopter Heart Beat */
	mavlink_msg_heartbeat_pack(1, 200, &msg,
		MAV_TYPE_QUADROTOR, 
		MAV_AUTOPILOT_GENERIC, 
		MAV_MODE_GUIDED_ARMED, 
		0, MAV_STATE_ACTIVE
	);
	pbuf += mavlink_msg_to_send_buffer(pbuf, &msg);
		
	/* Position (By GPS) */
	mavlink_msg_global_position_int_pack(1, 220, &msg, /*time*/get_boot_time(),  
		22.999326 * 1E7, 120.219416 * 1E7,
		100*1000, 10 * 1000, 1 * 100, 1 * 100,
		 1 * 100, 45
	);
	pbuf += mavlink_msg_to_send_buffer(pbuf, &msg);

	/* Attitude */
	mavlink_msg_attitude_pack(1, 200, &msg, /*time*/get_boot_time(),
		toRad( read_global_data_float(TRUE_ROLL) ), 
		toRad( read_global_data_float(TRUE_PITCH) ), 
		toRad( read_global_data_float(TRUE_YAW) ), 
		0.0, 0.0, 0.0
	);
	pbuf += mavlink_msg_to_send_buffer(pbuf, &msg);

	mavlink_msg_named_value_int_pack(1, 0, &msg,
		get_boot_time(), "boot time", get_boot_time()
	);

	pbuf += mavlink_msg_to_send_buffer(pbuf, &msg);

	int i;
	for(i = 0; i < (pbuf- buf); i++)
		serial.putc(buf[i]);
}

void mavlink_parse_received_cmd(mavlink_message_t *msg)
{
	int i;
	for(i = 0; i < CMD_LEN(cmd_list); i++) {
		if(msg->msgid == cmd_list[i].msgid)
			cmd_list[i].cmd_handler();
	}
}

void ground_station_send_task()
{
	while(1) {
#if configGCS_HIGHSPEED == 1
		/* High speed transmit mode for development using */
		send_global_info();
#else
		/* Normal */
		send_heartbeat_info();
		send_system_info();
		send_attitude_info();
		send_gps_info();
#endif
		mavlink_parse_received_cmd(&received_msg);
	}
}

void ground_station_receive_task()
{
	uint8_t buf[MAV_MAX_LEN];
	int buf_cnt = 0;

	while(1) {
		buf[buf_cnt] = serial.getc();

		mavlink_parse_char(MAVLINK_COMM_0, buf[buf_cnt], &received_msg, &received_status); 
	}
}
