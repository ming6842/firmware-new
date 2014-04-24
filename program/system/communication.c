#include <stddef.h>
#include <stdlib.h>

#include "QuadCopterConfig.h"
#include "mavlink.h"

void generate_package(IMU_package *package, uint8_t *buf)
{
	memcpy(&buf[0], &((package->roll)), sizeof(int16_t));
	memcpy(&buf[2], &((package->pitch)), sizeof(int16_t));
	memcpy(&buf[4], &((package->yaw)), sizeof(int16_t));
	memcpy(&buf[6], &((package->acc_x)), sizeof(int16_t));
	memcpy(&buf[8], &((package->acc_y)), sizeof(int16_t));
	memcpy(&buf[10], &((package->acc_z)), sizeof(int16_t));
	memcpy(&buf[12], &((package->gyro_x)), sizeof(int16_t));
	memcpy(&buf[14], &((package->gyro_y)), sizeof(int16_t));
	memcpy(&buf[16], &((package->gyro_z)), sizeof(int16_t));
}

void send_package(uint8_t *buf, size_t size)
{
	int i;
	for(i = 0; i < size; i++)
		serial.putc(buf[i]);
}

void ground_station_task()
{
	mavlink_message_t msg;
	uint8_t buf[100] = {0};
	uint16_t len;

	while(1) {
		/* Test - QuadCopter Heart Beat */
		mavlink_msg_heartbeat_pack(1, 200, &msg, MAV_TYPE_QUADROTOR, MAV_AUTOPILOT_GENERIC, MAV_MODE_GUIDED_ARMED, 0, MAV_STATE_ACTIVE);
		len = mavlink_msg_to_send_buffer(buf, &msg);
		send_package(buf, len);
		
		/* Test - Position (By GPS) */
		mavlink_msg_global_position_int_pack(1, 220, &msg, /*time*/0,  
			22.999326 * 1E7, 120.219416 * 1E7,
			100*1000, 10 * 1000, 1 * 100, 1 * 100,
			 1 * 100, 45
		);
		len = mavlink_msg_to_send_buffer(buf, &msg);
		send_package(buf, len);
	}
}
