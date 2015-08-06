#include "mavlink.h"

#include "communication.h"
#include "command_parser.h"
#include "mission.h"
#include "parameter.h"

#define CMD_LEN(list) (sizeof(list) / sizeof(struct mavlink_cmd))
#define MAV_CMD_DEF(function, id) [function ## _ID] = {.name = #function, .cmd_handler = function, .msgid = id}

/* Debug printf */
#define USE_MAVLINK_DEBUG_PRINT 1

#if USE_MAVLINK_DEBUG_PRINT == 1
	#define MAVLINK_DEBUG_PRINT(id, name) printf("[%d]%s\n\r", id, name)
#else
	#define MAVLINK_DEBUG_PRINT(...)
#endif

/*
 * Define the Mavlink command enumeration at here then initialize the
 * array.
 */
enum MAV_CMD_ID {
	mission_request_list_handler_ID,
	mission_count_handler_ID,
	mission_request_handler_ID,
	mission_item_handler_ID,
	mission_ack_ID,
	mission_clear_waypoint_ID,
	mission_set_new_current_waypoint_ID,
	parameter_read_value_ID,
	parameter_read_single_value_ID,
	parameter_write_value_ID,
	mission_command_ID,
	MAV_CMD_CNT
};

/*
 * To handle a mavlink command, just create a function which follow the 
 * protocol of the mavlink and fill in the message id.
 */
struct mavlink_cmd cmd_list[] = {
	/* Mission */
	MAV_CMD_DEF(mission_request_list_handler, 43), //Start message handler of the read waypoint transaction
	MAV_CMD_DEF(mission_count_handler, 44), //Start message handler of the write waypoint transaction
	MAV_CMD_DEF(mission_request_handler, 40), //Read waypoint request handler
	MAV_CMD_DEF(mission_item_handler, 39), //Write waypoint request handler
	MAV_CMD_DEF(mission_ack, 47), //End of the read waypoint transaction
	MAV_CMD_DEF(mission_clear_waypoint, 45),
	MAV_CMD_DEF(mission_set_new_current_waypoint, 41),
	MAV_CMD_DEF(mission_command, 76),
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
			MAVLINK_DEBUG_PRINT(cmd_list[i].msgid, cmd_list[i].name);
			cmd_list[i].cmd_handler();
			break;
		}
#if USE_MAVLINK_DEBUG_PRINT == 1
		else {
			if(i == (MAV_CMD_CNT - 1)) {
				printf("[%d]Undefined message for onboard parser\n\r", msg->msgid);
			}
		}
#endif		
	}
}
