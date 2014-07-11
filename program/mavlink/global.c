#include <stddef.h>
#include <stdbool.h>
#include "global.h"
#include "attitude_stabilizer.h"

int modifiable_data_cnt = 0;
global_data_t global_mav_data_list[GLOBAL_DATA_CNT] = {
	/* global data information */
	[VEHICLE_TYPE] = {.name = "vehicle_type"},

	/* Boot time */
	[BOOT_TIME] = {.name = "boot_time"},

	/* IMU information */
	[TRUE_ROLL] = {.name = "imu.roll"},
	[TRUE_PITCH] = {.name = "imu.pitch"},
	[TRUE_YAW] = {.name = "imu.yaw"},

	/* GPS Location */
	[GPS_LAT] = {.name = "gps.latitude", .type = INT32},
	[GPS_LON] = {.name = "gps.longitude", .type = INT32},
	[GPS_ALT] = {.name = "gps.altitude", .type = INT32},

	/* GPS Speed */
	[GPS_VX] = {.name = "gps.vx", .type = INT16},
	[GPS_VY] = {.name = "gps.vy", .type = INT16},
	[GPS_VZ] = {.name = "gps.vz", .type = INT16},

	/* Attitude PID Gain */
	[ROLL_KP] = {.name = "roll.kp", .parameter_config = true},
	[ROLL_KI] = {.name = "roll.ki", .parameter_config = true},
	[ROLL_KD] = {.name = "roll.kd", .parameter_config = true},
	[PITCH_KP] = {.name = "pitch.kp", .parameter_config = true},
	[PITCH_KI] = {.name = "pitch.ki", .parameter_config = true},
	[PITCH_KD] = {.name = "pitch.kd", .parameter_config = true},
	[YAW_KP] = {.name = "yaw.kp", .parameter_config = true},
	[YAW_KI] = {.name = "yaw.ki", .parameter_config = true},
	[YAW_KD] = {.name = "yaw.kd", .parameter_config = true},
};

#define QUADCOPTER 0

void init_global_data(void)
{
	/* Vehicle information */
	set_global_data_value(VEHICLE_TYPE, UINT8, DATA_CAST((uint8_t)QUADCOPTER));

	/* Boot time */
	set_global_data_value(BOOT_TIME, UINT32, DATA_CAST((uint32_t)0));

	/* Attitude PID Gain */
	set_global_data_value(ROLL_KP, FLOAT, DATA_CAST((float)0));
	set_global_data_value(ROLL_KI, FLOAT, DATA_CAST((float)0));
	set_global_data_value(ROLL_KD, FLOAT, DATA_CAST((float)0));
	set_global_data_value(PITCH_KP, FLOAT, DATA_CAST((float)0));
	set_global_data_value(PITCH_KI, FLOAT, DATA_CAST((float)0));
	set_global_data_value(PITCH_KD, FLOAT, DATA_CAST((float)0));
	set_global_data_value(YAW_KP, FLOAT, DATA_CAST((float)0));
	set_global_data_value(YAW_KI, FLOAT, DATA_CAST((float)0));
	set_global_data_value(YAW_KD, FLOAT, DATA_CAST((float)0));

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

/**
  * @brief  get the boot time value
  * @param  None
  * @retval boot time value (int)
  */
uint32_t get_boot_time(void)
{
	uint32_t boot_time;
	read_global_data_value(BOOT_TIME, (Data *)&boot_time);

	return boot_time;
}

