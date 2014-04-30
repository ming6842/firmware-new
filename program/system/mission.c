#include "QuadCopterConfig.h"
#include "mavlink.h"

#define MAV_MAX_LEN 263

struct waypoint_t mission_wp_list = {0};
int waypoint_cnt = 0;
int cur_waypoint = 0;

mavlink_message_t msg;
uint8_t buf[MAV_MAX_LEN];

void push_waypoint_node(struct waypoint_t *waypoint)
{
}

void free_waypoint_list(struct waypoint_t *waypoint)
{
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
