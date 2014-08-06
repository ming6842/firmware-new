#include "pwm_decoder.h"
#include "bound.h"
extern input_capture_t inc[6];
/*
*/
void get_pwm_decode_value(radio_controller_t* rc)
{
	rc->roll_control_input = ROLL_CONTROL_STEP_SIZE  *
			(float)(RC_CHANNEL_1_INPUT_CAPTURE - RC_CHANNEL_1_NEUTRAL);

	rc->pitch_control_input = - PITCH_CONTROL_STEP_SIZE  *
			(float)(RC_CHANNEL_2_INPUT_CAPTURE - RC_CHANNEL_2_NEUTRAL);

	rc->throttle_control_input = THROTTLE_CONTROL_STEP_SIZE *
			(float)(RC_CHANNEL_3_INPUT_CAPTURE - RC_CHANNEL_3_NEUTRAL);

	rc->yaw_rate_control_input = YAW_RATE_CONTROL_STEP_SIZE  *
			(float)(RC_CHANNEL_4_INPUT_CAPTURE - RC_CHANNEL_4_NEUTRAL);

	if ( RC_CHANNEL_5_INPUT_CAPTURE > RC_CHANNEL_5_HALF_SIZE + RC_CHANNEL_5_NEUTRAL) {

		rc->safety = ENGINE_ON;
	} else {

		rc->safety = ENGINE_OFF;
	}

	if ( RC_CHANNEL_6_INPUT_CAPTURE > (RC_CHANNEL_6_HALF_SIZE + RC_CHANNEL_6_NEUTRAL) ) {

		rc->mode = MODE_3;
	} else if( RC_CHANNEL_6_INPUT_CAPTURE < (- RC_CHANNEL_6_HALF_SIZE + RC_CHANNEL_6_NEUTRAL) ){

		rc->mode = MODE_1;
	} else{

		rc->mode = MODE_2;

	}




	bound_float(rc->roll_control_input, ROLL_CONTROL_MIN, ROLL_CONTROL_MAX);
	bound_float(rc->pitch_control_input, PITCH_CONTROL_MIN, PITCH_CONTROL_MAX);
	bound_float(rc->throttle_control_input, THROTTLE_CONTROL_MIN, THROTTLE_CONTROL_MAX);
	bound_float(rc->yaw_rate_control_input, YAW_RATE_CONTROL_MIN, YAW_RATE_CONTROL_MAX);

}