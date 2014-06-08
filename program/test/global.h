#ifndef __GLOBAL_H
#define __GLOBAL_H

#include <stdint.h>

/* Global data enumeration */
enum {
	VEHICLE_TYPE,
	BOOT_TIME,
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
	Type type;

	union {
		float flt_value;
		int int_value;
	};

	char *name;

	AccessRight access_right;
} global_data_t ;

/* Global data list operating functions */
void init_global_data(void);
int get_global_data_count(void);
void reset_global_data(int index, char *name, AccessRight access_right);
void set_global_data_int(int index, int value);
void set_global_data_float(int index, float value);
Type get_global_data_type(int index);
char *read_global_data_name(int index);
int read_global_data_int(int index);
float read_global_data_float(int index);

uint32_t get_boot_time(void);

#endif
