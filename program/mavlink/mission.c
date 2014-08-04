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

#define TIMEOUT_CNT 200

/* Mavlink related variables */
uint8_t buf[MAVLINK_MAX_PAYLOAD_LEN];
extern mavlink_message_t received_msg;
mavlink_message_t msg;

/* Mission manager */
waypoint_info_t waypoint_info;

/* Navigation manger */
extern navigation_info_t navigation_info;


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
	return waypoint_info.current_waypoint;
}

/**
  * @brief  Set the new current waypoint number
  * @param  new waypoint number
  * @retval None
  */
void set_new_current_waypoint(int new_waypoint_num)
{
	waypoint_t *wp;

	/* Clear the old current waypoint flag */
	wp = get_waypoint(waypoint_info.waypoint_list, waypoint_info.current_waypoint);
	wp->data.current = 0;

	/* Getting the seq of current waypoint */
	waypoint_info.current_waypoint = new_waypoint_num;

	/* Set the new waypoint flag */
	wp = get_waypoint(waypoint_info.waypoint_list, waypoint_info.current_waypoint);
	wp->data.current = 1;

	/* Notice the ground station that the vehicle is reached at the 
	   waypoint */
	mavlink_msg_mission_item_reached_pack(1, 0, &msg, new_waypoint_num);
	send_package(&msg);
}

#define MEMORY_DEBUG

#ifdef MEMORY_DEBUG /* Static limit: 200, over -> malloc */

static int memory_cnt = 0;
waypoint_t static_waypoint[200];

waypoint_t *create_waypoint_node(void)
{
	memory_cnt++;

	if(memory_cnt < 200)
		return static_waypoint + memory_cnt;
	else
		return (waypoint_t *)malloc(sizeof(waypoint_t));
} 

#else /* This is the original code! */

waypoint_t *create_waypoint_node(void)
{
	return (waypoint_t *)malloc(sizeof(waypoint_t));
} 

#endif

void free_waypoint_list(struct waypoint_t *wp_list)
{
	if(wp_list == NULL) return;

	waypoint_t *cur_wp = wp_list, *temp;

	while(1) {
		if(cur_wp->next != NULL) {
			temp = cur_wp->next;
			free(cur_wp);
			cur_wp = temp;
		} else {
			free(cur_wp); //End of the list
#ifdef MEMORY_DEBUG
			memory_cnt--;
#endif
			break;
		}
	}
}

waypoint_t *get_waypoint(waypoint_t *wp_list, int index)
{
	if(index == 0) return wp_list;

	waypoint_t *cur_wp = wp_list;

	int i;
	for(i = 0; i < index; i++) {
		if(cur_wp->next != NULL)
			cur_wp = cur_wp->next;
		else
			return NULL; //Index is out of the range
	}

	return cur_wp;
} 

void mission_read_waypoint_list(void)
{
	uint32_t start_time, cur_time;

	waypoint_t *cur_wp = waypoint_info.waypoint_list; //First node of the waypoint list
	mavlink_mission_request_t mmrt;

	mavlink_msg_mission_count_pack(
		1, 0, &msg, 255, 0, waypoint_info.waypoint_count /* Waypoint count */
	);
	send_package(&msg);

	int i;
	for(i = 0; i < waypoint_info.waypoint_count; i++) {
		start_time = get_system_time_ms();

		/* Waiting for mission request command */
		while(received_msg.msgid != 40) {
			cur_time = get_system_time_ms();

			//Suspend the task to read the new message
			vTaskDelay(100);

			/* Time out, leave */
			if((cur_time - start_time) > TIMEOUT_CNT)
				return;
		}

		/* Waiting for mission request command */
		mavlink_msg_mission_request_decode(&received_msg, &mmrt);

		/* Clear the received message */
		clear_message_id(&received_msg);

		/* Send the waypoint to the ground station */
		mavlink_msg_mission_item_pack(
			1, 0, &msg, 255, 0,
			cur_wp->data.seq,
			cur_wp->data.frame,
			cur_wp->data.command,
			cur_wp->data.current,
			cur_wp->data.autocontinue,
			cur_wp->data.param1,
			cur_wp->data.param2,
			cur_wp->data.param3,
			cur_wp->data.param4,
			cur_wp->data.x,
			cur_wp->data.y,
			cur_wp->data.z
		);
		
		send_package(&msg);

		cur_wp = cur_wp->next;
	}


	/* Send a mission ack Message at the end */
	mavlink_msg_mission_ack_pack(1, 0, &msg, 255, 0, 0);
	send_package(&msg);
}

