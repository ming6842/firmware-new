#ifndef FILE_INPUT_CAPTURE_H
#define FILE_INPUT_CAPTURE_H

typedef enum {
	RISING = 0,
	FALLING
}INPUT_CAPUTRE_PIN_STATUS;
enum {
	INC1 = 0,
	INC2,
	INC3,
	INC4,
	INC5,
	INC6,
};
typedef struct input_capture{
	volatile INPUT_CAPUTRE_PIN_STATUS status;	
	volatile uint32_t curr_value; // Use this value for control operation
	volatile uint32_t prev_value;

} input_capture_t;

input_capture_t inc[6];
#endif