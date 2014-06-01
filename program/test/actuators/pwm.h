#ifndef FILE_PWM_H
#define FILE_PWM_H
#include <stdint.h>

typedef struct pwm_motor {
	uint16_t pulse_width;

} pwm_motor_t;


pwm_motor_t motor[12];

void init_pwm_motor(void);
void set_pwm_motor(pwm_motor_t* , pwm_motor_t* , pwm_motor_t* , pwm_motor_t* ,
	pwm_motor_t* , pwm_motor_t*, pwm_motor_t* , pwm_motor_t * ,
	pwm_motor_t* , pwm_motor_t*, pwm_motor_t* , pwm_motor_t * );
void test_pwm_motor(void);
#endif