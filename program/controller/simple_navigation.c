#include <stdbool.h>
#include "FreeRTOS.h"
#include "task.h"
#include <stdint.h>
#include "estimator.h"
#include "simple_navigation.h"
#include "mission.h"
#include <stdio.h>

#define NAVIGATION_TASK_PERIOD_MS 200

struct simple_waypoint {
	int32_t x;
	int32_t y;

};
#define WAYPOINT_DEBUG printf
struct simple_waypoint go_to_wp_list[150];
uint16_t go_to_wp_count = 0;
uint16_t simple_nav_current_waypoint = 0;
extern waypoint_info_t waypoint_info;
bool simple_waypoint_availabe = false;
bool simple_waypoint_have_been_updated = true;
bool simple_nav_go_to_here = false;
bool got_set_current_command = false;
bool get_position_target(vector2d_i32_t * pos)
{

	if (simple_nav_go_to_here == true) {
		pos->x = go_to_wp_list[simple_nav_current_waypoint].x;
		pos->y = go_to_wp_list[simple_nav_current_waypoint].y;
		return true;
	}
	else {
		return false;
	}
}
void simple_nav_task()
{

	portTickType xLastWakeTime;
	const portTickType xFrequency = (uint32_t)NAVIGATION_TASK_PERIOD_MS/(1000.0 / configTICK_RATE_HZ);
	// Initialise the xLastWakeTime variable with the current time.
    	xLastWakeTime = xTaskGetTickCount();
	while(1) {

		/*copying waypoints if possible*/
		if ( (simple_waypoint_have_been_updated == false) && (waypoint_info.is_busy == false))
		{
			WAYPOINT_DEBUG("start copying list\r\n");
			/*lock the resources*/
			waypoint_info.is_busy = true;
			/*copying*/
			int i;
			waypoint_t* wp_ptr;
			for ( i=0; i < waypoint_info.waypoint_count; i++){

				wp_ptr = get_waypoint(waypoint_info.waypoint_list, i);
				go_to_wp_list[i].x = (int32_t)(wp_ptr->data.x * 1E7f);
				go_to_wp_list[i].y= (int32_t)(wp_ptr->data.y * 1E7f);

			}
			go_to_wp_count = waypoint_info.waypoint_count;
			simple_waypoint_have_been_updated = true;
			waypoint_info.is_busy = false;
			WAYPOINT_DEBUG("finish copying list\r\n");
			WAYPOINT_DEBUG("simple nav: %d point in the list\r\n",go_to_wp_count);
			vTaskDelay((uint32_t)3000/(1000.0 / configTICK_RATE_HZ));
		}
		/*get set current command*/
		if( (got_set_current_command == true)&&
			(waypoint_info.current_waypoint.number <= waypoint_info.waypoint_count)) {
			
			WAYPOINT_DEBUG("excute set current to navigation..\r\n");
			
			simple_nav_current_waypoint = waypoint_info.current_waypoint.number;
			simple_nav_go_to_here = true;
			WAYPOINT_DEBUG("go to %d waypoints now \r\n",simple_nav_current_waypoint);
			waypoint_info.current_waypoint.is_update = false;
			got_set_current_command = false;
			vTaskDelay((uint32_t)1000/(1000.0 / configTICK_RATE_HZ));
		} else {

			simple_nav_go_to_here = false;
			WAYPOINT_DEBUG("waiting command... \r\n");
		

		}


		vTaskDelayUntil( &xLastWakeTime, xFrequency );
	}

}