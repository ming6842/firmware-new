#include "FreeRTOS.h"
#include "task.h"
#include <stdint.h>
#include "simple_navigation.h"
#include "mission.h"
#define NAVIGATION_TASK_PERIOD_MS 200

struct simple_waypoint {
	int32_t x;
	int32_t y;

};

struct simple_waypoint go_to_wp_list[150];
uint16_t go_to_wp_count = 0;
extern waypoint_info_t waypoint_info;
bool simple_waypoint_availabe = false;
bool simple_waypoint_have_been_updated = true;
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
			vTaskDelay((uint32_t)3000/(1000.0 / configTICK_RATE_HZ));
		}


		vTaskDelayUntil( &xLastWakeTime, xFrequency );
	}

}