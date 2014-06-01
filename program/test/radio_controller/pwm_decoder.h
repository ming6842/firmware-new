#ifndef FILE_PWM_DECODER_H
#define FILE_PWM_DECODER_H
#include "input_capture.h"
#include "rc_config.h"
#include "pwm.h"
#include "radio_control.h"
#define ROLL_CONTROL_MAX  60
#define ROLL_CONTROL_MIN  -60
#define PITCH_CONTROL_MAX 60
#define PITCH_CONTROL_MIN -60
#define YAW_RATE_CONTROL_MAX 45
#define YAW_RATE_CONTROL_MIN -45
#define THROTTLE_CONTROL_MAX MOTOR_PWM_MAX_PULSE
#define THROTTLE_CONTROL_MIN MOTOR_PWM_MIN_PULSE

#define ROLL_CONTROL_STEP_SIZE  ( (float)ROLL_CONTROL_MAX /(float)(RC_CHANNEL_1_MAX- RC_CHANNEL_1_NEUTRAL) )
#pragma message _ROLL_CONTROL_STEP_SIZE_ "value"
#define PITCH_CONTROL_STEP_SIZE  ( (float)PITCH_CONTROL_MAX / (float)(RC_CHANNEL_2_MAX- RC_CHANNEL_2_NEUTRAL) )
#define THROTTLE_CONTROL_STEP_SIZE \
( ( (float)THROTTLE_CONTROL_MAX - (float)THROTTLE_CONTROL_MIN )/ (float) (RC_CHANNEL_3_MAX- RC_CHANNEL_3_NEUTRAL) )

#define YAW_RATE_CONTROL_STEP_SIZE  ( (float)YAW_RATE_CONTROL_MAX / (float)(RC_CHANNEL_4_MAX- RC_CHANNEL_4_NEUTRAL) )


#define RC_CHANNEL_1_INPUT_CAPTURE ((float)inc[INC6].curr_value)
#define RC_CHANNEL_2_INPUT_CAPTURE ((float)inc[INC5].curr_value)
#define RC_CHANNEL_3_INPUT_CAPTURE ((float)inc[INC4].curr_value)
#define RC_CHANNEL_4_INPUT_CAPTURE ((float)inc[INC3].curr_value)
#define RC_CHANNEL_5_INPUT_CAPTURE ((float)inc[INC2].curr_value) 
#define RC_CHANNEL_6_INPUT_CAPTURE ((float)inc[INC1].curr_value)
void get_pwm_decode_value(radio_controller_t* );
#endif 