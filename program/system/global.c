#include "QuadCopterConfig.h"

#define ASSERT(condition) if(!(condition)) return -1;

#define VAR_DEF(id, _name) [id] = {.name = _name}

system_t system = {
	.status = SYSTEM_UNINITIALIZED
};

status_t SD_status;

global_data_t global_mav_data_list[SYS_VAR_CNT];

#define QUADCOPTER 0
void init_vehicle_data()
{
	/* Vehicle information */
	reset_vehicle_data(VEHICLE_TYPE, "vehicle_type", READ_ONLY);
	set_vehicle_data(VEHICLE_TYPE, QUADCOPTER, 0.0, INTEGER);

	/* Boot ime */
	reset_vehicle_data(BOOT_TIME, "boot_time", READ_ONLY);
	set_vehicle_data(BOOT_TIME, 0, 0.0, INTEGER);

	reset_vehicle_data(PWM1_CCR, "pwm1", READ_ONLY);
	reset_vehicle_data(PWM2_CCR, "pwm2", READ_ONLY);
	reset_vehicle_data(PWM3_CCR, "pwm3", READ_ONLY);
	reset_vehicle_data(PWM4_CCR, "pwm4", READ_ONLY);
	reset_vehicle_data(PWM5_CCR, "pwm5", READ_ONLY);

	reset_vehicle_data(MOTOR1, "motor1", READ_ONLY);
	reset_vehicle_data(MOTOR2, "motor2", READ_ONLY);
	reset_vehicle_data(MOTOR3, "motor3", READ_ONLY);
	reset_vehicle_data(MOTOR4, "motor4", READ_ONLY);

	reset_vehicle_data(NO_RC_SIGNAL_MSG, "rc.status", READ_ONLY);
	
	reset_vehicle_data(RC_EXP_ROLL, "rc.roll", READ_ONLY);
	reset_vehicle_data(RC_EXP_PITCH, "rc.pitch", READ_ONLY);
	reset_vehicle_data(RC_EXP_YAW, "rc.yaw", READ_ONLY);
	reset_vehicle_data(RC_EXP_THR, "rc.throttle", READ_ONLY);

	/* IMU information */
	reset_vehicle_data(TRUE_ROLL, "imu.roll", READ_ONLY);
	reset_vehicle_data(TRUE_PITCH, "imu.pitch", READ_ONLY);
	reset_vehicle_data(TRUE_YAW, "imu.yaw", READ_ONLY);

	reset_vehicle_data(PID_ROLL, "pid.roll", READ_ONLY);
        reset_vehicle_data(PID_PITCH, "pid.pitch", READ_ONLY);
        reset_vehicle_data(PID_YAW, "pid.yaw", READ_ONLY);

	/* GPS Location */
        reset_vehicle_data(GPS_LAT, "gps.latitude", READ_ONLY);
        reset_vehicle_data(GPS_LON, "gps.longitude", READ_ONLY);
        reset_vehicle_data(GPS_ALT, "gps.altitude", READ_ONLY);

	/* GPS Speed */
        reset_vehicle_data(GPS_VX, "gps.vx", READ_ONLY);
        reset_vehicle_data(GPS_VY, "gps.vy", READ_ONLY);
        reset_vehicle_data(GPS_VZ, "gps.vz", READ_ONLY);
} 

int get_vehicle_data_count()
{
	return SYS_VAR_CNT;
}

int reset_vehicle_data(int index, char *name, AccessRight access_right)
{
	ASSERT((index >= 0) && (index < SYS_VAR_CNT)); /* Index is in the range or not */
	ASSERT((access_right == READ_ONLY) || (access_right == READ_WRITE)); /* Available access right or not */

	/* Reset all data to be defalut */
	global_mav_data_list[index].name = name;
	global_mav_data_list[index].type = FLOAT;
	global_mav_data_list[index].flt_value = 0.0;
	global_mav_data_list[index].int_value = 0;
	global_mav_data_list[index].access_right = READ_ONLY;

	return 0;
}

int set_vehicle_data(int index, int int_val, float flt_val, Type type)
{
	ASSERT((index >= 0) && (index < SYS_VAR_CNT)); /* Index is in the range or not */
	ASSERT((type == INTEGER) || (type == FLOAT)); /* Variable type is exist or not */

	/* Assign value to the variable according to the type */
	switch(type) {
	    case INTEGER:
		global_mav_data_list[index].int_value = int_val;
		break;
	    case FLOAT:
		global_mav_data_list[index].flt_value = flt_val;	
	}

	return 0;
}

Type get_vehicle_data_type(int index)
{
	ASSERT((index >= 0) && (index < SYS_VAR_CNT)); /* Index is in the range or not */

	return global_mav_data_list[index].type;
}

char *read_vehicle_data_name(int index)
{
	ASSERT((index >= 0) && (index < SYS_VAR_CNT)); /* Index is in the range or not */
	
	return global_mav_data_list[index].name;
}

int read_vehicle_data_int(int index)
{
	ASSERT((index >= 0) && (index < SYS_VAR_CNT)); /* Index is in the range or not */
	
	return global_mav_data_list[index].int_value;
}

float read_vehicle_data_flt(int index)
{
	ASSERT((index >= 0) && (index < SYS_VAR_CNT)); /* Index is in the range or not */

	return global_mav_data_list[index].flt_value;
}

uint32_t get_boot_time()
{
	return read_vehicle_data_int(BOOT_TIME);
}

