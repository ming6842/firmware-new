#ifndef FILE_RC_CONFIG_H
#define FILE_RC_CONGIG_H

#ifdef USE_FUTABA

#define RC_CHANNEL_1_MAX 28980
#define RC_CHANNEL_1_NEUTRAL 22839
#define RC_CHANNEL_1_MIN 16651

#define RC_CHANNEL_2_MAX 28980
#define RC_CHANNEL_2_NEUTRAL 22747
#define RC_CHANNEL_2_MIN 16665

#define RC_CHANNEL_3_MAX 28980
#define RC_CHANNEL_3_NEUTRAL 16665

#define RC_CHANNEL_4_MAX 28980
#define RC_CHANNEL_4_NEUTRAL 22825
#define RC_CHANNEL_4_MIN 16651
//SAFE
#define RC_CHANNEL_5_MAX 31082
#define RC_CHANNEL_5_NEUTRAL 14563
#define RC_CHANNEL_5_HALF_SIZE  ((float)RC_CHANNEL_5_MAX - RC_CHANNEL_5_NEUTRAL )/ 2.0f
//auto-pilot
#define RC_CHANNEL_6_MAX 31083 //lower 
#define RC_CHANNEL_6_NEUTRAL 22825 //mid
#define RC_CHANNEL_6_MIN 14563 // higher
#define RC_CHANNEL_6_HALF_SIZE  ((float)RC_CHANNEL_6_MAX - RC_CHANNEL_6_NEUTRAL )/ 2.0f

#endif 
/*set the radio controller maximum and minimun control input (in degrees)*/
#define ROLL_CONTROL_MAX  60
#define ROLL_CONTROL_MIN  -60
#define PITCH_CONTROL_MAX 60
#define PITCH_CONTROL_MIN -60
#define YAW_RATE_CONTROL_MAX 45
#define YAW_RATE_CONTROL_MIN -45
#define THROTTLE_CONTROL_MAX 100.0f
#define THROTTLE_CONTROL_MIN 0.0f
/*RC receiver channel corresponds to timer input captuer*/
#define RC_CHANNEL_1_INPUT_CAPTURE ((float)inc[INC1].curr_value)
#define RC_CHANNEL_2_INPUT_CAPTURE ((float)inc[INC2].curr_value)
#define RC_CHANNEL_3_INPUT_CAPTURE ((float)inc[INC3].curr_value)
#define RC_CHANNEL_4_INPUT_CAPTURE ((float)inc[INC4].curr_value)
#define RC_CHANNEL_5_INPUT_CAPTURE ((float)inc[INC5].curr_value) 
#define RC_CHANNEL_6_INPUT_CAPTURE ((float)inc[INC6].curr_value)

#endif
