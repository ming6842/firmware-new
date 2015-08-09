#ifndef __MISSION_H
#define __MISSION_H

#include <stdbool.h>
#include "mavlink.h"

/* Waypoint limit */
#define WAYPOINT_LIMIT 150

#define MISSION_MSG_DEF(id, handler) \
	{.name = #id, .message_handler = handler, .msgid = id}

enum {
	MISSION_STATE_IDLE,
	MISSION_STATE_SEND_LIST,
	MISSION_STATE_GET_LIST
} MissionState;

struct mission_parsed_item {
        uint8_t msgid;
        char *name;
        void (*message_handler)(mavlink_message_t *mavlink_message);
};

/* Waypoint status */
typedef enum {
	WAYPOINT_IS_SET,
	WAYPOINT_NOT_SET
} WaypointStatus;

/* Waypoint */
typedef struct waypoint_t waypoint_t;
struct waypoint_t {
	mavlink_mission_item_t data;
};

/* Mission manager */
typedef struct {
	/* Mavlink transaction state */
	int mavlink_state;
	uint32_t timeout_start_time;
	uint32_t last_retry_time;
	int sent_waypoint_count;
	int received_waypoint_count;

	/* Waypoint information */
	int mission_status;
	bool is_busy;

	waypoint_t waypoint_list[WAYPOINT_LIMIT];
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
} mission_info_t;

extern mission_info_t mission_info;

int get_home_waypoint_info(float *latitude, float *longitude, float *altitude,
	int *use_current);
int get_mission_flight_status(void);
int get_hold_waypoint_position(float *latitude, float *longitude, float *altitude,
	int *coordinate_frame, float *yaw_angle, int *hold_waypoint);
int get_current_waypoint_number(void);
void set_current_waypoint_number(int new_waypoint_num);
void set_reached_waypoint_number(int reached_waypoint_num);

waypoint_t *get_waypoint(waypoint_t *wp_list, int index);

bool mission_handle_message(mavlink_message_t *mavlink_message);
int get_mavlink_mission_state(void);
void handle_mission_read_timeout(void);
void handle_mission_write_timeout(void);

#endif
