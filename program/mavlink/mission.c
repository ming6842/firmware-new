#include <stdbool.h>

#include "memory.h"

#include "FreeRTOS.h"
#include "task.h"

#include "mavlink.h"
#include "common.h"

#include "global.h"
#include "communication.h"
#include "mission.h"
#include "system_time.h"
#include "navigation.h"

#define REGESTERED_MISSION_MSG_CNT (sizeof(mission_list) / sizeof(mission_list[0]))

static void mission_request_list_handler(mavlink_message_t *mavlink_message);
static void mission_request_handler(mavlink_message_t *mavlink_message);
static void mission_ack_handler(mavlink_message_t *mavlink_message);
static void mission_count_handler(mavlink_message_t *mavlink_message);
static void mission_item_handler(mavlink_message_t *mavlink_message);

/* Navigation manger */
extern bool nav_waypoint_list_is_updated;
extern bool got_set_current_command;

mavlink_message_t msg;

/* Mission manager */
waypoint_info_t waypoint_info;

struct mission_parsed_item mission_list[] = {
	/* Read waypoint protocol */
        MISSION_MSG_DEF(MAVLINK_MSG_ID_MISSION_REQUEST_LIST, mission_request_list_handler), //#43
        MISSION_MSG_DEF(MAVLINK_MSG_ID_MISSION_REQUEST, mission_request_handler), //#40
        MISSION_MSG_DEF(MAVLINK_MSG_ID_MISSION_ACK, mission_ack_handler), //#47
	/* Write waypoint protocol */
        MISSION_MSG_DEF(MAVLINK_MSG_ID_MISSION_COUNT, mission_count_handler), //#44
        MISSION_MSG_DEF(MAVLINK_MSG_ID_MISSION_ITEM, mission_item_handler), //#39
};

/**
  * @brief  Try to parse and handle the passed message
  * @param  Received mavlink message (pointer) 
  * @retval The message can be handled or not (is this a mission related message?)
  */
bool mission_handle_message(mavlink_message_t *mavlink_message)
{
	int i;
	for(i = 0; i < REGESTERED_MISSION_MSG_CNT; i++) {
		if(mavlink_message->msgid == mission_list[i].msgid) {
			mission_list[i].message_handler(mavlink_message);
			return true;
		}
	}

	return false;
}

/**
  * @brief  Get the home waypoint information 
  * @param  latitude, longitude, altitude (float* to get the result value)
	    use_current (int, is a flag)
  * @retval Waypoint status (WAYPOINT_IS_SET / WAYPOINT_NOT_SET)
  */
int get_home_waypoint_info(float *latitude, float *longitude, float *altitude, int *use_current)
{
	if(waypoint_info.home_waypoint.is_set == true) {
		*latitude = waypoint_info.home_waypoint.latitude;
		*longitude = waypoint_info.home_waypoint.longitude;
		*altitude = waypoint_info.home_waypoint.altitude;
		*use_current = waypoint_info.home_waypoint.use_current;
		return WAYPOINT_IS_SET;
	} else {
		return WAYPOINT_NOT_SET;
	}
}


/**
  * @brief  Get the mission flight status 
  * @param  None
  * @retval Status (int, MAV_GOTO_DO_HOLD and MAV_GOTO_DO_CONTINUE)
  */
int get_mission_flight_status(void)
{
	return (int)waypoint_info.mission_status;
}

/**
  * @brief  Get the mission hold waypoint information 
  * @param  latitude, longitude, altitude, yaw angle (float* to get the result value)
  *	    coordinate frame type (int* to get the result value)
  *	    Waypoint type (int, MAV_GOTO_HOLD_AT_CURRENT_POSITION and
  *	    MAV_GOTO_HOLD_AT_SPECIFIED_POSITION)
  * @retval Waypoint status (WAYPOINT_IS_SET / WAYPOINT_NOT_SET)
  */
int get_hold_waypoint_position(float *latitude, float *longitude, float *altitude,
	int *coordinate_frame, float *yaw_angle, int *hold_waypoint)
{
	if(waypoint_info.hold_waypoint.is_set == true) {
		/* Position */
		*latitude = waypoint_info.hold_waypoint.latitude; 
		*longitude = waypoint_info.hold_waypoint.longitude;
		*altitude = waypoint_info.hold_waypoint.altitude;
		/* Coordinate type */
		*coordinate_frame = waypoint_info.hold_waypoint.coordinate_frame;
		/* Yaw angle*/
		*yaw_angle = waypoint_info.hold_waypoint.yaw_angle;
		/* Waypoint type */
		*hold_waypoint = waypoint_info.hold_waypoint.hold_waypoint;

		return WAYPOINT_IS_SET;
	} else {
		return WAYPOINT_NOT_SET;
	}
}

