#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "AT24C04C.h"
#include "global.h"
#include "attitude_stabilizer.h"

#define QUADCOPTER 0

bool eeprom_is_wrote;

int modifiable_data_cnt = 0;
global_data_t global_mav_data_list[GLOBAL_DATA_CNT] = {
	/* global data information */
	[VEHICLE_TYPE] = {.name = "vehicle_type", .type = UINT8,
		.data.uint8_value = QUADCOPTER},

	/* IMU information */
	[TRUE_ROLL] = {.name = "imu.roll"},
	[TRUE_PITCH] = {.name = "imu.pitch"},
	[TRUE_YAW] = {.name = "imu.yaw"},

	/* GPS Location */
	[GPS_LAT] = {.name = "gps.latitude", .type = INT32},
	[GPS_LON] = {.name = "gps.longitude", .type = INT32},
	[GPS_ALT] = {.name = "gps.altitude", .type = INT32},

	/* GPS Speed */
	[GPS_VX] = {.name = "gps.vx", .type = INT16},
	[GPS_VY] = {.name = "gps.vy", .type = INT16},
	[GPS_VZ] = {.name = "gps.vz", .type = INT16},

	/* Attitude PID Gain */
	[ROLL_KP] = {.name = "roll.kp", .type = FLOAT, .parameter_config = true},
	[ROLL_KI] = {.name = "roll.ki", .type = FLOAT, .parameter_config = true},
	[ROLL_KD] = {.name = "roll.kd", .type = FLOAT, .parameter_config = true},
	[PITCH_KP] = {.name = "pitch.kp", .type = FLOAT, .parameter_config = true},
	[PITCH_KI] = {.name = "pitch.ki", .type = FLOAT, .parameter_config = true},
	[PITCH_KD] = {.name = "pitch.kd", .type = FLOAT, .parameter_config = true},
	[YAW_KP] = {.name = "yaw.kp", .type = FLOAT, .parameter_config = true},
	[YAW_KI] = {.name = "yaw.ki", .type = FLOAT, .parameter_config = true},
	[YAW_KD] = {.name = "yaw.kd", .type = FLOAT, .parameter_config = true}
};

void init_global_data(void)
{
	/* Calculate the data count on the  ground station parameter
	   configuration panel */
	int i;
	for(i = 0; i < get_global_data_count(); i++) {
		bool parameter_config;

		get_global_data_parameter_config_status(i, &parameter_config);
		if(parameter_config == true)
			modifiable_data_cnt++;
	}
	
	/* Load the data from eeprom */
	uint8_t eeprom_data[6] = {0};
	eeprom.read(eeprom_data, 0, 1);

	/* If first byte of EEPROM is set ad 0x40, it means the EEPROM has been wrote */	
	eeprom_is_wrote = (eeprom_data[0] == 0x40 ? true : false);

	if(eeprom_is_wrote == true) {
		bool parameter_config;
		/* Start from second byte, 
		 * First byte: check the eeprom has been use or not
		 */
		uint16_t eeprom_address = 1;
		Type type;

		for(i = 0; i < get_global_data_count(); i++) {
			get_global_data_parameter_config_status(i, &parameter_config);

			if(parameter_config == true) {
				//Set the eeprom address into the global data
				set_global_data_eeprom_address(i, eeprom_address);

				get_global_data_type(i, &type);

				/* Get the size of the current global 
				   data's data type */
				switch(type) {
				    case UINT8:
				    case INT8:
					eeprom_address += 1;
					break;
				    case UINT16:
				    case INT16:
					eeprom_address += 2;
					break;
				    case UINT32:
				    case INT32:
				    case FLOAT:
					eeprom_address += 4;
					break;
				}

				//Two byte, 1 for payload len, 1 for checksum
				eeprom_address += 2;

				//TODO: Checksum Test to test the eeprom data
			}			
		}
	}
} 

/**
  * @brief  get the count of global data
  * @param  None
  * @retval global data count (int)
  */
int get_global_data_count(void)
{
	return GLOBAL_DATA_CNT;
}

/**
  * @brief  get the count of modifiable global data
  * @param  None
  * @retval modifiable global data count (int)
  */
int get_modifiable_data_count(void)
{
	return modifiable_data_cnt;
}

/**
  * @brief  set global data's value
  * @param  index (int), Type type, value (Data)
  * @retval Operated result (0 - succeeded, 1 - error)
  */
