#include "QuadCopterConfig.h"

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

xSemaphoreHandle serial_tx_wait_sem = NULL;
xQueueHandle serial_rx_queue = NULL;
