#include <stdlib.h>

#include "global.h"
#include "communication.h"
#include "mission.h"

#define TIMEOUT_CNT 30

/* Mavlink related variables */
uint8_t buf[MAVLINK_MAX_PAYLOAD_LEN];
mavlink_message_t msg;
extern mavlink_message_t received_msg;

static int memory_cnt = 0;

/* Waypoint related variables */
waypoint_t *mission_wp_list = NULL;
int waypoint_cnt = 0;
int cur_waypoint = 0;

/* Mission command waypoint */
mavlink_command_long_t mission_command_wp;

/**
  * @brief  Get the mission flight status 
  * @param  None
  * @retval Status (int, MAV_GOTO_DO_HOLD and MAV_GOTO_DO_CONTINUE)
  */
int get_mission_flight_status(void)
{
	return (int)mission_command_wp.param1;
}

/**
  * @brief  Get the mission hold waypoint information 
  * @param  latitude, longitude, altitude, yaw angle (float* to get the result value)
	    coordinate frame type (int* to get the result value)
  * @retval Waypoint type (int, MAV_GOTO_HOLD_AT_CURRENT_POSITION and
  *	    MAV_GOTO_HOLD_AT_SPECIFIED_POSITION)
  */
int get_hold_waypoint_position(float *latitude, float *longitude, float *altitude,
	int *coordinate_frame, float *yaw_angle)
{
	/* Position */
	*latitude = mission_command_wp.param5; 
	*longitude = mission_command_wp.param6;
	*altitude = mission_command_wp.param7;

	/* Coordinate type */
	*coordinate_frame = (int)mission_command_wp.param3;

	/* Yaw angle*/
	*yaw_angle = mission_command_wp.param4;

	/* Waypoint type */
	return (int)mission_command_wp.param2;
}

/**
  * @brief  Get the current waypoint number
  * @param  None
  * @retval current waypoint number (int)
  */
int get_current_waypoint_number(void)
{
	return cur_waypoint;
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
	wp = get_waypoint(mission_wp_list, cur_waypoint);
	wp->data.current = 0;

	/* Getting the seq of current waypoint */
	cur_waypoint = new_waypoint_num;

	/* Set the new waypoint flag */
	wp = get_waypoint(mission_wp_list, cur_waypoint);
	wp->data.current = 1;
}

waypoint_t *create_waypoint_node(void)
{
	memory_cnt++;
	return (waypoint_t *)malloc(sizeof(waypoint_t));
} 

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

	waypoint_t *cur_wp = mission_wp_list; //First node of the waypoint list
	mavlink_mission_request_t mmrt;

	mavlink_msg_mission_count_pack(
		1, 0, &msg, 255, 0, waypoint_cnt /* Waypoint count */
	);
	send_package(&msg);

	int i;
	for(i = 0; i < waypoint_cnt; i++) {
		start_time = get_boot_time();

		/* Waiting for mission request command */
		while(received_msg.msgid != 40) {
			cur_time = get_boot_time();

			//Suspend the task to read the new message
			vTaskDelay(1);

			/* Time out, leave */
			if((cur_time - start_time) == TIMEOUT_CNT)
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

	int i;
	for(i = 0; i < q_cnt; i++) {
		/* Generate the mission_request message */
		mavlink_msg_mission_request_pack(
			1, 0, &msg, 255, 0, i /* waypoint index */
		);

		send_package(&msg);

		/* Create a new node of waypoint */
		if (  waypoint_cnt > i) {

			new_waypoint = get_waypoint(mission_wp_list, i);


		} else { 

			/* Create a new node of waypoint */
			new_waypoint = create_waypoint_node();
		}

		start_time = get_boot_time();		

		/* Waiting for new message */
		while(received_msg.msgid != 39) {
			cur_time = get_boot_time();

			//Suspend the task to read the new message
			vTaskDelay(1);

			/* Time out, leave */
			if((cur_time - start_time) == TIMEOUT_CNT) {

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
			mission_wp_list = cur_wp = new_waypoint;
		} else {
			cur_wp->next = new_waypoint;
			cur_wp = cur_wp->next;
		}

	}
	/*
	set tail is NULL, set current waypoint length
	*/
	cur_wp->next =NULL;
	waypoint_cnt = q_cnt;
	/* Clear the received message */
	received_msg.msgid = 0;

	/* Send a mission ack Message at the end */
	mavlink_msg_mission_ack_pack(1, 0, &msg, 255, 0, 0);
	send_package(&msg);
}

void mission_clear_waypoint(void)
{
	/* Free the waypoint list */
	free_waypoint_list(mission_wp_list);
	waypoint_cnt = 0;

	/* Send a mission ack Message at the end */
	mavlink_msg_mission_ack_pack(1, 0, &msg, 255, 0, 0);
	send_package(&msg);
}

void mission_set_new_current_waypoint(void)
{
	waypoint_t *wp;

	mavlink_mission_set_current_t mmst;
	mavlink_msg_mission_set_current_decode(&received_msg, &mmst);

	set_new_current_waypoint(mmst.seq);

	/* Send back the current waypoint seq as ack message */
	mavlink_msg_mission_current_pack(1, 0, &msg, cur_waypoint);
	send_package(&msg);
}

void mission_command(void)
{
	mavlink_msg_command_long_decode(&received_msg, &mission_command_wp);
}
