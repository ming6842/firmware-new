#include "QuadCopterConfig.h"
#include "mavlink.h"

#define MAV_MAX_LEN 263

mavlink_message_t msg;
uint8_t buf[MAV_MAX_LEN];

void read_waypoint_list()
{
}

void write_waypoint_list()
{
}

void clean_waypoint()
{
	mavlink_msg_mission_ack_pack(1, 200, &msg, 255, 0, 0);
	send_package(buf, &msg);
}

void set_new_current_waypoint()
{
}
