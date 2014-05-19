#include "QuadCopterConfig.h"

#define ASSERT(condition) if(!(condition)) return -1;

#define VAR_DEF(id, _name) [id] = {.name = _name}

global_t variable[] = {
	/* Vehicle information */
	VAR_DEF(VEHICLE_TYPE, "vehicle_type"),

	/* Boot ime */
	VAR_DEF(BOOT_TIME, "boot_time"),

	VAR_DEF(PWM1_CCR, "pwm1"),
	VAR_DEF(PWM2_CCR, "pwm2"),
	VAR_DEF(PWM3_CCR, "pwm3"),
	VAR_DEF(PWM4_CCR, "pwm4"),
	VAR_DEF(PWM5_CCR, "pwm5"),

	VAR_DEF(MOTOR1, "motor1"),
	VAR_DEF(MOTOR2, "motor2"),
	VAR_DEF(MOTOR3, "motor3"),
	VAR_DEF(MOTOR4, "motor4"),

	VAR_DEF(NO_RC_SIGNAL_MSG, "rc.status"),

	VAR_DEF(RC_EXP_ROLL, "rc.roll"),
	VAR_DEF(RC_EXP_PITCH, "rc.pitch"),
	VAR_DEF(RC_EXP_YAW, "rc.yaw"),
	VAR_DEF(RC_EXP_THR, "rc.throttle"),

	/* IMU information */
	VAR_DEF(TRUE_ROLL, "imu.roll"),
	VAR_DEF(TRUE_PITCH, "imu.pitch"),
	VAR_DEF(TRUE_YAW, "imu.yaw"),

	VAR_DEF(PID_ROLL, "pid.roll"),
	VAR_DEF(PID_PITCH, "pid.pitch"),
	VAR_DEF(PID_YAW, "pid.yaw"),

	/* GPS Location */
	VAR_DEF(GPS_LAT, "gps.latitude"),
	VAR_DEF(GPS_LON, "gps.longitude"),
	VAR_DEF(GPS_ALT, "gps.altitude"),

	/* GPS Speed */
	VAR_DEF(GPS_VX, "gps.vx"),
	VAR_DEF(GPS_VY, "gps.vy"),
	VAR_DEF(GPS_VZ, "gps.vz")
};

system_t system = {
	.variable = variable,
	.var_count = SYS_VAR_CNT,
	.status = SYSTEM_UNINITIALIZED
};

status_t SD_status;

uint64_t boot_time = 0;

/* The code below this line is being developed and not using right now */

vehicle_data_t global_mav_data_list[SYS_VAR_CNT];

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

