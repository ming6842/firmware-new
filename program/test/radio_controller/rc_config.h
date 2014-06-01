#ifndef FILE_RC_CONFIG_H
#define FILE_RC_CONGIG_H
#define USE_FUTABA
#ifdef USE_FUTABA

#define RC_CHANNEL_1_MAX 28986
#define RC_CHANNEL_1_NEUTRAL 22810
#define RC_CHANNEL_1_MIN 16606

#define RC_CHANNEL_2_MAX 29010
#define RC_CHANNEL_2_NEUTRAL 22797
#define RC_CHANNEL_2_MIN 16630

#define RC_CHANNEL_3_MAX 29005
#define RC_CHANNEL_3_NEUTRAL 16626

#define RC_CHANNEL_4_MAX 29002
#define RC_CHANNEL_4_NEUTRAL 22761
#define RC_CHANNEL_4_MIN 16624
//SAFE
#define RC_CHANNEL_5_MAX 31122
#define RC_CHANNEL_5_NEUTRAL 14500
#define RC_CHANNEL_5_HALF_SIZE  ((float)RC_CHANNEL_5_MAX - RC_CHANNEL_5_NEUTRAL )/ 2.0f
//auto-pilot
#define RC_CHANNEL_6_MAX 31118 //lower 
#define RC_CHANNEL_6_NEUTRAL 22812 //mid
#define RC_CHANNEL_6_MIN 14490 // higher

#endif 

typedef enum {
	ENGINE_ON = 0,
	ENGINE_OFF
}SAFETY_CHANNEL;

typedef enum {
	MODE_1= 0,
	MODE_2,
	MODE_3
}MODE_CHANNEL;

typedef struct radio_controller{

	volatile float roll_control_input;
	volatile float pitch_control_input;
	volatile float throttle_control_input;
	volatile float yaw_rate_control_input;
	volatile SAFETY_CHANNEL safety;
	volatile MODE_CHANNEL mode;

} radio_controller_t;

#endif