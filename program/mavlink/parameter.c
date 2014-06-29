#include "global.h"
#include "communication.h"
#include "parameter.h"

extern mavlink_message_t received_msg;

mavlink_message_t msg;

void parameter_read_value(void)
{
	Type data_type;
	int data_int;
	float data_float;

	int i, modifiable_data_cnt = 0;
	for(i = 0; i < get_global_data_count(); i++) {
		/* Prepare the data */
		data_type = get_global_data_type(i);
		if(data_type == INTEGER)
			data_int = read_global_data_int(i);
		else
			data_float = read_global_data_float(i);
		
		/* If the access right equal to READ_WRITE, send the data to the ground station */
		if(get_global_data_access_right(i) == READ_WRITE) {
			mavlink_msg_param_value_pack(
				1, 0, &msg,
				read_global_data_name(i),   	     /* Data name */ 
				data_type ?  data_int : data_float,  /* Data value */
				data_type ? MAV_PARAM_TYPE_INT16 : MAV_PARAM_TYPE_REAL32, /* Data type */
				(uint16_t)get_modifiable_data_count(), /* Data count */
				modifiable_data_cnt		     /* Index */
			);
			send_package(&msg);

			modifiable_data_cnt++;
		}
	}

	/* Clear the received message */
	received_msg.msgid = 0;
}

void parameter_read_single_value(void)
{
}

void parameter_write_value(void)
{
}
