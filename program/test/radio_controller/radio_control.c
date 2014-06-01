#include "radio_control.h"
#include "pwm_decoder.h"
#include <stdio.h>
#define DEBUG_RADIO_CONTROLLER
static radio_controller_t radio_controller = {
	.roll_control_input = 0.0f,
	.pitch_control_input = 0.0f,
	.throttle_control_input = 0.0f,
	.yaw_rate_control_input = 0.0f,
	.safety = ENGINE_ON,
	.mode = MODE_1
	
};
void update_radio_control_input(void)
{
	get_pwm_decode_value(&radio_controller);
#ifdef DEBUG_RADIO_CONTROLLER
	printf("%d,%d,%d,%d,",
	(int16_t) (radio_controller.roll_control_input*100),	
	(int16_t) (radio_controller.pitch_control_input*100),
	(int16_t) (radio_controller.throttle_control_input),
	(int16_t) (radio_controller.yaw_rate_control_input*100));
	if (radio_controller.safety == ENGINE_ON) {

		printf("ENGINE_ON,%d",(int16_t)RC_CHANNEL_5_INPUT_CAPTURE);
	} else {

		printf("ENGINE_OFF,%d",(int16_t)RC_CHANNEL_5_INPUT_CAPTURE);

	}
	printf("\r\n");
#endif	
}