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
} SystemStatus;

typedef __IO enum {
	SD_READY,
	SD_UNREADY,
	SD_SAVE,
	SD_UNSAVE,
	SD_ERSAVE
} SD_Status;

/* Access right */
typedef enum {
	READ_ONLY,
	READ_WRITE
} AccessRight;

/* global data typedef */
typedef enum { INTEGER, FLOAT } Type;
typedef struct {
	Type type;

	union {
		float flt_value;
		int int_value;
	};

	char *name;

	AccessRight access_right;
} global_data_t ;

extern uint32_t boot_time;
extern SystemStatus system_status;
extern SD_Status sd_status;

void init_global_data();
int get_global_data_count();
void reset_global_data(int index, char *name, AccessRight access_right);
void set_global_data_int(int index, int value);
void set_global_data_float(int index, float value);
Type get_global_data_type(int index);
char *read_global_data_name(int index);
int read_global_data_int(int index);
float read_global_data_float(int index);

uint32_t get_boot_time();

#endif
