#include "FreeRTOS.h"
#include "task.h"

#include "mavlink.h"

#include "mission.h"
#include "gps_simulator.h"

#define CREATE_GPS_DATA(index, latitude, longitude, altitude) \
	[index] =  {.next = 0}, \
	[index].data = {.param1 = 0, .param2 = 5, .param3 = 0, .param4 = 0, \
	.command = 16, .target_system = 1, .target_component = 190, \
	.frame = 0, .current = 0, .autocontinue = 1, \
	.seq = index, .x = latitude, .y = longitude, .z = altitude}

extern waypoint_info_t waypoint_info;

waypoint_t fake_gps_data[] = {
	CREATE_GPS_DATA(0, 22.99543762207031250000, 120.2235260, 30.00),
	CREATE_GPS_DATA(1, 22.99548149108886718750, 120.2235336, 30.00),
	CREATE_GPS_DATA(2, 22.99552345275878906250, 120.2235336, 30.00),
	CREATE_GPS_DATA(3, 22.99552536010742187500, 120.2234879, 30.00),
	CREATE_GPS_DATA(4, 22.99552726745605468750, 120.2234344, 30.00),
	CREATE_GPS_DATA(5, 22.99553489685058593750, 120.2233658, 30.00),
	CREATE_GPS_DATA(6, 22.99554252624511718750, 120.2232819, 30.00),
	CREATE_GPS_DATA(7, 22.99554824829101562500, 120.2232132, 30.00),
	CREATE_GPS_DATA(8, 22.99555969238281250000, 120.2231369, 30.00),
	CREATE_GPS_DATA(9, 22.99556350708007812500, 120.2230682, 30.00),
	CREATE_GPS_DATA(10, 22.99557304382324218750, 120.2229996, 30.00),
	CREATE_GPS_DATA(11, 22.99557304382324218750, 120.2228699, 30.00),
	CREATE_GPS_DATA(12, 22.99557876586914062500, 120.2228165, 30.00),
	CREATE_GPS_DATA(13, 22.99551391601562500000, 120.2228165, 30.00),
	CREATE_GPS_DATA(14, 22.99547958374023437500, 120.2227936, 30.00),
	CREATE_GPS_DATA(15, 22.99547767639160156250, 120.2229156, 30.00),
	CREATE_GPS_DATA(16, 22.99546623229980468750, 120.2230072, 30.00),
	CREATE_GPS_DATA(17, 22.99546623229980468750, 120.2231369, 30.00),
	CREATE_GPS_DATA(18, 22.99546241760253906250, 120.2232285, 30.00),
	CREATE_GPS_DATA(19, 22.99545478820800781250, 120.2232819, 30.00),
	CREATE_GPS_DATA(20, 22.99545097351074218750, 120.2233505, 30.00),
	CREATE_GPS_DATA(21, 22.99543380737304687500, 120.2234192, 30.00),
	CREATE_GPS_DATA(22, 22.99542808532714843750, 120.2234573, 30.00)
};

int fake_gps_data_cnt = 23;

void mission_get_fake_data(void)
{
	waypoint_t *cur_waypoint;

	/* Update the waypoint count */
	waypoint_info.waypoint_count = fake_gps_data_cnt;

	/* Update the waypoint list */
	int i;
	for(i = 0; i < fake_gps_data_cnt; i++) {
		if(i == 0) {
			//First node of the list
			waypoint_info.waypoint_list = cur_waypoint = (fake_gps_data + i);
		} else {
			cur_waypoint->next = (fake_gps_data + i);
			cur_waypoint = cur_waypoint->next;
		}
	}
}

void gps_simulator_task(void)
{
	vTaskSuspend(NULL);
	while(1);
}
