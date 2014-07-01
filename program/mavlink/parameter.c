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
				read_global_data_name(i),   	       /* Data name */ 
				data_type ? data_float : data_int,    /* Data value */
				data_type ? MAV_PARAM_TYPE_REAL32 : MAV_PARAM_TYPE_INT16, /* Data type */
				(uint16_t)get_modifiable_data_count(), /* Data count */
				modifiable_data_cnt		       /* Index */
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
	mavlink_param_request_read_t mprr;
	mavlink_msg_param_request_read_decode(&received_msg, &mprr);

	Type data_type;
	int data_int;
	float data_float;

	int i;
	for(i = 0; i < get_global_data_count(); i++) {
		if(get_global_data_access_right(i) == READ_ONLY)
			continue;

		if(strcmp(read_global_data_name(i), mprr.param_id) == 0) {
			/* Prepare the data */
			data_type = get_global_data_type(i);
			if(data_type == INTEGER)
				data_int = read_global_data_int(i);
			else
				data_float = read_global_data_float(i);
			
			mavlink_msg_param_value_pack(
				1, 0, &msg,
				read_global_data_name(i),   	       /* Data name */ 
				data_type ? data_float : data_int,     /* Data value */
				data_type ? MAV_PARAM_TYPE_REAL32 : MAV_PARAM_TYPE_INT16, /* Data type */
				(uint16_t)get_modifiable_data_count(), /* Data count */
				mprr.param_index 		       /* Index */
			);
			send_package(&msg);

			break;
		}
	}

	/* Clear the received message */
	received_msg.msgid = 0;
}

void parameter_write_value(void)
{
	mavlink_param_set_t mps;	
	mavlink_msg_param_set_decode(&received_msg, &mps);

	Type data_type;
	int data_int;
	float data_float;

	int i;
	for(i = 0; i < get_global_data_count(); i++) {
		/* Compare the global data with the parameter id */
		if(strcmp(read_global_data_name(i), mps.param_id) == 0) {
			data_type = get_global_data_type(i);
			
			/* Update the new value */
			if(get_global_data_type(i) == INTEGER) {
				set_global_data_int(i, mps.param_value);
				data_int = mps.param_value;
			} else {
				set_global_data_float(i, mps.param_value);
				data_float = mps.param_value;
			}

			/* Ack message */
			mavlink_msg_param_value_pack(
				1, 0, &msg,
				read_global_data_name(i),   	       /* Data name */ 
				data_type ? data_float : data_int,     /* Data value */
				data_type ? MAV_PARAM_TYPE_REAL32 : MAV_PARAM_TYPE_INT16, /* Data type */
				get_modifiable_data_count(), /* Data count */
				i		 		       /* Index */
			);
			send_package(&msg);

			break;
		}
	}

	/* Clear the received message */
	received_msg.msgid = 0;
}
