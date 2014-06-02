#include "radio_control.h"
#include "pwm_decoder.h"
#include <stdio.h>
#include <string.h>
#include "delay.h"
#define DEBUG_RADIO_CONTROLLER
static radio_controller_t radio_controller = {
	.roll_control_input = 0.0f,
	.pitch_control_input = 0.0f,
	.throttle_control_input = 0.0f,
	.yaw_rate_control_input = 0.0f,
	.safety = ENGINE_ON,
	.mode = MODE_1
	
};
void update_radio_control_input(radio_controller_t *rc_data)
{
	get_pwm_decode_value(&radio_controller);
	memcpy(rc_data, &radio_controller, sizeof(radio_controller_t));
	
#ifdef DEBUG_RADIO_CONTROLLER
	printf("%d,%d,%d,%d,",
		(int16_t) (rc_data->roll_control_input*100),	
		(int16_t) (rc_data->pitch_control_input*100),
		(int16_t) (rc_data->throttle_control_input),
		(int16_t) (rc_data->yaw_rate_control_input*100));
	if (rc_data->safety == ENGINE_ON) {

			printf("ENGINE_ON,");
	} else {

			printf("ENGINE_OFF");

	}
	printf("\r\n");
	Delay_1us(100);
#endif	
}