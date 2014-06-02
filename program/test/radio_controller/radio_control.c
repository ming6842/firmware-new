#include "radio_control.h"
#include "pwm_decoder.h"
#include <stdio.h>
//#define DEBUG_RADIO_CONTROLLER
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
	rc_data->roll_control_input = radio_controller.roll_control_input;
	rc_data->pitch_control_input = radio_controller.pitch_control_input;
	rc_data->throttle_control_input  = radio_controller.throttle_control_input ;
	rc_data->yaw_rate_control_input = radio_controller.yaw_rate_control_input;
	rc_data->safety = radio_controller.safety;
	rc_data->mode = radio_controller.mode;
#ifdef DEBUG_RADIO_CONTROLLER
	printf("%d,%d,%d,%d,",
		(int16_t) (my_rc.roll_control_input*100),	
		(int16_t) (my_rc.pitch_control_input*100),
		(int16_t) (my_rc.throttle_control_input),
		(int16_t) (my_rc.yaw_rate_control_input*100));
	if (my_rc.safety == ENGINE_ON) {

			printf("ENGINE_ON,%d");
	} else {

			printf("ENGINE_OFF,%d");

	}
	printf("\r\n");
#endif	
}