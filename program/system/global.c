#include "QuadCopterConfig.h"

global_t variable[] = {
	/* Vehicle information */
	[VEHICLE_TYPE] = {.name = "vehicle_type"},

	/* Boot ime */
	[BOOT_TIME] = {.name = "boot_time"},

	[PWM1_CCR] = {.name = "pwm1"},
	[PWM2_CCR] = {.name = "pwm2"},
	[PWM3_CCR] = {.name = "pwm3"},
	[PWM4_CCR] = {.name = "pwm4"},
	[PWM5_CCR] = {.name = "pwm5"},

	[MOTOR1] = {.name = "motor1"},
	[MOTOR2] = {.name = "motor2"},
	[MOTOR3] = {.name = "motor3"},
	[MOTOR4] = {.name = "motor4"},

	[NO_RC_SIGNAL_MSG] = {.name = "rc.status"},

	[RC_EXP_ROLL] = {.name = "rc.roll"},
	[RC_EXP_PITCH] = {.name = "rc.pitch"},
	[RC_EXP_YAW] = {.name = "rc.yaw"},
	[RC_EXP_THR] = {.name = "rc.throttle"},

	/* IMU information */
	[TRUE_ROLL] = {.name = "imu.roll"},
	[TRUE_PITCH] = {.name = "imu.pitch"},
	[TRUE_YAW] = {.name = "imu.yaw"},

	[PID_ROLL] = {.name = "pid.roll"},
	[PID_PITCH] = {.name = "pid.pitch"},
	[PID_YAW] = {.name = "pid.yaw"},

	/* GPS Location */
	[GPS_LAT] = {.name = "gps.latitude"},
	[GPS_LON] = {.name = "gps.longitude"},
	[GPS_ALT] = {.name = "gps.altitude"},

	/* GPS Speed */
	[GPS_VX] = {.name = "gps.vx"},
	[GPS_VY] = {.name = "gps.vy"},
	[GPS_VZ] = {.name = "gps.vz"}
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
