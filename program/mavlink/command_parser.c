#include "mavlink.h"
#include "command_parser.h"
#include "mission.h"
#include "parameter.h"

#define CMD_LEN(list) (sizeof(list) / sizeof(struct mavlink_cmd))
#define MAV_CMD_DEF(name, id) [name ## _ID] = {.cmd_handler = name, .msgid = id}

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

void mavlink_parse_received_cmd(mavlink_message_t *msg)
{
	int i;
	for(i = 0; i < (signed int)CMD_LEN(cmd_list); i++) {
		if(msg->msgid == cmd_list[i].msgid) {
			cmd_list[i].cmd_handler();
			clear_message_id(msg);
		}
	}
}
