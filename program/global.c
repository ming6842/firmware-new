#include <stddef.h>
#include <stdbool.h>
#include "global.h"
#include "attitude_stabilizer.h"

global_data_t global_mav_data_list[GLOBAL_DATA_CNT];
int modifiable_data_cnt = 0;

#define QUADCOPTER 0

extern attitude_stablizer_pid_t pid_roll_info;
extern attitude_stablizer_pid_t pid_pitch_info;
extern attitude_stablizer_pid_t pid_yaw_info;

void init_global_data(void)
{
	/* global data information */
	reset_global_data(VEHICLE_TYPE, "vehicle_type", READ_ONLY);
	set_global_data_int(VEHICLE_TYPE, QUADCOPTER);

	/* Boot time */
	reset_global_data(BOOT_TIME, "boot_time", READ_ONLY);
	set_global_data_int(BOOT_TIME, 0);

	/* IMU information */
	reset_global_data(TRUE_ROLL, "imu.roll", READ_ONLY);
	reset_global_data(TRUE_PITCH, "imu.pitch", READ_ONLY);
	reset_global_data(TRUE_YAW, "imu.yaw", READ_ONLY);

        /* GPS Location */
	reset_global_data(GPS_LAT, "gps.latitude", READ_ONLY);
	reset_global_data(GPS_LON, "gps.longitude", READ_ONLY);
	reset_global_data(GPS_ALT, "gps.altitude", READ_ONLY);

	/* GPS Speed */
	reset_global_data(GPS_VX, "gps.vx", READ_ONLY);
	reset_global_data(GPS_VY, "gps.vy", READ_ONLY);
	reset_global_data(GPS_VZ, "gps.vz", READ_ONLY);
#if 0
	/* PID Controller */
	reset_global_data(ROLL_KP, "roll.kp", READ_WRITE);
	reset_global_data(ROLL_KI, "roll.ki", READ_WRITE);
	reset_global_data(ROLL_KD, "roll.kd", READ_WRITE);
	add_update_target_float(ROLL_KP, &(pid_roll_info.kp));
	add_update_target_float(ROLL_KI, &(pid_roll_info.ki));
	add_update_target_float(ROLL_KD, &(pid_roll_info.kd));

	reset_global_data(PITCH_KP, "pitch.kp", READ_WRITE);
	reset_global_data(PITCH_KI, "pitch.ki", READ_WRITE);
	reset_global_data(PITCH_KD, "pitch.kd", READ_WRITE);
	add_update_target_float(PITCH_KP, &(pid_pitch_info.kp));
	add_update_target_float(PITCH_KI, &(pid_pitch_info.ki));
	add_update_target_float(PITCH_KD, &(pid_pitch_info.kd));

	reset_global_data(YAW_KP, "yaw.kp", READ_WRITE);
	reset_global_data(YAW_KI, "yaw.ki", READ_WRITE);
	reset_global_data(YAW_KD, "yaw.kd", READ_WRITE);
	add_update_target_float(YAW_KP, &(pid_yaw_info.kp));
	add_update_target_float(YAW_KI, &(pid_yaw_info.ki));
	add_update_target_float(YAW_KD, &(pid_yaw_info.kd));
#endif
} 


/**
  * @brief  reset data value and set global data's name and assess right
  * @param  index (int), name (char *), assess_right (AcessRight)
  * @retval None
  */
void reset_global_data(int index, char *name, AccessRight access_right)
{
	/* Index is in the range or not */
	if((index < 0) || (index >= GLOBAL_DATA_CNT)) {
		return;
	}
	/* Available access right or not */
	if((access_right != READ_ONLY) && (access_right != READ_WRITE)) {
		return;
	}

	/* Reset all data to be defalut */
	global_mav_data_list[index].name = name;
	global_mav_data_list[index].type = FLOAT;
	global_mav_data_list[index].flt_value = 0.0;
	global_mav_data_list[index].int_value = 0;
	global_mav_data_list[index].access_right = access_right;
	if(access_right == READ_WRITE) modifiable_data_cnt++;
}

