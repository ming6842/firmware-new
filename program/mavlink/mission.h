#ifndef __MISSION_H
#define __MISSION_H

typedef struct waypoint_t waypoint_t;

struct waypoint_t{
	mavlink_mission_item_t data;
	struct waypoint_t *next;
};

int get_current_waypoint_number();

waypoint_t *create_waypoint_node(void);
waypoint_t *get_waypoint(waypoint_t *wp_list, int index);
void free_waypoint_list(struct waypoint_t *wp_list);

void mission_read_waypoint_list(void);
void mission_write_waypoint_list(void);
void mission_clear_waypoint(void);
void mission_set_new_current_waypoint(void);

#endif
