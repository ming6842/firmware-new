#ifndef __GLOBAL_H
#define __GLOBAL_H

#include <stdint.h>
#include <stdbool.h>

/* Global data enumeration */
enum {
	/* Vehicle type */
	VEHICLE_TYPE,
	/* Boot time */
	BOOT_TIME,
	/* Attitude information (true value ) */
	TRUE_ROLL,
	TRUE_PITCH,
	TRUE_YAW,
	/* GPS information */
	GPS_LAT,
	GPS_LON,
	GPS_ALT,	
	GPS_VX,
	GPS_VY,
	GPS_VZ,
	/* PID Controller */
	ROLL_KP,
	ROLL_KI,
	ROLL_KD,
	PITCH_KP,
	PITCH_KI,
	PITCH_KD,
	YAW_KP,
	YAW_KI,
	YAW_KD,
	GLOBAL_DATA_CNT
};

/* Access right */
typedef enum {
	READ_ONLY,
	READ_WRITE
} AccessRight;

/* Global data typedef */
typedef enum { INTEGER, FLOAT } Type;
typedef struct {
	/* Data */
	Type type;
	union {
		float flt_value;
		int int_value;
	};

	/* Data name */
	char *name;

	/* Access right */
	AccessRight access_right;

	/* Target information */
	bool target_is_exist;
	int *target_int;
	float *target_float;
} global_data_t ;

/* Global data list operating functions */
void init_global_data(void);
void reset_global_data(int index, char *name, AccessRight access_right);
void add_update_target_int(int index, int *target);
void add_update_target_float(int index, float *target);
int get_global_data_count(void);
int get_modifiable_data_count(void);
void reset_global_data(int index, char *name, AccessRight access_right);
void set_global_data_int(int index, int value);
void set_global_data_float(int index, float value);
Type get_global_data_type(int index);
AccessRight get_global_data_access_right(int index);
char *read_global_data_name(int index);
int read_global_data_int(int index);
float read_global_data_float(int index);

uint32_t get_boot_time(void);

#endif