/**
  * @brief  Get the current waypoint number
  * @param  None
  * @retval current waypoint number (int)
  */
int get_current_waypoint_number(void)
{
	return waypoint_info.current_waypoint.number;
}

/**
  * @brief  Set the new current waypoint number
  * @param  new waypoint number
  * @retval None
  */
void set_current_waypoint_number(int new_waypoint_num)
{
	waypoint_info.current_waypoint.number = new_waypoint_num;
	waypoint_info.current_waypoint.is_update = true;
}

/**
  * @brief  Set the reached waypoint number
  * @param  reached waypoint number
  * @retval None
  */
void set_reached_waypoint_number(int reached_waypoint_num)
{
	waypoint_info.reached_waypoint.number = reached_waypoint_num;
	waypoint_info.reached_waypoint.is_update = true;
}

static void free_waypoint_list()
{
	waypoint_info.waypoint_count = 0;
}

waypoint_t *get_waypoint(waypoint_t *wp_list, int index)
{
	if(index == 0) {
		return wp_list;
	} else if(index >= waypoint_info.waypoint_count) {
		//XXX:Should assert the developer here...
		return NULL;
	} else {
		return &waypoint_info.waypoint_list[index];
	}
} 

int current_waypoint_index;

static void mission_request_list_handler(mavlink_message_t *mavlink_message)
{
	transaction_begin(WAYPOINT_READ_PROTOCOL);

	current_waypoint_index = 0;

	mavlink_msg_mission_count_pack(
		1, 0, &msg, 255, 0, waypoint_info.waypoint_count /* Waypoint count */
	);
	send_package(&msg);

	reset_transaction_timer();
}

static void mission_request_handler(mavlink_message_t *mavlink_message)
{
	/* Decode the message to know which waypoint need to be sent */
	mavlink_mission_request_t mmrt;
	mavlink_msg_mission_request_decode(mavlink_message, &mmrt);

	/* Send the waypoint to the ground station */
	mavlink_msg_mission_item_pack(
		1, 0, &msg, 255, 0,
		waypoint_info.waypoint_list[mmrt.seq].data.seq,
		waypoint_info.waypoint_list[mmrt.seq].data.frame,
		waypoint_info.waypoint_list[mmrt.seq].data.command,
		waypoint_info.waypoint_list[mmrt.seq].data.current,
		waypoint_info.waypoint_list[mmrt.seq].data.autocontinue,
		waypoint_info.waypoint_list[mmrt.seq].data.param1,
		waypoint_info.waypoint_list[mmrt.seq].data.param2,
		waypoint_info.waypoint_list[mmrt.seq].data.param3,
		waypoint_info.waypoint_list[mmrt.seq].data.param4,
		waypoint_info.waypoint_list[mmrt.seq].data.x,
		waypoint_info.waypoint_list[mmrt.seq].data.y,
		waypoint_info.waypoint_list[mmrt.seq].data.z
	);
		
	send_package(&msg);

	reset_transaction_timer();
}

static void mission_ack_handler(mavlink_message_t *mavlink_message)
{
	/* XXX:Check the ack state? */

	transaction_end();
}

static void mission_count_handler(mavlink_message_t *mavlink_message)
{
	transaction_begin(WAYPOINT_WRITE_PROTOCOL);

	current_waypoint_index = 0;

	waypoint_info.is_busy = true;

	//free_waypoint_list();

	waypoint_info.waypoint_count = mavlink_msg_mission_count_get_count(mavlink_message);

	if(waypoint_info.waypoint_count > WAYPOINT_LIMIT) {
		send_status_text_message("Error: waypoint count is over the maximum limit!");

		//XXX: Reject the write waypoint list request?
	}

	/* Request to get the first waypoint */
	mavlink_msg_mission_request_pack(1, 0, &msg, 255, 0, 0);
	send_package(&msg);

	reset_transaction_timer();
}

