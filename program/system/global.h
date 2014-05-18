#ifndef __SYS_MANAGER_H
#define __SYS_MANAGER_H

#include "QuadCopterConfig.h"

enum {
	VEHICLE_TYPE,
	BOOT_TIME,
	PWM1_CCR,
	PWM2_CCR,
	PWM3_CCR,
	PWM4_CCR,
	PWM5_CCR,
	RC_EXP_ROLL,
	RC_EXP_PITCH,
	RC_EXP_YAW,
	RC_EXP_THR,
	TRUE_ROLL,
	TRUE_PITCH,
	TRUE_YAW,
	NO_RC_SIGNAL_MSG,
	PID_ROLL,
	PID_PITCH,
	PID_YAW,
	MOTOR1,
	MOTOR2,
	MOTOR3,
	MOTOR4,
	GPS_LAT,
	GPS_LON,
	GPS_ALT,
	GPS_VX,
	GPS_VY,
	GPS_VZ,
	SYS_VAR_CNT
};

typedef __IO enum {
	SYSTEM_UNINITIALIZED,
	SYSTEM_INITIALIZED,
	SYSTEM_CORRECTION_SENSOR,
	SYSTEM_FLIGHT_CONTROL,
	/* Hardware Error */
	SYSTEM_ERROR_SD
} System_Status;

typedef __IO enum {
	SD_READY,
	SD_UNREADY,
	SD_SAVE,
	SD_UNSAVE,
	SD_ERSAVE
} SD_Status;

typedef int status_t;

typedef struct  {
	char *name;
	__IO float value;
} global_t;

typedef struct {
	global_t *variable;
	int var_count;
	status_t status;
} system_t;

global_t variable[SYS_VAR_CNT];

/* Access right */
typedef enum {
	READ_ONLY,
	READ_WRITE
} AccessRight;

/* Vehicle data typedef */
typedef enum { INTEGER, FLOAT } Type;
typedef struct {
	Type type;

	union {
		float flt_value;
		int int_value;
	};

	AccessRight access_right;
} vehicle_data_t ;

extern uint64_t boot_time;

extern system_t system;

extern status_t SD_status;

extern xSemaphoreHandle serial_tx_wait_sem;
extern xQueueHandle serial_rx_queue;

#endif
