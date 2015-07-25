#include <stddef.h>
#include <stdbool.h>
#include "global.h"
#include "attitude_stabilizer.h"

#define QUADCOPTER 0
int modifiable_data_cnt = 0;
global_data_t global_mav_data_list[GLOBAL_DATA_CNT] = {
	/* global data information */
	[VEHICLE_TYPE] = {.name = "vehicle_type", .type = UINT8,
		.data.uint8_value = QUADCOPTER},

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

	/* Remote controller */
	[SAFTY_BUTTON] = {.name = "safty-button", .type = UINT8},
	[MODE_BUTTON] = {.name = "flight-mode-button", .type = UINT8},

	/* Attitude PID gain */
	[ROLL_KP] = {.name = "Roll.kp", .type = FLOAT, .parameter_config = true},
	[ROLL_KI] = {.name = "Roll.ki", .type = FLOAT, .parameter_config = true},
	[ROLL_KD] = {.name = "Roll.kd", .type = FLOAT, .parameter_config = true},
	[PITCH_KP] = {.name = "Pitch.kp", .type = FLOAT, .parameter_config = true},
	[PITCH_KI] = {.name = "Pitch.ki", .type = FLOAT, .parameter_config = true},
	[PITCH_KD] = {.name = "Pitch.kd", .type = FLOAT, .parameter_config = true},
	[YAW_KP] = {.name = "Yaw.kp", .type = FLOAT, .parameter_config = true},
	[YAW_KI] = {.name = "Yaw.ki", .type = FLOAT, .parameter_config = true},
	[YAW_KD] = {.name = "Yaw.kd", .type = FLOAT, .parameter_config = true},
	[HEADING_KP] = {.name = "Heading.kp", .type = FLOAT, .parameter_config = true},
	[HEADING_KI] = {.name = "Heading.ki", .type = FLOAT, .parameter_config = true},
	[HEADING_KD] = {.name = "Heading.kd", .type = FLOAT, .parameter_config = true},

	/* Height PID gain */
	[Z_KP] = {.name = "Z.kp", .type = FLOAT, .parameter_config = true},
	[Z_KI] = {.name = "Z.ki", .type = FLOAT, .parameter_config = true},
	[Z_KD] = {.name = "Z.kd", .type = FLOAT, .parameter_config = true},
	[ZD_KP] = {.name = "Zd.kp", .type = FLOAT, .parameter_config = true},
	[ZD_KI] = {.name = "Zd.ki", .type = FLOAT, .parameter_config = true},
	[ZD_KD] = {.name = "Zd.kd", .type = FLOAT, .parameter_config = true},

	/* Navigation PID gain */
	[NAV_KP] = {.name = "Navigation.kp", .type = FLOAT, .parameter_config = true},
	[NAV_KI] = {.name = "Navigation.ki", .type = FLOAT, .parameter_config = true},
	[NAV_KD] = {.name = "Navigation.kd", .type = FLOAT, .parameter_config = true},

	/* Sensor calibration */
	[ACCEL_X_MAX] = {.name = "accel.max-x", .type = FLOAT, .parameter_config = true},
	[ACCEL_X_MIN] = {.name = "accel.min-x", .type = FLOAT, .parameter_config = true},
	[ACCEL_Y_MAX] = {.name = "accel.max-y", .type = FLOAT, .parameter_config = true},
	[ACCEL_Y_MIN] = {.name = "accel.min-y", .type = FLOAT, .parameter_config = true},
	[ACCEL_Z_MAX] = {.name = "accel.max-z", .type = FLOAT, .parameter_config = true},
	[ACCEL_Z_MIN] = {.name = "accel.min-z", .type = FLOAT, .parameter_config = true},
	[MAG_X_MAX] = {.name = "mag.max-x", .type = FLOAT, .parameter_config = true},
	[MAG_X_MIN] = {.name = "mag.min-x", .type = FLOAT, .parameter_config = true},
	[MAG_Y_MAX] = {.name = "mag.max-y", .type = FLOAT, .parameter_config = true},
	[MAG_Y_MIN] = {.name = "mag.min-y", .type = FLOAT, .parameter_config = true},
	[MAG_Z_MAX] = {.name = "mag.max-z", .type = FLOAT, .parameter_config = true},
	[MAG_Z_MIN] = {.name = "mag.min-z", .type = FLOAT, .parameter_config = true}
};



void init_global_data(void)
{
	/* Vehicle information */
	set_global_data_value(VEHICLE_TYPE, UINT8, DATA_CAST((uint8_t)QUADCOPTER));

	/* Flight status */

	
	set_global_data_value(SAFTY_BUTTON, UINT8, DATA_CAST((uint8_t)QUADCOPTER));
	set_global_data_value(MODE_BUTTON, UINT8, DATA_CAST((uint8_t)QUADCOPTER));

	/* Attitude PID Gain */
	set_global_data_value(ROLL_KP, FLOAT, DATA_CAST((float)0.20f));
	set_global_data_value(ROLL_KI, FLOAT, DATA_CAST((float)0.045f));
	set_global_data_value(ROLL_KD, FLOAT, DATA_CAST((float)0.07f));

	set_global_data_value(PITCH_KP, FLOAT, DATA_CAST((float)0.20f));
	set_global_data_value(PITCH_KI, FLOAT, DATA_CAST((float)0.045));
	set_global_data_value(PITCH_KD, FLOAT, DATA_CAST((float)0.07f));

	set_global_data_value(YAW_KP, FLOAT, DATA_CAST((float)0.65));
	set_global_data_value(YAW_KI, FLOAT, DATA_CAST((float)0));
	set_global_data_value(YAW_KD, FLOAT, DATA_CAST((float)0));

	set_global_data_value(YAW_KP, FLOAT, DATA_CAST((float)0.65));
	set_global_data_value(YAW_KI, FLOAT, DATA_CAST((float)0));
	set_global_data_value(YAW_KD, FLOAT, DATA_CAST((float)0));

	set_global_data_value(HEADING_KP, FLOAT, DATA_CAST((float)2.5));
	set_global_data_value(HEADING_KI, FLOAT, DATA_CAST((float)0));
	set_global_data_value(HEADING_KD, FLOAT, DATA_CAST((float)0));

	set_global_data_value(ZD_KP, FLOAT, DATA_CAST((float)0.3));
	set_global_data_value(ZD_KI, FLOAT, DATA_CAST((float)0.03));
	set_global_data_value(ZD_KD, FLOAT, DATA_CAST((float)0));

	set_global_data_value(Z_KP, FLOAT, DATA_CAST((float)1.4));
	set_global_data_value(Z_KI, FLOAT, DATA_CAST((float)0));
	set_global_data_value(Z_KD, FLOAT, DATA_CAST((float)0));


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

	global_mav_data_list[index].updated_flag = true;

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
  * @brief  Set updated_flag
  * @param  index (int),
  * @retval None
  */
void set_global_data_update_flag(int index){


	global_mav_data_list[index].updated_flag = true;

}


/**
  * @brief  Unset updated_flag
  * @param  index (int),
  * @retval None
  */
void reset_global_data_update_flag(int index){


	global_mav_data_list[index].updated_flag = false;

}

/**
  * @brief  Check global data updated_flag
  * @param  index (int),
  * @retval Updated_flag status
  */
bool check_global_data_update_flag(int index){


	return global_mav_data_list[index].updated_flag;

}