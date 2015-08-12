#include <stdbool.h>

#include "memory.h"
#include "delay.h"

#include "FreeRTOS.h"
#include "task.h"

#include "mavlink.h"
#include "common.h"

#include "global.h"
#include "communication.h"
#include "mission.h"
#include "system_time.h"
#include "navigation.h"

#define REGISTERED_MISSION_MSG_CNT (sizeof(mission_list) / sizeof(mission_list[0]))

#define MISSION_PROTOCOL_TIMEOUT 5000 //5 3seconds (in ms)
#define MISSION_RETRY_TIMEOUT 500 //half second (in ms)

#define MISSION_DEBUG_PRINT printf

static void mission_request_list_handler(mavlink_message_t *mavlink_message);
static void mission_request_handler(mavlink_message_t *mavlink_message);
static void mission_ack_handler(mavlink_message_t *mavlink_message);
static void mission_count_handler(mavlink_message_t *mavlink_message);
static void mission_item_handler(mavlink_message_t *mavlink_message);
static void mission_clear_all_handler(mavlink_message_t *mavlink_message);
static void mission_set_current_waypoint_handler(mavlink_message_t *mavlink_message);
static void command_long_handler(mavlink_message_t *mavlink_message);

/* Navigation manger */
extern bool nav_waypoint_list_is_updated;
extern bool got_set_current_command;

/* Mission manager */
mission_info_t mission_info;

struct mission_parsed_item mission_list[] = {
	/* Read waypoint protocol */
        MISSION_MSG_DEF(MAVLINK_MSG_ID_MISSION_REQUEST_LIST, mission_request_list_handler), //#43
        MISSION_MSG_DEF(MAVLINK_MSG_ID_MISSION_REQUEST, mission_request_handler), //#40
        MISSION_MSG_DEF(MAVLINK_MSG_ID_MISSION_ACK, mission_ack_handler), //#47
	/* Write waypoint protocol */
        MISSION_MSG_DEF(MAVLINK_MSG_ID_MISSION_COUNT, mission_count_handler), //#44
        MISSION_MSG_DEF(MAVLINK_MSG_ID_MISSION_ITEM, mission_item_handler), //#39
	/* Waypoint clear */
	MISSION_MSG_DEF(MAVLINK_MSG_ID_MISSION_CLEAR_ALL, mission_clear_all_handler), //#45
	/* Waypoint set */
	MISSION_MSG_DEF(MAVLINK_MSG_ID_MISSION_SET_CURRENT, mission_set_current_waypoint_handler), //#41
	/* Mission command */
	MISSION_MSG_DEF(MAVLINK_MSG_ID_COMMAND_LONG, command_long_handler) //#76
};

/**
  * @brief  Try to parse and handle the passed message
  * @param  Received mavlink message (pointer) 
  * @retval The message can be handled or not (is this a mission related message?)
  */
bool mission_handle_message(mavlink_message_t *mavlink_message)
{
	unsigned int i;
	for(i = 0; i < REGISTERED_MISSION_MSG_CNT; i++) {
		if(mavlink_message->msgid == mission_list[i].msgid) {
			mission_list[i].message_handler(mavlink_message);
			MISSION_DEBUG_PRINT("%s\n\r", mission_list[i].name);
			return true;
		}
	}

	return false;
}

bool mission_handler_is_busy(void)
{
	if(mission_info.mavlink_state != MISSION_STATE_IDLE) {
		return true;
	}

	return false;
}

int get_mavlink_mission_state(void)
{
	return mission_info.mavlink_state;
}

/**
  * @brief  Get the home waypoint information 
  * @param  latitude, longitude, altitude (float* to get the result value)
	    use_current (int, is a flag)
  * @retval Waypoint status (WAYPOINT_IS_SET / WAYPOINT_NOT_SET)
  */
