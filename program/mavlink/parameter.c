#include "FreeRTOS.h"
#include "task.h"

#include "global.h"
#include "communication.h"
#include "parameter.h"

/* Debug printf */
#define USE_PARAMETER_DEBUG_PRINT 1

#if USE_PARAMETER_DEBUG_PRINT == 1
        #define PARAMETER_DEBUG_PRINT printf
#else
        #define PARAMETER_DEBUG_PRINT(...)
#endif

#define REGISTERED_PARAMETER_MSG_CNT (sizeof(parameter_list) / sizeof(parameter_list[0]))

static void parameter_read_handler(mavlink_message_t *mavlink_message);
static void parameter_read_single_handler(mavlink_message_t *mavlink_message);
static void parameter_write_handler(mavlink_message_t *mavlink_message);

extern mavlink_message_t received_msg;

mavlink_message_t msg;

struct mission_parser_data parameter_list[] = {
        PARAMETER_MSG_DEF(MAVLINK_MSG_ID_PARAM_REQUEST_LIST, parameter_read_handler), //#21
	PARAMETER_MSG_DEF(MAVLINK_MSG_ID_PARAM_VALUE, parameter_read_single_handler), //#20
	PARAMETER_MSG_DEF(MAVLINK_MSG_ID_PARAM_SET, parameter_write_handler) //#23
};

/**
  * @brief  Try to parse and handle the passed message
  * @param  Received mavlink message (pointer) 
  * @retval The message can be handled or not (is this a parameter related message?)
  */
bool parameter_handle_message(mavlink_message_t *mavlink_message)
{
	int i;
	for(i = 0; i < REGISTERED_PARAMETER_MSG_CNT; i++) {
		if(mavlink_message->msgid == parameter_list[i].msgid) {
			parameter_list[i].message_handler(mavlink_message);
			return true;
		}
	}

	return false;
}

static void parameter_read_handler(mavlink_message_t *mavlink_message)
{
}

static void parameter_read_single_handler(mavlink_message_t *mavlink_message)
{
}

static void parameter_write_handler(mavlink_message_t *mavlink_message)
{
}

void parameter_send(void)
{
}

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

	PARAMETER_DEBUG_PRINT("(%d) - parameter single read\n\r", mprr.param_index);

	/* Ground station use index to identify the parameter */
	if(mprr.param_index != -1) {
		//Calculate the index of current parameter in the global data list
		int global_data_index =
			mprr.param_index + get_global_data_count() - get_modifiable_data_count();

		//Error:the index the greater then the global data count!
		if(global_data_index >= GLOBAL_DATA_CNT) {
			//TODO:Should assert the developer at here
			return;
		}

		/* Prepare the data for transmission */
		read_global_data_name(global_data_index, &data_name);
		get_global_data_type(global_data_index, &data_type);
		read_global_data_value(global_data_index, DATA_POINTER_CAST(&data));
	} else {
		bool parameter_found;

		/* Ground station use parameter name to identify the parameter */
		int i;
		for(i = 0; i < get_global_data_count(); i++) {
			//Check the global data is modifiable or not
			get_global_data_parameter_config_status(i, &parameter_config);
			if(parameter_config == false) {
				continue;
			}
	
			/* Get the data name */
			read_global_data_name(i, &data_name);

			/* Compare the name */
			if(strcmp(data_name, mprr.param_id) == 0) {
				/* Found the data which ground station need, prepare the data for transmission */
				get_global_data_type(i, &data_type);
				read_global_data_value(i, DATA_POINTER_CAST(&data));

				parameter_found = true;
				break; //Leave the loop
			}
		}

		/* Error: can't find the same id's parameter in the global data */
		if(parameter_found == false) {
			//TODO:Should assert the developer at here
			return;
		}
	}

	/* Pack the message with correct data type */
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
