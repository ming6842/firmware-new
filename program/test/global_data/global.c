#include <stddef.h>
#include <stdbool.h>
#include "global.h"

int modifiable_data_cnt = 0;
global_data_t global_mav_data_list[GLOBAL_DATA_CNT] = {
	/* Test all the different type data */
	[TEST_UINT8] = {.name = "test_uint8", .type = UINT8},
	[TEST_INT8] = {.name = "test_int8", .type = INT8},
	[TEST_UINT16] = {.name = "test_uint16", .type = UINT16},
	[TEST_INT16] = {.name = "test_int16", .type = INT16},
	[TEST_UINT32] = {.name = "test_uint32", .type = UINT32},
	[TEST_INT32] = {.name = "test_int32", .type = INT32},
	[TEST_FLOAT] = {.name = "test_float", .type = FLOAT},

	/* Test the parameter config flag */
	[TEST_PARAMETER_CONFIG] = {.name = "test_parameter_config",
		.type = UINT8, .parameter_config = true},
};

void init_global_data(void)
{
	/* Initial the data value */
	set_global_data_value(TEST_UINT8,  UINT8, DATA_CAST((uint8_t)+8));
	set_global_data_value(TEST_INT8, INT8, DATA_CAST((int8_t)-8));
	set_global_data_value(TEST_UINT16, UINT16, DATA_CAST((uint16_t)+16));
	set_global_data_value(TEST_INT16, INT16, DATA_CAST((int16_t)-16));
	set_global_data_value(TEST_UINT32, UINT32, DATA_CAST((uint32_t)+32));
	set_global_data_value(TEST_INT32, INT32, DATA_CAST((int32_t)-32));
	set_global_data_value(TEST_FLOAT, FLOAT, DATA_CAST((float)666.666));
	set_global_data_value(TEST_PARAMETER_CONFIG, UINT8, DATA_CAST((uint8_t)8));

	int i;
	for(i = 0; i < get_global_data_count(); i++) {
		bool parameter_config;

		get_global_data_parameter_config_status(i, &parameter_config);
		if(parameter_config == true)
			modifiable_data_cnt++;
	}
} 

/**
  * @brief  get the count of global data
  * @param  None
  * @retval global data count (int)
  */
int get_global_data_count(void)
{
	return GLOBAL_DATA_CNT;
}

/**
  * @brief  get the count of modifiable global data
  * @param  None
  * @retval modifiable global data count (int)
  */
int get_modifiable_data_count(void)
{
	return modifiable_data_cnt;
}

/**
  * @brief  set global data's value
  * @param  index (int), Type type, value (Data)
  * @retval Operated result (0 - succeeded, 1 - error)
  */
int set_global_data_value(int index, Type type, Data value)
{
	/* Index is in the range or not */
	if((index < 0) || (index >= GLOBAL_DATA_CNT))
		return GLOBAL_ERROR_INDEX_OUT_RANGE;

	/* Set the variable type and value */
	global_mav_data_list[index].type = type;

	switch(type) {
	    case UINT8:
		global_mav_data_list[index].data.uint8_value = 
			value.uint8_value;;
		break;
	    case INT8:
		global_mav_data_list[index].data.int8_value = 
			value.int8_value;
		break;
	    case UINT16:
		global_mav_data_list[index].data.uint16_value = 
			value.uint16_value;
		break;
	    case INT16:
		global_mav_data_list[index].data.int16_value = 
			value.int16_value;
		break;
	    case UINT32:
		global_mav_data_list[index].data.uint32_value = 
			value.uint32_value;
		break;
	    case INT32:
		global_mav_data_list[index].data.int32_value = 
			value.int32_value;
		break;
	    case FLOAT:
		global_mav_data_list[index].data.float_value = 
			value.float_value;
		break;
	}

	return GLOBAL_SUCCESS;
}

/**
  * @brief  get the variable type of global data
  * @param  index (int), variable type (Type* to get the type)
  * @retval Operated result (0 - succeeded, 1 - error)
  */
int get_global_data_type(int index, Type *type)
{
	/* Index is in the range or not */
	if((index < 0) || (index >= GLOBAL_DATA_CNT))
		return GLOBAL_ERROR_INDEX_OUT_RANGE;

	*type = global_mav_data_list[index].type;

	return GLOBAL_SUCCESS;
}

/**
  * @brief  get the access right of global data
  * @param  index (int), access right (AccessRight* to get the type)
  * @retval Operated result (0 - succeeded, 1 - error)
  */
int get_global_data_parameter_config_status(int index, bool *parameter_config)
{
	/* Index is in the range or not */
	if((index < 0) || (index >= GLOBAL_DATA_CNT))
		return GLOBAL_ERROR_INDEX_OUT_RANGE;

	*parameter_config = global_mav_data_list[index].parameter_config;

	return GLOBAL_SUCCESS;
}

/**
  * @brief  get the name of global data
  * @param  index (int), name (char* to get the name)
  * @retval Operated result (0 - succeeded, 1 - error)
  */
int read_global_data_name(int index, char **name)
{
        /* Index is in the range or not */
	if((index < 0) || (index >= GLOBAL_DATA_CNT))
		return GLOBAL_ERROR_INDEX_OUT_RANGE;
	
	*name = global_mav_data_list[index].name;

	return GLOBAL_SUCCESS;
}

/**
  * @brief  get the value of global data
  * @param  index (int), value (Data* to get the result value)
  * @retval Operated result (0 - succeeded, 1 - error)
  */
int read_global_data_value(int index, Data *value)
{
        /* Index is in the range or not */
	if((index < 0) || (index >= GLOBAL_DATA_CNT))
		return GLOBAL_ERROR_INDEX_OUT_RANGE;

	switch(global_mav_data_list[index].type) {
	    case UINT8:
		value->uint8_value = 
			global_mav_data_list[index].data.uint8_value;
		break;
	    case INT8:
		value->int8_value =
			global_mav_data_list[index].data.int8_value;
		break;
	    case UINT16:
		value->uint16_value =
			global_mav_data_list[index].data.uint16_value;
		break;
	    case INT16:
		value->int16_value = 
			global_mav_data_list[index].data.int16_value;
		break;
	    case UINT32:
		value->uint32_value =
			global_mav_data_list[index].data.uint32_value;
		break;
	    case INT32:
		value->int32_value =
			global_mav_data_list[index].data.int32_value;
		break;
	    case FLOAT:
		value->float_value =
			global_mav_data_list[index].data.float_value;
		break;
	}

	return GLOBAL_SUCCESS;
}