int get_home_mission_info(float *latitude, float *longitude, float *altitude, int *use_current)
{
	if(mission_info.home_waypoint.is_set == true) {
		*latitude = mission_info.home_waypoint.latitude;
		*longitude = mission_info.home_waypoint.longitude;
		*altitude = mission_info.home_waypoint.altitude;
		*use_current = mission_info.home_waypoint.use_current;
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
	return (int)mission_info.mission_status;
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
	if(mission_info.hold_waypoint.is_set == true) {
		/* Position */
		*latitude = mission_info.hold_waypoint.latitude; 
		*longitude = mission_info.hold_waypoint.longitude;
		*altitude = mission_info.hold_waypoint.altitude;
		/* Coordinate type */
		*coordinate_frame = mission_info.hold_waypoint.coordinate_frame;
		/* Yaw angle*/
		*yaw_angle = mission_info.hold_waypoint.yaw_angle;
		/* Waypoint type */
		*hold_waypoint = mission_info.hold_waypoint.hold_waypoint;

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
	return mission_info.current_waypoint.number;
}

/**
  * @brief  Set the new current waypoint number
  * @param  new waypoint number
  * @retval None
  */
void set_current_waypoint_number(int new_waypoint_num)
{
	mission_info.current_waypoint.number = new_waypoint_num;
	mission_info.current_waypoint.is_update = true;
}

/**
  * @brief  Set the reached waypoint number
  * @param  reached waypoint number
  * @retval None
  */
void set_reached_waypoint_number(int reached_waypoint_num)
{
	mission_info.reached_waypoint.number = reached_waypoint_num;
	mission_info.reached_waypoint.is_update = true;
}

static void free_waypoint_list(void)
{
	mission_info.waypoint_count = 0;
}

waypoint_t *get_waypoint(waypoint_t *wp_list, int index)
{
	if(index == 0) {
		return wp_list;
	} else if(index >= mission_info.waypoint_count) {
		//XXX:Should assert the developer here...
		return NULL;
	} else {
		return &mission_info.waypoint_list[index];
	}
} 

/**************************************
 * Waypoint read transaction handlers *
 **************************************/

/* @brief: handle mavlink message #43 - MISSION_REQUEST_LIST */
static void mission_request_list_handler(__attribute__((__unused__))mavlink_message_t *mavlink_message)
{
	if(mission_info.mavlink_state == MISSION_STATE_IDLE || mission_info.mavlink_state == MISSION_STATE_SEND_LIST) {
		mission_info.mavlink_state = MISSION_STATE_SEND_LIST;

		mission_info.sent_waypoint_count = 0; //"Sent" means send to the ground station

		mavlink_message_t msg;
		mavlink_msg_mission_count_pack(1, 0, &msg, 255, 0, mission_info.waypoint_count);
		receiver_task_send_package(&msg);

		if(mission_info.waypoint_count > 0) {
			set_mavlink_receiver_delay_time(MILLI_SECOND_TICK * 10);
		} else {
			/* No waypoint, stop the transaction */
			mission_info.mavlink_state = MISSION_STATE_IDLE;
			return;
		}

		//Reset timers
		mission_info.timeout_start_time = get_system_time_ms();
		mission_info.last_retry_time = get_system_time_ms();
	}
}

/* @brief: handle mavlink message #40 - MISSION_REQUEST */
static void mission_request_handler(mavlink_message_t *mavlink_message)
{
	if(mission_info.mavlink_state == MISSION_STATE_SEND_LIST) {
		/* Decode the message to know which waypoint need to be sent */
		mavlink_mission_request_t mmrt;
		mavlink_msg_mission_request_decode(mavlink_message, &mmrt);

		/* Send the waypoint to the ground station */
		mavlink_message_t msg;
		mavlink_msg_mission_item_pack(
			1, 0, &msg, 255, 0,
			mission_info.waypoint_list[mmrt.seq].data.seq,
			mission_info.waypoint_list[mmrt.seq].data.frame,
			mission_info.waypoint_list[mmrt.seq].data.command,
			mission_info.waypoint_list[mmrt.seq].data.current,
			mission_info.waypoint_list[mmrt.seq].data.autocontinue,
			mission_info.waypoint_list[mmrt.seq].data.param1,
			mission_info.waypoint_list[mmrt.seq].data.param2,
			mission_info.waypoint_list[mmrt.seq].data.param3,
			mission_info.waypoint_list[mmrt.seq].data.param4,
			mission_info.waypoint_list[mmrt.seq].data.x,
			mission_info.waypoint_list[mmrt.seq].data.y,
			mission_info.waypoint_list[mmrt.seq].data.z
		);	
		receiver_task_send_package(&msg);

		mission_info.sent_waypoint_count++;

		//Reset timers
		mission_info.timeout_start_time = get_system_time_ms();
		mission_info.last_retry_time = get_system_time_ms();
	}
}

/* @brief: handle mavlink message #47 - MISSION_ACK */
static void mission_ack_handler(__attribute__((__unused__))mavlink_message_t *mavlink_message)
{
	if(mission_info.mavlink_state == MISSION_STATE_SEND_LIST) {
		mission_info.mavlink_state = MISSION_STATE_IDLE;

		if(mission_info.sent_waypoint_count < mission_info.waypoint_count) {
			//Not finish sending every waypoint but receive the ack message!
			send_status_text_message("#Received ack message before sending all waypoints", MAV_SEVERITY_ERROR);

			mission_info.sent_waypoint_count = 0;
		} else {
			//Transaction succeeded
			send_status_text_message("#Mission read complete", MAV_SEVERITY_INFO);
		}

		set_mavlink_receiver_delay_time(portMAX_DELAY);
	}
}

void handle_mission_read_timeout(void)
{
	if(mission_info.mavlink_state == MISSION_STATE_SEND_LIST) {
		/* Transaction time check  */
		if((get_system_time_ms() - mission_info.timeout_start_time) <= MISSION_PROTOCOL_TIMEOUT) {
			/* Retry time check */
			if((get_system_time_ms() - mission_info.last_retry_time) >= MISSION_RETRY_TIMEOUT) {
				mavlink_message_t msg;

				if(mission_info.sent_waypoint_count > 0) {
					int index = mission_info.sent_waypoint_count - 1;

					/* Send the waypoint again */
					mavlink_msg_mission_item_pack(
						1, 0, &msg, 255, 0,
						mission_info.waypoint_list[index].data.seq,
						mission_info.waypoint_list[index].data.frame,
						mission_info.waypoint_list[index].data.command,
						mission_info.waypoint_list[index].data.current,
						mission_info.waypoint_list[index].data.autocontinue,
						mission_info.waypoint_list[index].data.param1,
						mission_info.waypoint_list[index].data.param2,
						mission_info.waypoint_list[index].data.param3,
						mission_info.waypoint_list[index].data.param4,
						mission_info.waypoint_list[index].data.x,
						mission_info.waypoint_list[index].data.y,
						mission_info.waypoint_list[index].data.z
					);

					receiver_task_send_package(&msg);	
				}
				printf("[Retry]Read protocol\n\r");
				mission_info.last_retry_time = get_system_time_ms();
			}
		} else {
			printf("[Timeout]read protocol\n\r");
			mission_info.mavlink_state = MISSION_STATE_IDLE; //Timeout, give up!
			set_mavlink_receiver_delay_time(portMAX_DELAY);
		}
	}
}

/***************************************
 * Waypoint write transaction handlers *
 ***************************************/
static void mission_count_handler(mavlink_message_t *mavlink_message)
{
	if(mission_info.mavlink_state == MISSION_STATE_IDLE || mission_info.mavlink_state == MISSION_STATE_GET_LIST) {
		mission_info.is_busy = true;
		mission_info.mavlink_state = MISSION_STATE_GET_LIST;
		mission_info.received_waypoint_count = 0;
		mission_info.waypoint_count = mavlink_msg_mission_count_get_count(mavlink_message);

		set_mavlink_receiver_delay_time(MILLI_SECOND_TICK * 10);

		mavlink_message_t msg;

		/* Waypoint write request's count is to big, send the ack message and stop the transaction */
		if(mission_info.waypoint_count > WAYPOINT_LIMIT) {
			mission_info.mavlink_state = MISSION_STATE_IDLE;

			send_status_text_message("#Waypoint count is bigger then maximum limit!", MAV_SEVERITY_ERROR);

			mavlink_msg_mission_ack_pack(1, 0, &msg, 255, 0, MAV_MISSION_NO_SPACE);			
			receiver_task_send_package(&msg);
		}

		printf("all:%d\n\r", mission_info.waypoint_count);

		/* Request for first waypoint */
		mavlink_msg_mission_request_pack(1, 0, &msg, 255, 0, 0);
		receiver_task_send_package(&msg);

		//Reset timers
		mission_info.timeout_start_time = get_system_time_ms();
		mission_info.last_retry_time = get_system_time_ms();
	}
}

static void mission_item_handler(mavlink_message_t *mavlink_message)
{
	if(mission_info.mavlink_state == MISSION_STATE_GET_LIST) {
		/* Decode and get the new waypoint */
		mavlink_msg_mission_item_decode(
			mavlink_message,
			&(mission_info.waypoint_list[mission_info.received_waypoint_count].data)
		);

		/* Check the sequence of received waypoint is correct or not  */
		if(mission_info.waypoint_list[mission_info.received_waypoint_count].data.seq !=
			mission_info.received_waypoint_count) 
		{
			return; //Return and wait for the waypoint message again

			//XXX:Send the waypoint request again?
		}

		mission_info.received_waypoint_count++;

		printf("index:%d\n\r", mission_info.received_waypoint_count);

		mavlink_message_t msg;
	
		/* Is this the last waypoint?  */
		if(mission_info.received_waypoint_count == mission_info.waypoint_count) {
			/* Update the wayppoint, navigation manager */
			mission_info.is_busy = false;
			nav_waypoint_list_is_updated = false; /* From navigation point of view */

			/* Send the mission ack message */
			mavlink_msg_mission_ack_pack(1, 0, &msg, 255, 0, MAV_MISSION_ACCEPTED);
			receiver_task_send_package(&msg);

			set_mavlink_receiver_delay_time(portMAX_DELAY);

			mission_info.mavlink_state = MISSION_STATE_IDLE;
			printf("End of the game\n\r");
			return;
		}

		/* Request for next waypoint */
		mavlink_msg_mission_request_pack(1, 0, &msg, 255, 0, mission_info.received_waypoint_count);
		receiver_task_send_package(&msg);

		//Reset timers
		mission_info.timeout_start_time = get_system_time_ms();
		mission_info.last_retry_time = get_system_time_ms();
	}
}

void handle_mission_write_timeout(void)
{
	if(mission_info.mavlink_state == MISSION_STATE_GET_LIST) {
		/* Transaction time check  */
		if((get_system_time_ms() - mission_info.timeout_start_time) <= MISSION_PROTOCOL_TIMEOUT) {
			/* Retry time check */
			if((get_system_time_ms() - mission_info.last_retry_time) >= MISSION_RETRY_TIMEOUT) {
				/* Request for waypoint again */
				mavlink_message_t msg;
				mavlink_msg_mission_request_pack(1, 0, &msg, 255, 0, mission_info.received_waypoint_count);
				receiver_task_send_package(&msg);
				printf("[Retry]Write protocol\n\r");
				mission_info.last_retry_time = get_system_time_ms();
			}
		} else {
			printf("[Timeout]Read protocol\n\r");
			mission_info.mavlink_state = MISSION_STATE_IDLE; //Timeout, give up!
			mission_info.waypoint_count = 0;
		}
	}
}

static void mission_clear_all_handler(__attribute__((__unused__))mavlink_message_t *mavlink_message)
{
	if(mission_info.mavlink_state == MISSION_STATE_IDLE) {
		mission_info.is_busy = true;

		/* Free the waypoint list */
		free_waypoint_list();

		mission_info.is_busy = false;
		nav_waypoint_list_is_updated = false;

		/* Send a mission ack Message at the end */
		mavlink_message_t msg;
		mavlink_msg_mission_ack_pack(1, 0, &msg, 255, 0, 0);
		receiver_task_send_package(&msg);
	}
}

static void mission_set_current_waypoint_handler(mavlink_message_t *mavlink_message)
{
	if(mission_info.mavlink_state == MISSION_STATE_IDLE) {
		mavlink_mission_set_current_t mmst;
		mavlink_msg_mission_set_current_decode(mavlink_message, &mmst);

		/* Clear the old current waypoint flag */
		mission_info.waypoint_list[mission_info.current_waypoint.number].data.current = 0;	

		/* Getting the seq of current waypoint */
		/* Ming change : I think should set this too*/
		mission_info.current_waypoint.number = mmst.seq;
		Nav_update_current_wp_id(mmst.seq);

		/* Set the new waypoint flag */
		mission_info.waypoint_list[mission_info.current_waypoint.number].data.current = 1;

		/* Send back the current waypoint seq as ack message */
		mavlink_message_t msg;
		mavlink_msg_mission_current_pack(1, 0, &msg, mission_info.current_waypoint.number);
		receiver_task_send_package(&msg);
	}
}

static void command_long_handler(mavlink_message_t *mavlink_message)
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
		mission_info.home_waypoint.latitude = mmcl.param5;
		mission_info.home_waypoint.longitude = mmcl.param6;
		mission_info.home_waypoint.altitude = mmcl.param7;
		mission_info.home_waypoint.use_current = (int)mmcl.param1;
		mission_info.home_waypoint.is_set = true;
		break;
	    case MAV_CMD_OVERRIDE_GOTO:
		mission_info.mission_status  = (int)mmcl.param1;
		mission_info.hold_waypoint.latitude = mmcl.param5; 
		mission_info.hold_waypoint.longitude = mmcl.param6;
		mission_info.hold_waypoint.altitude = mmcl.param7;
		mission_info.hold_waypoint.coordinate_frame = (int)mmcl.param3;
		mission_info.hold_waypoint.yaw_angle = mmcl.param4;
		mission_info.hold_waypoint.hold_waypoint = (int)mmcl.param2;
		mission_info.hold_waypoint.is_set = true;
		break;
	    case MAV_CMD_MISSION_START:
		break;
	    case MAV_CMD_COMPONENT_ARM_DISARM:
		break;
	    default:
		break;
	}
}