void resend_mission_write_waypoint_list(void)
{
	/* Request to get the next waypoint */
	mavlink_msg_mission_request_pack(1, 0, &msg, 255, 0, current_waypoint_index);
	send_package(&msg);
}

static void mission_item_handler(mavlink_message_t *mavlink_message)
{
	/* Decode and get the new waypoint */
	mavlink_msg_mission_item_decode(
		mavlink_message,
		&(waypoint_info.waypoint_list[current_waypoint_index].data)
	);

	/* Check the sequence of received waypoint is correct or not  */
	if(waypoint_info.waypoint_list[current_waypoint_index].data.seq != current_waypoint_index) {
		return; //Return and wait for the waypoint message again

		//XXX:Send the waypoint request again?
	}

	current_waypoint_index++; //Next waypoint

	/* Request to get the next waypoint */
	mavlink_msg_mission_request_pack(1, 0, &msg, 255, 0, current_waypoint_index);
	send_package(&msg);

	reset_transaction_timer();

	/* End of the mission waypoint write transaction */
	if(current_waypoint_index == (waypoint_info.waypoint_count - 1)) {
		/* Update the wayppoint, navigation manager */
		waypoint_info.is_busy = false;
		nav_waypoint_list_is_updated = false; /* From navigation point of view */

		/* Send a mission ack Message at the end */
		mavlink_msg_mission_ack_pack(1, 0, &msg, 255, 0, 0);
		send_package(&msg);

		transaction_end();
	}
}

static void mission_clear_waypoint(mavlink_message_t *mavlink_message)
{
	waypoint_info.is_busy = true;

	/* Free the waypoint list */
	free_waypoint_list();

	waypoint_info.is_busy = false;
	nav_waypoint_list_is_updated = false;

	/* Send a mission ack Message at the end */
	mavlink_msg_mission_ack_pack(1, 0, &msg, 255, 0, 0);
	send_package(&msg);
}

static mission_set_new_current_waypoint(mavlink_message_t *mavlink_message)
{
	mavlink_mission_set_current_t mmst;
	mavlink_msg_mission_set_current_decode(mavlink_message, &mmst);

	/* Clear the old current waypoint flag */
	waypoint_info.waypoint_list[waypoint_info.current_waypoint.number].data.current = 0;	

	/* Getting the seq of current waypoint */
	/* Ming change : I think should set this too*/
	waypoint_info.current_waypoint.number = mmst.seq;
	Nav_update_current_wp_id(mmst.seq);

	/* Set the new waypoint flag */
	waypoint_info.waypoint_list[waypoint_info.current_waypoint.number].data.current = 1;

	/* Send back the current waypoint seq as ack message */
	mavlink_msg_mission_current_pack(1, 0, &msg, waypoint_info.current_waypoint.number);
	send_package(&msg);
}

static void mission_command(mavlink_message_t *mavlink_message)
{
	mavlink_command_long_t mmcl;
	mavlink_msg_command_long_decode(mavlink_message, &mmcl);

	switch(mmcl.command) {
	    case MAV_CMD_DO_SET_MODE:
		break;
	    case MAV_CMD_DO_JUMP:
		break;
	    case MAV_CMD_DO_CHANGE_SPEED:
		break;
	    case MAV_CMD_DO_SET_HOME:
		waypoint_info.home_waypoint.latitude = mmcl.param5;
		waypoint_info.home_waypoint.longitude = mmcl.param6;
		waypoint_info.home_waypoint.altitude = mmcl.param7;
		waypoint_info.home_waypoint.use_current = (int)mmcl.param1;
		waypoint_info.home_waypoint.is_set = true;
		break;
	    case MAV_CMD_OVERRIDE_GOTO:
		waypoint_info.mission_status  = (int)mmcl.param1;
		waypoint_info.hold_waypoint.latitude = mmcl.param5; 
		waypoint_info.hold_waypoint.longitude = mmcl.param6;
		waypoint_info.hold_waypoint.altitude = mmcl.param7;
		waypoint_info.hold_waypoint.coordinate_frame = (int)mmcl.param3;
		waypoint_info.hold_waypoint.yaw_angle = mmcl.param4;
		waypoint_info.hold_waypoint.hold_waypoint = (int)mmcl.param2;
		waypoint_info.hold_waypoint.is_set = true;
		break;
	    case MAV_CMD_MISSION_START:
		break;
	    case MAV_CMD_COMPONENT_ARM_DISARM:
		break;
	    default:
		break;
	}
}