/**
  * @brief  Connect the variable to the global data, update the value while global data is changed
  * @param  index (int), target variable (int *)
  * @retval None
  */
void add_update_target_int(int index, int *target)
{
	/* Index is in the range or not */
	if((index < 0) || (index >= GLOBAL_DATA_CNT)) {
		return;
	}
	//Target variable is exist
	if(target == NULL) {
		return;
	}

	global_mav_data_list[index].target_is_exist = true;
	global_mav_data_list[index].target_int = target;
}

/**
  * @brief  Connect the variable to the global data, update the value while global data is changed
  * @param  index (int), target variable (float *)
  * @retval None
  */
void add_update_target_float(int index, float *target)
{
	/* Index is in the range or not */
	if((index < 0) || (index <= GLOBAL_DATA_CNT)) {
		return;
	}
	//Target variable is exist
	if(target == NULL) {
		return;
	}

	global_mav_data_list[index].target_is_exist = true;
	global_mav_data_list[index].target_float = target;
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
  * @brief  set global data's value (int version)
  * @param  index (int), value (int)
  * @retval None
  */
void set_global_data_int(int index, int value)
{
	/* Index is in the range or not */
	if((index < 0) || (index >= GLOBAL_DATA_CNT)) {
		return;
	}

	/* Set the variable type and value */
	global_mav_data_list[index].type = INTEGER;
	global_mav_data_list[index].int_value = value;

	/* Update target variable */
	if(global_mav_data_list[index].target_is_exist == true)
		*(global_mav_data_list[index].target_int) = value;
}

/**
  * @brief  set global data's value (float version)
  * @param  index (int), value (int)
  * @retval None
  */
void set_global_data_float(int index, float value)
{
	/* Index is in the range or not */
	if((index < 0) || (index >= GLOBAL_DATA_CNT)) {
		return;
	}

	/* Set the variable type and value */
	global_mav_data_list[index].type = FLOAT;
	global_mav_data_list[index].flt_value = value;	

	/* Update target variable */
	if(global_mav_data_list[index].target_is_exist == true)
		*(global_mav_data_list[index].target_float) = value;
}

/**
  * @brief  get the variable type of global data
  * @param  index (int)
  * @retval variable type (Type)
  */
Type get_global_data_type(int index)
{
	/* Index is in the range or not */
	if((index < 0) || (index >= GLOBAL_DATA_CNT)) {
		return FLOAT;
	}

	return global_mav_data_list[index].type;
}

/**
  * @brief  get the access right of global data
  * @param  index (int)
  * @retval access right (AccessRight)
  */
AccessRight get_global_data_access_right(int index)
{
	/* Index is in the range or not */
	if((index < 0) || (index >= GLOBAL_DATA_CNT)) {
		return READ_ONLY;
	}

	return global_mav_data_list[index].access_right;
}

/**
  * @brief  get the name of global data
  * @param  index (int)
  * @retval global data's name (char *)
  */
char *read_global_data_name(int index)
{
        /* Index is in the range or not */
	if((index < 0) || (index >= GLOBAL_DATA_CNT)) {
		return NULL;
	}
	
	return global_mav_data_list[index].name;
}

/**
  * @brief  get the value of global data (int version)
  * @param  index (int)
  * @retval global data's value (int)
  */
int read_global_data_int(int index)
{
        /* Index is in the range or not */
	if((index < 0) || (index >= GLOBAL_DATA_CNT)) {
		return 0;
	}
	
	return global_mav_data_list[index].int_value;
}

/**
  * @brief  get the value of global data (float version)
  * @param  index (float)
  * @retval global data's value (float)
  */
float read_global_data_float(int index)
{
        /* Index is in the range or not */
	if((index < 0) || (index >= GLOBAL_DATA_CNT)) {
		return 0.0;
	}

	return global_mav_data_list[index].flt_value;
}

/**
  * @brief  get the boot time value
  * @param  None
  * @retval boot time value (int)
  */
uint32_t get_boot_time(void)
{
	return read_global_data_int(BOOT_TIME);
}

