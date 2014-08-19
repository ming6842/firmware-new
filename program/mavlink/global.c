#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
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

	/* Remote controller */
	[RC_STATUS] = {.name = "remote_controll.status", .type = UINT8},

	/* Attitude PID gain */
	[ROLL_KP] = {.name = "roll.kp", .type = FLOAT, .parameter_config = true},
	[ROLL_KI] = {.name = "roll.ki", .type = FLOAT, .parameter_config = true},
	[ROLL_KD] = {.name = "roll.kd", .type = FLOAT, .parameter_config = true},
	[PITCH_KP] = {.name = "pitch.kp", .type = FLOAT, .parameter_config = true},
	[PITCH_KI] = {.name = "pitch.ki", .type = FLOAT, .parameter_config = true},
	[PITCH_KD] = {.name = "pitch.kd", .type = FLOAT, .parameter_config = true},
	[YAW_KP] = {.name = "yaw.kp", .type = FLOAT, .parameter_config = true},
	[YAW_KI] = {.name = "yaw.ki", .type = FLOAT, .parameter_config = true},
	[YAW_KD] = {.name = "yaw.kd", .type = FLOAT, .parameter_config = true},

	/* Sensor calibration */
	[ACCEL_X_MAX] = {.name = "accel.max-x", .type = FLOAT, .parameter_config = true},
	[ACCEL_X_MIN] = {.name = "accel.min-x", .type = FLOAT, .parameter_config = true},
	[ACCEL_Y_MAX] = {.name = "accel.max-y", .type = FLOAT, .parameter_config = true},
	[ACCEL_Y_MIN] = {.name = "accel.min-y", .type = FLOAT, .parameter_config = true},
	[ACCEL_Z_MAX] = {.name = "accel.max-z", .type = FLOAT, .parameter_config = true},
	[ACCEL_Z_MIN] = {.name = "accel.min-z", .type = FLOAT, .parameter_config = true},
	[MAG_X_MAX] = {.name = "mag.max-x", .type = FLOAT, .parameter_config = true},
	[MAG_X_MIN] = {.name = "mag.min-x", .type = FLOAT, .parameter_config = true},
	[MAG_Y_MAX] = {.name = "mag.max-y", .type = FLOAT, .parameter_config = true},
	[MAG_Y_MIN] = {.name = "mag.min-y", .type = FLOAT, .parameter_config = true},
	[MAG_Z_MAX] = {.name = "mag.max-z", .type = FLOAT, .parameter_config = true},
	[MAG_Z_MIN] = {.name = "mag.min-z", .type = FLOAT, .parameter_config = true}
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
	uint8_t eeprom_data[5] = {0};
	eeprom.read(eeprom_data, 0, 1);

	/* If first byte of EEPROM is set ad 0x40, it means the EEPROM has been wrote */	
	eeprom_is_wrote = (eeprom_data[0] == 0x40 ? true : false);

	bool parameter_config;
	/* Start from second byte, 
	 * First byte: check the eeprom has been use or not
	 */
	uint16_t eeprom_address = 1;
	Type type;
	uint8_t type_size;
	Data data;

	for(i = 0; i < get_global_data_count(); i++) {
		get_global_data_parameter_config_status(i, &parameter_config);

		if(parameter_config == true) {
			//Set the eeprom address into the global data
			set_global_data_eeprom_address(i, eeprom_address);

			get_global_data_type(i, &type);

			/* Get the size of the current global data's data type */
			switch(type) {
			    case UINT8:
				type_size = sizeof(uint8_t);
				break;
			    case INT8:
				type_size = sizeof(int8_t);
				break;
			    case UINT16:
				type_size = sizeof(uint16_t);
				break;
			    case INT16:
				type_size = sizeof(int16_t);
				break;
			    case UINT32:
				type_size = sizeof(uint32_t);
				break;
			    case INT32:
				type_size = sizeof(int32_t);
				break;
			    case FLOAT:
				type_size = sizeof(float);
				break;
			}

			set_global_data_value(i, type, DATA_CAST(data));

			//One more byte for checksum
			eeprom_address += type_size + 1;
		}
	}

	if(eeprom_is_wrote == true) {
		/* Read the data from the eeprom */
		eeprom.read(eeprom_data, eeprom_address, type_size + 1);
		memcpy(&data, eeprom_data, type_size);
	
		//TODO: Checksum Test to test the eeprom data
	} else {
		/* Clear the EEPROM */
		uint8_t buffer[1024] = {'\0'};
		eeprom.write(buffer, 0, 1024);
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
	/* Index is in the range or not */
	if((index < 0) || (index >= GLOBAL_DATA_CNT))
		return GLOBAL_ERROR_INDEX_OUT_RANGE;

	/* Set the variable type and value */
	global_mav_data_list[index].type = type;

	switch(type) {
	    case UINT8:
		global_mav_data_list[index].data.uint8_value = 
			value.uint8_value;
		break;
	    case INT8:
		global_mav_data_list[index].data.int8_value = 
			value.int8_value;
		break;
	    case UINT16:
		global_mav_data_list[index].data.uint16_value = 
			value.uint16_value;
		break;
	    case INT16:
		global_mav_data_list[index].data.int16_value = 
			value.int16_value;
		break;
	    case UINT32:
		global_mav_data_list[index].data.uint32_value = 
			value.uint32_value;
		break;
	    case INT32:
		global_mav_data_list[index].data.int32_value = 
			value.int32_value;
		break;
	    case FLOAT:
		global_mav_data_list[index].data.float_value = 
			value.float_value;
		break;
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

int save_global_data_into_eeprom(int index)
{
        /* Index is in the range or not */
	if((index < 0) || (index >= GLOBAL_DATA_CNT))
		return GLOBAL_ERROR_INDEX_OUT_RANGE;

	uint8_t *buffer;
	uint16_t eeprom_address;
	uint8_t data_len;

	switch(global_mav_data_list[index].type) {
	    case UINT8:
		buffer = (uint8_t *)&global_mav_data_list[index].data.uint8_value;
		data_len = sizeof(uint8_t);
		break;
	    case INT8:
		buffer = (uint8_t *)&global_mav_data_list[index].data.int8_value;
		data_len = sizeof(int8_t);
		break;
	    case UINT16:
		buffer = (uint8_t *)&global_mav_data_list[index].data.uint16_value;
		data_len = sizeof(uint16_t);
		break;
	    case INT16:
		buffer = (uint8_t *)&global_mav_data_list[index].data.int16_value;
		data_len = sizeof(uint16_t);
		break;
	    case UINT32:
		buffer = (uint8_t *)&global_mav_data_list[index].data.uint32_value;
		data_len = sizeof(uint32_t);
		break;
	    case INT32:
		buffer = (uint8_t *)&global_mav_data_list[index].data.int32_value;
		data_len = sizeof(int32_t);
		break;
	    case FLOAT:
		buffer = (uint8_t *)&global_mav_data_list[index].data.float_value;
		data_len = sizeof(float);
		break;
	}

	if(global_mav_data_list[index].parameter_config == true) {
		//Get the eeprom address
		eeprom_address = global_mav_data_list[index].eeprom_address;

		/* Write the data into the eeprom */
		eeprom.write(buffer, eeprom_address, data_len); //Payload, n byte
		eeprom.write('\0', eeprom_address + data_len + 1, 1); //Checksum, 1 byte

		/* Verify the data */
		Data data_eeprom;
		uint8_t buffer_verify[5]; //Hard code, the max size of the multiple data type
		bool data_is_correct;

		eeprom.read(buffer_verify, eeprom_address, data_len);
		memcpy(&data_eeprom, buffer_verify, data_len);

		//TODO: The code is too long, improve this!
		switch(global_mav_data_list[index].type) {
		    case UINT8:
			if(global_mav_data_list[index].data.uint8_value == data_eeprom.uint8_value)
				data_is_correct = true;
			break;
		    case INT8:
			if(global_mav_data_list[index].data.int8_value == data_eeprom.int8_value)
				data_is_correct = true;
			break;
		    case UINT16:
			if(global_mav_data_list[index].data.uint16_value == data_eeprom.uint16_value)
				data_is_correct = true;
			break;
		    case INT16:
			if(global_mav_data_list[index].data.int16_value == data_eeprom.int16_value)
				data_is_correct = true;
			break;
		    case UINT32:
			if(global_mav_data_list[index].data.uint32_value == data_eeprom.uint32_value)
				data_is_correct = true;
			break;
		    case INT32:
			if(global_mav_data_list[index].data.int32_value == data_eeprom.int32_value)
				data_is_correct = true;
			break;
		    case FLOAT:
			if(fabs(global_mav_data_list[index].data.float_value - data_eeprom.float_value) <= 0.0001)
				data_is_correct = true;
			break;
		}

		if(data_is_correct == false) {
			printf("EEPROM Data Check is failed!"); //TODO:Data is not correct, handle this situation!
		}

		/* Set up the first byte of eeprom (data = 0x40) */
		if(eeprom_is_wrote == false) {
			uint8_t start_byte = 0x40;
			eeprom_is_wrote = true;
			eeprom.write(&start_byte, 0, 1);
		}
	}


	return GLOBAL_SUCCESS;
}
