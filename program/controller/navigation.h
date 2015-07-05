#ifndef FILE_NAVIGATION_H
#define FILE_NAVIGATION_H

#include "bound.h"
#include "system_time.h"
#include "estimator.h"
#include <stdbool.h>

#include "FreeRTOS.h"
#include "task.h"

#define CONTROLLER_DISABLE 0
#define CONTROLLER_ENABLE 1

typedef struct nav_pid_t {
	float kp,ki,kd;
	vector2d_f_t error;
	vector2d_f_t integral;
	vector2d_i32_t setpoint;
	float output_roll,output_pitch;
	float out_max, out_min;
	float integral_max;
	uint8_t controller_status;

} nav_pid_t;

typedef struct lla_pos_t
{
	int32_t lat;
	int32_t lon;
	float alt;

}lla_pos_t;

typedef enum {

	WAYPOINT_MODE_ABS=0,
	WAYPOINT_MODE_RELATIVE,
	WAYPOINT_MODE_LOITER_TIME

} waypoint_type_i;

typedef struct waypoint_navigation_t
{
	lla_pos_t position;
	uint8_t autocontinue;
	uint8_t waypoint_state;
	float tol_radius;
	float loiter_time;
	uint8_t data_available;
}waypoint_navigation_t;

typedef enum {

	WAYPOINT_STATUS_PENDING=0,
	WAYPOINT_STATUS_ACTIVE,
	WAYPOINT_STATUS_LOITERING,
	WAYPOINT_STATUS_DONE

 }waypoint_status;


#define WAYPOINT_MAX_SIZE 250

typedef enum {

	NAVIGATION_MODE_HOLD_POINT=0,
	NAVIGATION_MODE_GO_HOME,
	NAVIGATION_MODE_GO_SPECIFIED_POS,
	NAVIGATION_MODE_WAYPOINT

} navigation_mode_status;

typedef enum {

	ACCESS_BUSY=0,
	ACCESS_CLEAR

} busy_flag_status;

enum WAY_POINTS_STATUS {
	HAVE_BEEN_UPDATED = 0,
	NOT_HAVE_BEEN_UPDATED
};
typedef struct navigation_info_t
{
	waypoint_navigation_t wp_info[WAYPOINT_MAX_SIZE];
	lla_pos_t home_wp;
	lla_pos_t instant_wp;
	lla_pos_t current_pos;
	lla_pos_t target_pos;
	lla_pos_t hold_wp;
	float max_dist_from_home;
	float current_distance_to_target;
	uint8_t current_wp_id;
	uint8_t navigation_mode;
	uint8_t hold_point_flag;
	uint8_t busy_flag;
	uint8_t waypoint_status;
	bool target_pos_updated_flag;
}navigation_info_t;



#include "lea6h_ubx.h"// Should link to higher level like GPS.h but so far we have one GPS receiver only.
#include "vertical_stabilizer.h"
void PID_Nav(nav_pid_t *,attitude_t *,UBXvelned_t *, UBXposLLH_t *);
float get_elasped_time(uint32_t ,float );
void update_current_state(void);
void navigation_task(void);
void pass_navigation_setpoint(nav_pid_t *,vertical_pid_t *);
void Nav_update_current_wp_id(uint32_t new_wp_id);

float calc_distance_two_wp(int32_t ,int32_t , int32_t , int32_t );

#include "flight_controller.h"
#endif