int set_global_data_value(int index, Type type, Data value)
{
	bool parameter_config;
	uint8_t buffer[4];
	uint16_t eeprom_address;
	uint8_t data_len;

	/* Index is in the range or not */
	if((index < 0) || (index >= GLOBAL_DATA_CNT))
		return GLOBAL_ERROR_INDEX_OUT_RANGE;

	/* Set the variable type and value */
	global_mav_data_list[index].type = type;

	//Get the eeprom address
	get_global_data_eeprom_address(index, &eeprom_address);

	get_global_data_parameter_config_status(index, &parameter_config);

	switch(type) {
	    case UINT8:
		global_mav_data_list[index].data.uint8_value = 
			value.uint8_value;

		memcpy(buffer, &value.uint8_value, 1);
		data_len = 1;
		break;
	    case INT8:
		global_mav_data_list[index].data.int8_value = 
			value.int8_value;

		memcpy(buffer, &value.uint8_value, 1);
		data_len = 1;
		break;
	    case UINT16:
		global_mav_data_list[index].data.uint16_value = 
			value.uint16_value;

		memcpy(buffer, &value.uint8_value, 2);
		data_len = 2;
		break;
	    case INT16:
		global_mav_data_list[index].data.int16_value = 
			value.int16_value;

		memcpy(buffer, &value.uint8_value, 2);
		data_len = 2;
		break;
	    case UINT32:
		global_mav_data_list[index].data.uint32_value = 
			value.uint32_value;

		memcpy(buffer, &value.uint8_value, 4);
		data_len = 4;
		break;
	    case INT32:
		global_mav_data_list[index].data.int32_value = 
			value.int32_value;

		memcpy(buffer, &value.uint8_value, 4);
		data_len = 4;
		break;
	    case FLOAT:
		global_mav_data_list[index].data.float_value = 
			value.float_value;

		memcpy(buffer, &value.uint8_value, 4);
		data_len = 4;
		break;
	}

	if(parameter_config == true) {
		/* Write the data into the eeprom */
		eeprom.write(&data_len, eeprom_address, 1); //Payload length, 1 byte
		eeprom.write(buffer, eeprom_address + 1, data_len); //Payload, n byte
		eeprom.write('\0', eeprom_address + data_len, 1); //Checksum, 1 byte

		/* Set up the first byte of eeprom (data = 0x40) */
		if(eeprom_is_wrote == false) {
			eeprom_is_wrote = true;
		}
	}
	return GLOBAL_SUCCESS;
}

/**
  * @brief  get the variable type of global data
  * @param  index (int), variable type (Type* to get the type)
  * @retval Operated result (0 - succeeded, 1 - error)
  */
int get_global_data_type(int index, Type *type)
{
	/* Index is in the range or not */
	if((index < 0) || (index >= GLOBAL_DATA_CNT))
		return GLOBAL_ERROR_INDEX_OUT_RANGE;

	*type = global_mav_data_list[index].type;

	return GLOBAL_SUCCESS;
}

/**
  * @brief  get the access right of global data
  * @param  index (int), access right (AccessRight* to get the type)
  * @retval Operated result (0 - succeeded, 1 - error)
  */
int get_global_data_parameter_config_status(int index, bool *parameter_config)
{
	/* Index is in the range or not */
	if((index < 0) || (index >= GLOBAL_DATA_CNT))
		return GLOBAL_ERROR_INDEX_OUT_RANGE;

	*parameter_config = global_mav_data_list[index].parameter_config;

	return GLOBAL_SUCCESS;
}

/**
  * @brief  get the name of global data
  * @param  index (int), name (char* to get the name)
  * @retval Operated result (0 - succeeded, 1 - error)
  */
int read_global_data_name(int index, char **name)
{
        /* Index is in the range or not */
	if((index < 0) || (index >= GLOBAL_DATA_CNT))
		return GLOBAL_ERROR_INDEX_OUT_RANGE;
	
	*name = global_mav_data_list[index].name;

	return GLOBAL_SUCCESS;
}

/**
  * @brief  get the value of global data
  * @param  index (int), value (Data* to get the result value)
  * @retval Operated result (0 - succeeded, 1 - error)
  */
int read_global_data_value(int index, Data *value)
{
        /* Index is in the range or not */
	if((index < 0) || (index >= GLOBAL_DATA_CNT))
		return GLOBAL_ERROR_INDEX_OUT_RANGE;

	switch(global_mav_data_list[index].type) {
	    case UINT8:
		value->uint8_value = 
			global_mav_data_list[index].data.uint8_value;
		break;
	    case INT8:
		value->int8_value =
			global_mav_data_list[index].data.int8_value;
		break;
	    case UINT16:
		value->uint16_value =
			global_mav_data_list[index].data.uint16_value;
		break;
	    case INT16:
		value->int16_value = 
			global_mav_data_list[index].data.int16_value;
		break;
	    case UINT32:
		value->uint32_value =
			global_mav_data_list[index].data.uint32_value;
		break;
	    case INT32:
		value->int32_value =
			global_mav_data_list[index].data.int32_value;
		break;
	    case FLOAT:
		value->float_value =
			global_mav_data_list[index].data.float_value;
		break;
	}

	return GLOBAL_SUCCESS;
}

int set_global_data_eeprom_address(int index, uint16_t eeprom_address)
{
        /* Index is in the range or not */
	if((index < 0) || (index >= GLOBAL_DATA_CNT))
		return GLOBAL_ERROR_INDEX_OUT_RANGE;

	/* Address is valid or not */
	if(eeprom_address > 1024)
		return GLOBAL_EEPROM_INVALID_ADDRESS;

	global_mav_data_list[index].eeprom_address = eeprom_address;

	return GLOBAL_SUCCESS;
}

int get_global_data_eeprom_address(int index, uint16_t *eeprom_address)
{
        /* Index is in the range or not */
	if((index < 0) || (index >= GLOBAL_DATA_CNT))
		return GLOBAL_ERROR_INDEX_OUT_RANGE;

	*eeprom_address = global_mav_data_list[index].eeprom_address;

	return GLOBAL_SUCCESS;
}
