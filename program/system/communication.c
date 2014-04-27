#include <stddef.h>
#include <stdlib.h>

#include "QuadCopterConfig.h"
#include "mavlink.h"

#define MAV_MAX_LEN 263

extern xTaskHandle ground_station_handle;

mavlink_message_t received_msg;
mavlink_status_t received_status;

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

int get_mavlink_message(uint8_t *buf)
{
	int len;

	/* Check for the start byte */
	while(serial.getc() != 0xFE);

	buf[0] = 0xFE;

	/* Second time receiving the data, get the length */		
	buf[1] = serial.getc(); //Payload of the mavlink
	len = buf[1] + 8; 	//MavLink frame equal len + 8

	/* Start from the third byte */
	int i;
	for(i = 2; i < len - 2; i++) {
		buf[i] = serial.getc();
	}

	return len;
}

void ground_station_send_task()
{
	mavlink_message_t msg;
	uint8_t buf[MAV_MAX_LEN] = {0};
	uint16_t len;

	while(1) {
		/* Test - QuadCopter Heart Beat */
		mavlink_msg_heartbeat_pack(1, 200, &msg,
			MAV_TYPE_QUADROTOR, 
			MAV_AUTOPILOT_GENERIC, 
			MAV_MODE_GUIDED_ARMED, 
			0, MAV_STATE_ACTIVE
		);
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


		/* Test - Attitude */
		mavlink_msg_attitude_pack(1, 200, &msg, 0,
			toRad( system.variable[TRUE_ROLL].value ), 
			toRad( system.variable[TRUE_PITCH].value ), 
			toRad( system.variable[TRUE_YAW].value ), 
			0.0, 0.0, 0.0
		);
		len = mavlink_msg_to_send_buffer(buf, &msg);
		send_package(buf, len);

		/* Test - Ack Message */
		mavlink_msg_command_ack_pack(1, 200, &msg, MAV_CMD_NAV_WAYPOINT, MAV_RESULT_ACCEPTED);
		len = mavlink_msg_to_send_buffer(buf, &msg);
		send_package(buf, len);

		/* Test - Debug Message */
		mavlink_msg_named_value_int_pack(1, 200, &msg, 0, "msg-id", received_msg.msgid);
		len = mavlink_msg_to_send_buffer(buf, &msg);
		send_package(buf, len);
	
	}
}

void ground_station_receive_task()
{
	uint8_t buf[MAV_MAX_LEN];
	int buf_cnt = 0;

	while(1) {
		buf[buf_cnt] = serial.getc();

		mavlink_parse_char(MAVLINK_COMM_0, buf[buf_cnt], &received_msg, &received_status); 
	}
}
