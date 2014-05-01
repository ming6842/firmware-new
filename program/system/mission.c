#include <stdlib.h>

#include "QuadCopterConfig.h"
#include "mavlink.h"

#define MAV_MAX_LEN 263

waypoint_t mission_wp_list;
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
		if((*cur_wp).next != NULL) {
			cur_wp = (*cur_wp).next;
		} else {
			(*cur_wp).next = new_wp;
			break;
		}
		wp_cnt++;
	}

	wp_cnt++;
	
	return wp_cnt;
}

void free_waypoint_list(struct waypoint_t *wp_list)
{
	waypoint_t *cur_wp = wp_list, *temp;

	while(1) {
		if((*cur_wp).next != NULL) {
			temp = (*cur_wp).next;
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
}

void mission_write_waypoint_list()
{
}

void mission_clear_waypoint()
{
	mavlink_msg_mission_ack_pack(1, 200, &msg, 255, 0, 0);
	send_package(buf, &msg);
}

void mission_set_new_current_waypoint()
{
}
