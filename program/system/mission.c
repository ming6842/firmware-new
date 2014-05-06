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

void mission_read_waypoint_list()
{
#if 0
	waypoint_t *cur_wp;
	mavlink_mission_request_t mmrt;

	mavlink_msg_mission_count_pack(
		1, 0, &msg, 255, 0, waypoint_cnt /* Waypoint count */
	);
	send_package(buf, &msg);

	int i;
	for(i = 0; i < waypoint_cnt; i++) {
		/* Waiting for mission request command */
		do {
			mavlink_msg_mission_request_decode(&msg, &mmrt);
		} while(mmrt.seq != i);

		/* Prepare the waypoint for sending */
		cur_wp = get_waypoint(mission_wp_list, i);

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
		
		send_package(buf, &msg);
	}

	/* Clear the received message */
	received_msg.msgid = 0;

	/* Send a mission ack Message at the end */
	mavlink_msg_mission_ack_pack(1, 0, &msg, 255, 0, 0);
	send_package(buf, &msg);
#endif
}

void mission_write_waypoint_list()
{
	/* Clear the old data */
	waypoint_cnt = 0;
	free_waypoint_list(mission_wp_list);

	waypoint_t *cur_wp = mission_wp_list; //First node of the waypoint list
	waypoint_t *new_waypoint;

	/* Getting the waypoint count */
	int count = mavlink_msg_mission_count_get_count(&received_msg);

	int i;
	for(i = 0; i < count; i++) {
		/* Generate the mission_request message */
		mavlink_msg_mission_request_pack(
			1, 0, &msg, 255, 0, i /* waypoint index */
		);

		send_package(buf, &msg);

		/* Create a new node of waypoint */
		new_waypoint = create_waypoint_node();

		/* Get the waypoint message */
		mavlink_msg_mission_item_decode(&received_msg, &(new_waypoint->data));

		/* insert the new waypoint */
		cur_wp->next = new_waypoint;
		cur_wp = cur_wp->next;
		waypoint_cnt++;	
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
	waypoint_cnt = 0;

	/* Clear the received message */
	received_msg.msgid = 0;

	/* Send a mission ack Message at the end */
	mavlink_msg_mission_ack_pack(1, 0, &msg, 255, 0, 0);
	send_package(buf, &msg);
}

void mission_set_new_current_waypoint()
{
}
