#include <stddef.h>
#include <stdlib.h>

#include "QuadCopterConfig.h"
#include "mavlink.h"

#define MAV_MAX_LEN 263
#define CMD_LEN(list) (sizeof(list) / sizeof(struct mavlink_cmd))

/* Mavlink message handlers */
void mission_read_waypoint_list();
void mission_write_waypoint_list();
void mission_clear_waypoint();
void mission_set_new_current_waypoint();

mavlink_message_t received_msg;
mavlink_status_t received_status;

/*
 * To handle a mavlink command, just create a function which follow the 
 * protocol of the mavkink and fill in the message id.
 */
struct mavlink_cmd cmd_list[] = {
	/* flight mission clear command */
	[0] = {.cmd_handler = mission_read_waypoint_list, .msgid = 43},
	[1] = {.cmd_handler = mission_write_waypoint_list, .msgid = 44},
	[2] = {.cmd_handler = mission_clear_waypoint, .msgid = 45},
	[3] = {.cmd_handler = mission_set_new_current_waypoint, .msgid = 42}
};

void send_package(uint8_t *buf, mavlink_message_t *msg)
{
	uint16_t len = mavlink_msg_to_send_buffer(buf, msg);

	int i;
	for(i = 0; i < len; i++)
		serial.putc(buf[i]);
}

void send_vehicle_info()
{
	mavlink_message_t msg;
	uint8_t buf[MAV_MAX_LEN];

	/* Test - QuadCopter Heart Beat */
	mavlink_msg_heartbeat_pack(1, 200, &msg,
		MAV_TYPE_QUADROTOR, 
		MAV_AUTOPILOT_GENERIC, 
		MAV_MODE_GUIDED_ARMED, 
		0, MAV_STATE_ACTIVE
	);
	send_package(buf, &msg);
		
	/* Test - Position (By GPS) */
	mavlink_msg_global_position_int_pack(1, 220, &msg, /*time*/0,  
		22.999326 * 1E7, 120.219416 * 1E7,
		100*1000, 10 * 1000, 1 * 100, 1 * 100,
		 1 * 100, 45
	);
	send_package(buf, &msg);

	/* Test - Attitude */
	mavlink_msg_attitude_pack(1, 200, &msg, 0,
		toRad( system.variable[TRUE_ROLL].value ), 
		toRad( system.variable[TRUE_PITCH].value ), 
		toRad( system.variable[TRUE_YAW].value ), 
		0.0, 0.0, 0.0
	);
	send_package(buf, &msg);

	/* Test - Ack Message */
	//mavlink_msg_command_ack_pack(1, 200, &msg, MAV_CMD_NAV_WAYPOINT, MAV_RESULT_ACCEPTED);
	//send_package(buf, &msg);


	/* Test - Debug Message */
	//mavlink_msg_named_value_int_pack(1, 200, &msg, 0, "msg-id", received_msg.msgid);
	//send_package(buf, &msg);
}

void parse_received_cmd(mavlink_message_t *msg)
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
		send_vehicle_info();

		parse_received_cmd(&received_msg);
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
