#ifndef __MISSION_H
#define __MISSION_H

struct waypoint_t {
	float longitude;
	float latitude;
	float altitude;
	struct waypoint_t *next_wp;
};

void read_waypoint_list();
void write_waypoint_list();
void clear_waypoint();
void set_new_current_waypoint();

#endif
