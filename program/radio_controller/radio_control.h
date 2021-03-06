#ifndef FILE_RADIO_CONTROL_H
#define FILE_RADIO_CONTROL_H
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

void update_radio_control_input(radio_controller_t *);
void check_rc_safety_init(radio_controller_t * );
#endif