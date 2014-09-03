#ifndef __MISSION_H
#define __MISSION_H

#include <stdbool.h>
#include "mavlink.h"

/* Waypoint status */
typedef enum {
	WAYPOINT_IS_SET,
	WAYPOINT_NOT_SET
} WaypointStatus;

/* Waypoint */
typedef struct waypoint_t waypoint_t;
struct waypoint_t {
	mavlink_mission_item_t data;
	struct waypoint_t *next;
};

/* Mission manager */
typedef struct {
	int mission_status;
	bool is_busy;

	waypoint_t *waypoint_list;
	int waypoint_count;

	struct {
		int number;
		bool is_update;
	} current_waypoint;

	struct {
		int number;
		bool is_update;
	} reached_waypoint;	

	struct {
		float latitude;
		float longitude;
		float altitude;
		int use_current;

		bool is_set;
	} home_waypoint;

	struct {
		float latitude;
		float longitude;
		float altitude;
		int coordinate_frame;
		float yaw_angle;
		int hold_waypoint;

		bool is_set;
	} hold_waypoint;
} waypoint_info_t;

extern waypoint_info_t waypoint_info;

int get_home_waypoint_info(float *latitude, float *longitude, float *altitude,
	int *use_current);
int get_mission_flight_status(void);
int get_hold_waypoint_position(float *latitude, float *longitude, float *altitude,
	int *coordinate_frame, float *yaw_angle, int *hold_waypoint);
int get_current_waypoint_number(void);
void set_current_waypoint_number(int new_waypoint_num);
void set_reached_waypoint_number(int reached_waypoint_num);

waypoint_t *create_waypoint_node(void);
waypoint_t *get_waypoint(waypoint_t *wp_list, int index);
void free_waypoint_list(struct waypoint_t *wp_list);

void mission_read_waypoint_list(void);
void mission_write_waypoint_list(void);
void mission_clear_waypoint(void);
void mission_set_new_current_waypoint(void);
void mission_command(void);

#endif
