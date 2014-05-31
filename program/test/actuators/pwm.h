#ifndef __PWM_H
#define __PWM_H
#include <stdint.h>
#define Bound(_x, _min, _max) { if (_x > _max) _x = _max; else if (_x < _min) _x = _min; }



typedef struct pwm_motor {
	uint16_t pulse_width;

} pwm_motor_t;

pwm_motor_t motor1,motor2,motor3,motor4;

void init_pwm_motor(void);
void set_pwm_motor(pwm_motor_t* , pwm_motor_t* , pwm_motor_t*, pwm_motor_t*);


#endif