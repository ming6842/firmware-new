#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "global.h"

#define PASSED 0
#define FAIL   1

/* Expect value table */
#define TEST_INC_VALUE 10

char *except_name[] = {
	"test_uint8", "test_int8", "test_uint16", "test_int16",
	"test_uint32", "test_int32", "test_float", "test_parameter_config"
};

int except_type[] = {UINT8, INT8, UINT16, INT16, UINT32, INT32, FLOAT, UINT8};
char *type_name[] = {"X", "UINT8", "INT8", "UINT16", "INT16", "UINT32", "INT32",
	"X", "X", "FLOAT", "UINT8"};

Data expect_value[] = {
	{.uint8_value = +8},
	{.int8_value = -8},
	{.uint16_value = +16},
	{.int16_value = -16},
	{.uint32_value = +32},
	{.int32_value = -32},
	{.float_value = 666.666},
	{.uint8_value = 8}
};

bool expect_param_config[] = {false, false, false, false, false, false, false, true};

/* Unit test */
int main()
{
	int unit_test_status = PASSED;
	printf("[Unit test for global data]\n");

	/* Initialize the global data list */
	init_global_data();

	/* Name test */
	printf("\[Name test]\n");
	int i;
	for(i = 0; i < get_global_data_count(); i++) {
		char *testing_name;
		read_global_data_name(i, &testing_name); /* Read the testing data */
		printf("Testing name:%s | Expect name:%s", testing_name,
			 *(except_name + i));

		/* Compare the name */
		if(strcmp(testing_name, *(except_name + i)) == 0) {
			printf(" | Pass\n");
		} else {
			printf(" | Fail\n");
			unit_test_status = FAIL;
		}
	}

	/* Type test */
	printf("\n[Data type test]\n");
	for(i = 0; i < get_global_data_count(); i++) {
		Type testing_type;
		get_global_data_type(i, &testing_type);
		printf("Testing type:%s | Expect type:%s", type_name[testing_type],
			type_name[except_type[i]]);

		/* Compare the type */
		if(testing_type == except_type[i]) {
			printf(" | Pass\n");
		} else {
			printf(" | Fail\n");
			unit_test_status = FAIL;
		}
	}

	/* Value test */
	printf("\n[Value test]\n");
	for(i = 0; i < get_global_data_count(); i++) {
		int pass_flag = 0;
		Data testing_value;
		read_global_data_value(i, DATA_POINTER_CAST(&testing_value));

		switch(except_type[i]) {
		    case UINT8:
			printf("Testing value:%u | Expect Value:%u",
				testing_value.uint8_value, expect_value[i].uint8_value);

			if(testing_value.uint8_value == expect_value[i].uint8_value)
				pass_flag = 1;
			break;
		    case INT8:
			printf("Testing value:%d | Expect Value:%d",
				testing_value.int8_value, expect_value[i].int8_value);
		
			if(testing_value.int8_value == expect_value[i].int8_value)
				pass_flag = 1;
			break;
		    case UINT16:
			printf("Testing value:%u | Expect Value:%u",
				testing_value.uint16_value, expect_value[i].uint16_value);

			if(testing_value.uint16_value == expect_value[i].uint16_value)
				pass_flag = 1;
			break;	
		    case INT16:
			printf("Testing value:%d | Expect Value:%d",
				testing_value.int16_value, expect_value[i].int16_value);

			if(testing_value.int16_value == expect_value[i].int16_value)
				pass_flag = 1;
			break;	
		    case UINT32:
			printf("Testing value:%u | Expect Value:%u",
				testing_value.uint32_value, expect_value[i].uint32_value);

			if(testing_value.uint32_value == expect_value[i].uint32_value)
				pass_flag = 1;
			break;	
		    case INT32:
			printf("Testing value:%d | Expect Value:%d",
				testing_value.int32_value, expect_value[i].int32_value);

			if(testing_value.int32_value == expect_value[i].int32_value)
				pass_flag = 1;
			break;	
		    case FLOAT:
			printf("Testing value:%f | Expect Value:%f",
				testing_value.float_value, expect_value[i].float_value);

			if(fabs(testing_value.float_value - expect_value[i].float_value) < 0.0001)
				pass_flag = 1;
			break;	
		}

		if(pass_flag) {
			printf(" | Pass\n");
		} else {
			printf(" | Fail\n");
			unit_test_status = FAIL;
		}

	}

	/* Parameter status flag test */
	printf("\nParameter config status test\n");
	for(i = 0; i < get_global_data_count(); i++) {
		bool testing_param_config;
		get_global_data_parameter_config_status(i, &testing_param_config);

		printf("Testing bool status:%s | Expect bool status:%s",
			testing_param_config ? "true" : "false",
			expect_param_config[i] ? "true" : "false"
		);

		/* Compare the parameter config status */
		if(testing_param_config == expect_param_config[i]) {
			printf(" | Pass\n");
		} else {
			printf(" | Fail\n");
			unit_test_status = FAIL;
		}
	}	

	printf("\n\[Pass: %d, Fail:%d]\n", 0, 0);
	return unit_test_status;
}
