#include <stdlib.h>

#include "QuadCopterConfig.h"
#include "mavlink.h"

#define MAV_MAX_LEN 263

extern mavlink_message_t received_msg;

waypoint_t *mission_wp_list = NULL;
int waypoint_cnt = 0;
int cur_waypoint = 0;

mavlink_message_t msg;
uint8_t buf[MAV_MAX_LEN];

waypoint_t *create_waypoint_node()
{
	return (waypoint_t *)malloc(sizeof(waypoint_t));
} 

int push_waypoint_node(waypoint_t *wp_list, waypoint_t *new_wp)
{
	waypoint_t *cur_wp = wp_list;
	int wp_cnt = 0;

	while(1) {
		if(cur_wp->next != NULL) {
			cur_wp = cur_wp->next;
		} else {
			cur_wp->next = new_wp;
			break;
		}
		wp_cnt++;
	}

	wp_cnt++;
	
	return wp_cnt;
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
			return NULL; //Index if out of the range
	}

	return cur_wp;
}

void mission_read_waypoint_list()
{
}

void mission_write_waypoint_list()
{
	waypoint_t *new_waypoint = create_waypoint_node();

	/* Getting the waypoint count */
	int waypoint_cnt = mavlink_msg_mission_count_get_count(&received_msg);

	int i;
	for(i = 0; i < waypoint_cnt; i++) {
		/* Generate the mission_request message */
		mavlink_msg_mission_request_pack(
			1, 0, &msg, 255, 0, i /* waypoint index */
		);

		/* Send out for three times to make sure the ground control station
		 * received the message
		 */
		int j;
		for(j = 0; j < 3; j++)
			send_package(buf, &msg);

		/* Decode the mission_item message */
		mavlink_msg_mission_item_decode(&msg, &(new_waypoint->data));

		push_waypoint_node(mission_wp_list, new_waypoint);
	}

	/* Clear the received message */
	received_msg.msgid = 0;

	/* Send a mission ack Message at the end */
	mavlink_msg_mission_ack_pack(1, 0, &msg, 255, 0, 0);
	send_package(buf, &msg);
}

void mission_clear_waypoint()
{
	/* Free the waypoint list */
	free_waypoint_list(mission_wp_list);

	/* Clear the received message */
	received_msg.msgid = 0;

	/* Send a mission ack Message at the end */
	mavlink_msg_mission_ack_pack(1, 0, &msg, 255, 0, 0);
	send_package(buf, &msg);
}

void mission_set_new_current_waypoint()
{
}
