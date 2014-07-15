#include "FreeRTOS.h"
#include "task.h"

#include "global.h"
#include "communication.h"
#include "parameter.h"

extern mavlink_message_t received_msg;

mavlink_message_t msg;

void parameter_read_value(void)
{
	bool parameter_config;
	Type data_type;
	Data data;
	char *data_name;

	int i, send_data_cnt = 0;
	for(i = 0; i < get_global_data_count(); i++) {

		/* If the parameter_config status is equal to true, send the data to the ground station */
		get_global_data_parameter_config_status(i, &parameter_config);
		if(parameter_config == true) {

			/* Prepare the data */
			get_global_data_type(i, &data_type);
			read_global_data_value(i, DATA_POINTER_CAST(&data));

			/* Get the data name*/
			read_global_data_name(i, &data_name);

			switch(data_type) {
			    case UINT32:
				mavlink_msg_param_value_pack(
					1, 0, &msg,
					data_name,   		               /* Data name */ 
					data.uint32_value,		       /* Data value */
					data_type,			       /* Data type */
					(uint16_t)get_modifiable_data_count(), /* Data count */
					send_data_cnt			       /* Index */
				);
				break;
			    case INT32:
				mavlink_msg_param_value_pack(
					1, 0, &msg,
					data_name,   		               /* Data name */ 
					data.int32_value,		       /* Data value */
					data_type,			       /* Data type */
					(uint16_t)get_modifiable_data_count(), /* Data count */
					send_data_cnt			       /* Index */
				);
				break;
			    case FLOAT:
				mavlink_msg_param_value_pack(
					1, 0, &msg,
					data_name,   		               /* Data name */ 
					data.float_value,		       /* Data value */
					data_type,		    	       /* Data type */
					(uint16_t)get_modifiable_data_count(), /* Data count */
					send_data_cnt			       /* Index */
				);
				break;
			    default:
				return;
			}
			send_package(&msg);

			uint32_t delay_t =(uint32_t) 100.0/(1000.0 / configTICK_RATE_HZ);
			vTaskDelay(delay_t);

			send_data_cnt++;
		}
	}
}

void parameter_read_single_value(void)
{
	mavlink_param_request_read_t mprr;
	mavlink_msg_param_request_read_decode(&received_msg, &mprr);

	bool parameter_config;
	Type data_type;
	Data data;
	char *data_name;

	int i;
	for(i = 0; i < get_global_data_count(); i++) {
		/* If the parameter_config status is equal to true, send the data to the ground station */
		get_global_data_parameter_config_status(i, &parameter_config);
		if(parameter_config == true)
			continue;

		/* Get the data name */
		read_global_data_name(i, &data_name);

		if(strcmp(data_name, mprr.param_id) == 0) {
			/* Prepare the data */
			get_global_data_type(i, &data_type);
			read_global_data_value(i, DATA_POINTER_CAST(&data));

			switch(data_type) {
			    case UINT32:
				mavlink_msg_param_value_pack(
					1, 0, &msg,
					data_name,   		               /* Data name */ 
					data.uint32_value,		       /* Data value */
					data_type,			       /* Data type */
					(uint16_t)get_modifiable_data_count(), /* Data count */
					mprr.param_index		       /* Index */
				);
				break;
			    case INT32:
				mavlink_msg_param_value_pack(
					1, 0, &msg,
					data_name,   		               /* Data name */ 
					data.int32_value,		       /* Data value */
					data_type,			       /* Data type */
					(uint16_t)get_modifiable_data_count(), /* Data count */
					mprr.param_index		       /* Index */
				);
				break;
			    case FLOAT:
				mavlink_msg_param_value_pack(
					1, 0, &msg,
					data_name,   		               /* Data name */ 
					data.float_value,		       /* Data value */
					data_type,			       /* Data type */
					(uint16_t)get_modifiable_data_count(), /* Data count */
					mprr.param_index		       /* Index */
				);
				break;
			    default:
				return;
			}
			
			/* Send out the data */
			send_package(&msg);

			break;
		}
	}
}

void parameter_write_value(void)
{
	mavlink_param_set_t mps;	
	mavlink_msg_param_set_decode(&received_msg, &mps);

	Type data_type;
	Data data;
	char *data_name;

	int i;
	for(i = 0; i < get_global_data_count(); i++) {
		/* Get the data name */
		read_global_data_name(i, &data_name);

		/* Compare the global data with the parameter id */
		if(strcmp(data_name, mps.param_id) == 0) {
			get_global_data_type(i, &data_type);
			
			/* Update the new value */
			set_global_data_value(i, mps.param_type, DATA_CAST(mps.param_value));

			/* Ack message */
			switch(data_type) {
			    case UINT32:
				data.uint32_value = mps.param_value;				

				mavlink_msg_param_value_pack(
					1, 0, &msg,
					data_name,   		               /* Data name */ 
					data.uint32_value,		       /* Data value */
					data_type,			       /* Data type */
					(uint16_t)get_modifiable_data_count(), /* Data count */
					i				       /* Index */
				);
				break;
			    case INT32:
				data.int32_value = mps.param_value;

				mavlink_msg_param_value_pack(
					1, 0, &msg,
					data_name,   		               /* Data name */ 
					data.int32_value,		       /* Data value */
					data_type,		 	       /* Data type */
					(uint16_t)get_modifiable_data_count(), /* Data count */
					i				       /* Index */
				);
				break;
			    case FLOAT:
				data.float_value = mps.param_value;

				mavlink_msg_param_value_pack(
					1, 0, &msg,
					data_name,   		               /* Data name */ 
					data.float_value,		       /* Data value */
					data_type,			       /* Data type */
					(uint16_t)get_modifiable_data_count(), /* Data count */
					i				       /* Index */
				);
				break;
			    default:
				return;
			}

			send_package(&msg);

			break;
		}
	}
}
