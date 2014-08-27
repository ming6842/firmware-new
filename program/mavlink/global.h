#ifndef __GLOBAL_H
#define __GLOBAL_H

#include <stdint.h>
#include <stdbool.h>

#define DATA_CAST(data) (Data )(data)
#define DATA_POINTER_CAST(data) (Data *)(data)

/* Global data enumeration */
enum {
	/* Vehicle type */
	VEHICLE_TYPE,
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
	/* Remote controller */
	SAFTY_BUTTON,
	MODE_BUTTON,
	/* PID controller */
	ROLL_KP,
	ROLL_KI,
	ROLL_KD,
	PITCH_KP,
	PITCH_KI,
	PITCH_KD,
	YAW_KP,
	YAW_KI,
	YAW_KD,
	HEADING_KP,
	HEADING_KI,
	HEADING_KD,
	Z_KP,
	Z_KI,
	Z_KD,
	ZD_KP,
	ZD_KI,
	ZD_KD,
	/* Sensor calibration */
	ACCEL_X_MAX,
	ACCEL_X_MIN,
	ACCEL_Y_MAX,
	ACCEL_Y_MIN,
	ACCEL_Z_MAX,
	ACCEL_Z_MIN,
	MAG_X_MAX, 
	MAG_X_MIN,
	MAG_Y_MAX,
	MAG_Y_MIN,
	MAG_Z_MAX,
	MAG_Z_MIN,
	GLOBAL_DATA_CNT
};

/* Global data exit status */
typedef enum {
	GLOBAL_SUCCESS,
	GLOBAL_ERROR_INDEX_OUT_RANGE,
	GLOBAL_EEPROM_INVALID_ADDRESS
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

	uint16_t eeprom_address;
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
int set_global_data_eeprom_address(int index, uint16_t eeprom_address);
int get_global_data_eeprom_address(int index, uint16_t *eeprom_address);
int save_global_data_into_eeprom(int index);
void load_global_data_from_eeprom(void);

#endif
