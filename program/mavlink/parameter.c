#include "FreeRTOS.h"
#include "task.h"

#include "delay.h"

#include "global.h"
#include "communication.h"
#include "mission.h"
#include "parameter.h"

#define REGISTERED_PARAMETER_MSG_CNT (sizeof(parameter_message_list) / sizeof(parameter_message_list[0]))

static void parameter_request_list_handler(mavlink_message_t *mavlink_message);
static void parameter_request_read_handler(mavlink_message_t *mavlink_message);
static void parameter_set_handler(mavlink_message_t *mavlink_message);

parameter_info_t parameter_info;

struct parameter_parser_item parameter_message_list[] = {
        PARAMETER_MSG_DEF(MAVLINK_MSG_ID_PARAM_REQUEST_LIST, parameter_request_list_handler), //#21
	PARAMETER_MSG_DEF(MAVLINK_MSG_ID_PARAM_REQUEST_READ, parameter_request_read_handler), //#20
	PARAMETER_MSG_DEF(MAVLINK_MSG_ID_PARAM_SET, parameter_set_handler) //#23
};

/**
  * @brief  Try to parse and handle the passed message
  * @param  Received mavlink message (pointer) 
  * @retval The message can be handled or not (is this a parameter related message?)
  */
bool parameter_handle_message(mavlink_message_t *mavlink_message)
{
	unsigned int i;
	for(i = 0; i < REGISTERED_PARAMETER_MSG_CNT; i++) {
		if(mavlink_message->msgid == parameter_message_list[i].msgid) {
			parameter_message_list[i].message_handler(mavlink_message);
			MAVLINK_DEBUG_PRINT("%s\n\r", parameter_message_list[i].name);
			return true;
		}
	}

	return false;
}

/* @brief: handle mavlink message #21 - PARAM_REQUEST_LIST */
static void parameter_request_list_handler(__attribute__((__unused__))mavlink_message_t *mavlink_message)
{
	if(parameter_info.active_to_send == false) {
		parameter_info.active_to_send = true;
		parameter_info.sent_count = 0;
		parameter_info.send_index = 0;
		set_mavlink_receiver_delay_time(MILLI_SECOND_TICK * 10);
	} else {
		MAVLINK_DEBUG_PRINT("[Received the new parameter request, ignore anyway]\n\r");
	}
}

/* @brief: handle mavlink message #20 - PARAM_REQUSET_READ */
static void parameter_request_read_handler(mavlink_message_t *mavlink_message)
{
	mavlink_param_request_read_t mprr;
	mavlink_msg_param_request_read_decode(mavlink_message, &mprr);

	bool parameter_config;
	Type data_type;
	Data data;
	char *data_name;
	mavlink_message_t msg;

	MAVLINK_DEBUG_PRINT("[Parameter single read]index:%d\n\r", mprr.param_index);

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
	receiver_task_send_package(&msg);
}

/* @brief: handle mavlink message #23 - PARAM_SET */
static void parameter_set_handler(mavlink_message_t *mavlink_message)
{
	mavlink_param_set_t mps;	
	mavlink_msg_param_set_decode(mavlink_message, &mps);

	Type data_type;
	Data data;
	char *data_name;
	mavlink_message_t msg;

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

			receiver_task_send_package(&msg);

			MAVLINK_DEBUG_PRINT("[Parameter write]index:%d\n\r", i);

			break;
		}
	}
}

void parameter_send(void)
{
	if(parameter_info.active_to_send == false) {
		return;
	}

	/* Check the global data is modifiable or not */
	bool parameter_config;
	get_global_data_parameter_config_status(parameter_info.send_index, &parameter_config);
	if(parameter_config == false) {
		parameter_info.send_index++;
		return;
	}

	/* Get data type, value and name */
	Type data_type;
	Data data;
	char *data_name;
	mavlink_message_t msg;

	get_global_data_type(parameter_info.send_index, &data_type);
	read_global_data_value(parameter_info.send_index, DATA_POINTER_CAST(&data));
	read_global_data_name(parameter_info.send_index, &data_name);

	/* Pack the message */
	switch(data_type) {
	    case UINT32:
		mavlink_msg_param_value_pack(
			1, 0, &msg,
			data_name,   		               /* Data name */ 
			data.uint32_value,		       /* Data value */
			data_type,			       /* Data type */
			(uint16_t)get_modifiable_data_count(), /* Data count */
			parameter_info.sent_count              /* Index */
		);
		break;
	    case INT32:
		mavlink_msg_param_value_pack(
			1, 0, &msg,
			data_name,   		               /* Data name */ 
			data.int32_value,		       /* Data value */
			data_type,			       /* Data type */
			(uint16_t)get_modifiable_data_count(), /* Data count */
			parameter_info.sent_count              /* Index */
		);
		break;
	    case FLOAT:
		mavlink_msg_param_value_pack(
			1, 0, &msg,
			data_name,   		               /* Data name */ 
			data.float_value,		       /* Data value */
			data_type,		    	       /* Data type */
			(uint16_t)get_modifiable_data_count(), /* Data count */
			parameter_info.sent_count	       /* Index */
		);
		break;
	    default:
		//XXX:ASSERT
		return;
	}
	receiver_task_send_package(&msg);

	MAVLINK_DEBUG_PRINT("[Parameter read list]index:%d\n\r", parameter_info.sent_count);

	parameter_info.send_index++;
	parameter_info.sent_count++;

	/* Finish sending parameter */
	if(parameter_info.sent_count == get_modifiable_data_count()) {
		parameter_info.active_to_send = false;
		parameter_info.sent_count = 0;
		parameter_info.send_index = 0;

		if(mission_handler_is_busy() == true) {
			//Delay 100ms for mission timeout check
			set_mavlink_receiver_delay_time(MILLI_SECOND_TICK * 100);
		} else {
			//No transaction need to be handled, sleep until receive the data
			set_mavlink_receiver_delay_time(portMAX_DELAY);
		}

		return;
	}
}