void mission_write_waypoint_list(void)
{
	uint32_t start_time, cur_time;

	waypoint_t *cur_wp = NULL;
	waypoint_t *new_waypoint;

	/* Getting the waypoint count */
	int q_cnt = mavlink_msg_mission_count_get_count(&received_msg);

	waypoint_info.is_busy = true;

	int i;
	for(i = 0; i < q_cnt; i++) {
		/* Generate the mission_request message */
		mavlink_msg_mission_request_pack(
			1, 0, &msg, 255, 0, i /* waypoint index */
		);

		send_package(&msg);

		/* Create a new node of waypoint */
		if (waypoint_info.waypoint_count > i) {
			new_waypoint = get_waypoint(waypoint_info.waypoint_list, i);
		} else { 
			/* Create a new node of waypoint */
			new_waypoint = create_waypoint_node();
		}

		start_time = get_system_time_ms();		

		/* Waiting for new message */
		while(received_msg.msgid != 39) {
			cur_time = get_system_time_ms();

			//Suspend the task to read the new message
			vTaskDelay(100);

			/* Time out, leave */
			if((cur_time - start_time) > TIMEOUT_CNT) {

				return;
			}
		}		

		/* Get the waypoint message */
		mavlink_msg_mission_item_decode(&received_msg, &(new_waypoint->data));

		/* Clear the received message */
		clear_message_id(&received_msg);

		/* insert the new waypoint at the end of the list */
		if(i == 0) {
			//First node of the list
			waypoint_info.waypoint_list = cur_wp = new_waypoint;
		} else {
			cur_wp->next = new_waypoint;
			cur_wp = cur_wp->next;
		}

	}
	/*
	set tail is NULL, set current waypoint length
	*/
	cur_wp->next =NULL;
	waypoint_info.waypoint_count = q_cnt;
	/* Clear the rec
	eived message */
	received_msg.msgid = 0;
	navigation_info.waypoint_status = NOT_HAVE_BEEN_UPDATED;
	waypoint_info.is_busy = false;
	/* Send a mission ack Message at the end */
	mavlink_msg_mission_ack_pack(1, 0, &msg, 255, 0, 0);
	send_package(&msg);
}

void mission_clear_waypoint(void)
{
	waypoint_info.is_busy = true;

	/* Free the waypoint list */
	free_waypoint_list(waypoint_info.waypoint_list);
	waypoint_info.waypoint_count = 0;

	navigation_info.waypoint_status = NOT_HAVE_BEEN_UPDATED;
	waypoint_info.is_busy = false;
	/* Send a mission ack Message at the end */
	mavlink_msg_mission_ack_pack(1, 0, &msg, 255, 0, 0);
	send_package(&msg);
}

void mission_set_new_current_waypoint(void)
{
	mavlink_mission_set_current_t mmst;
	mavlink_msg_mission_set_current_decode(&received_msg, &mmst);

	waypoint_t *wp;

	/* Clear the old current waypoint flag */
	wp = get_waypoint(waypoint_info.waypoint_list, waypoint_info.current_waypoint);
	wp->data.current = 0;

	/* Getting the seq of current waypoint */
	waypoint_info.current_waypoint = mmst.seq;

	/* Set the new waypoint flag */
	wp = get_waypoint(waypoint_info.waypoint_list, waypoint_info.current_waypoint);
	wp->data.current = 1;

	/* Send back the current waypoint seq as ack message */
	mavlink_msg_mission_current_pack(1, 0, &msg, waypoint_info.current_waypoint);
	send_package(&msg);
}

void mission_command(void)
{
	mavlink_command_long_t mmcl;
	mavlink_msg_command_long_decode(&received_msg, &mmcl);

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
