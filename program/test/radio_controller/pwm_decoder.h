#ifndef FILE_PWM_DECODER_H
#define FILE_PWM_DECODER_H
#include "input_capture.h"
#include "rc_config.h"
#include "pwm.h"
#include "radio_control.h"


#define ROLL_CONTROL_STEP_SIZE  ( (float)ROLL_CONTROL_MAX /(float)(RC_CHANNEL_1_MAX- RC_CHANNEL_1_NEUTRAL) )
#define PITCH_CONTROL_STEP_SIZE  ( (float)PITCH_CONTROL_MAX / (float)(RC_CHANNEL_2_MAX- RC_CHANNEL_2_NEUTRAL) )
#define THROTTLE_CONTROL_STEP_SIZE \
( ( (float)THROTTLE_CONTROL_MAX - (float)THROTTLE_CONTROL_MIN )/ (float) (RC_CHANNEL_3_MAX- RC_CHANNEL_3_NEUTRAL) )

#define YAW_RATE_CONTROL_STEP_SIZE  ( (float)YAW_RATE_CONTROL_MAX / (float)(RC_CHANNEL_4_MAX- RC_CHANNEL_4_NEUTRAL) )



void get_pwm_decode_value(radio_controller_t* );
#endif 