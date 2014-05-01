#ifndef __MISSION_H
#define __MISSION_H

typedef struct {
	float longitude;
	float latitude;
	float altitude;
	struct waypoint_t *next_wp;
} waypoint_t;

void mission_read_waypoint_list();
void mission_write_waypoint_list();
void mission_clear_waypoint();
void mission_set_new_current_waypoint();

#endif
