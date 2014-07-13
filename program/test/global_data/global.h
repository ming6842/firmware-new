#ifndef __GLOBAL_H
#define __GLOBAL_H

#include <stdint.h>
#include <stdbool.h>

#define DATA_CAST(data) (Data )(data)
#define DATA_POINTER_CAST(data) (Data *)(data)

/* Global data enumeration */
enum {
	TEST_UINT8,
	TEST_INT8,
	TEST_UINT16,
	TEST_INT16,
	TEST_UINT32,
	TEST_INT32,
	TEST_FLOAT,
	TEST_PARAMETER_CONFIG,
	GLOBAL_DATA_CNT
};

/* Global data exit status */
typedef enum {
	GLOBAL_SUCCESS,
	GLOBAL_ERROR_INDEX_OUT_RANGE,
} GlobalExitStatus;

/* Data type, map as the mavlink message type */
typedef enum { 
	UINT8  = 1,
	INT8   = 2,
	UINT16 = 3,
	INT16  = 4,
	UINT32 = 5,
	INT32  = 6,
	FLOAT  = 9,
} Type;

/* Multiple data union */
typedef union {
	uint8_t  uint8_value;
	int8_t   int8_value;
	uint16_t uint16_value;
	int16_t  int16_value;
	uint32_t uint32_value;
	int32_t  int32_value;
	float    float_value;
} Data;

typedef struct {
	/* Data */
	Type type;
	Data data;

	/* Data name */
	char *name;

	/* QGroundControl parameter config */
	bool parameter_config;
} global_data_t ;

/* Global data list operating functions */
void init_global_data(void);
int get_global_data_count(void);
int get_modifiable_data_count(void);
int set_global_data_value(int index, Type type, Data value);
int get_global_data_type(int index, Type *type);
int get_global_data_parameter_config_status(int index, bool *parameter_config);
int read_global_data_name(int index, char **name);
int read_global_data_value(int index, Data *value);

#endif
