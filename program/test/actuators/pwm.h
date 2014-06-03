#ifndef FILE_PWM_H
#define FILE_PWM_H
#include <stdint.h>

#define MOTOR_PWM_MAX_PULSE 20000
#define MOTOR_PWM_MIN_PULSE 10000

#define MOTOR1_PWM_PULSE TIM1->CCR1
#define MOTOR2_PWM_PULSE TIM1->CCR2
#define MOTOR3_PWM_PULSE TIM1->CCR3
#define MOTOR4_PWM_PULSE TIM1->CCR4
#define MOTOR5_PWM_PULSE TIM4->CCR1
#define MOTOR6_PWM_PULSE TIM4->CCR2
#define MOTOR7_PWM_PULSE TIM4->CCR3
#define MOTOR8_PWM_PULSE TIM4->CCR4
#define MOTOR9_PWM_PULSE TIM3->CCR1
#define MOTOR10_PWM_PULSE TIM3->CCR2
#define MOTOR11_PWM_PULSE TIM3->CCR3
#define MOTOR12_PWM_PULSE TIM3->CCR4
#define CONTROL_OUTPUT_SCALE 100.0f
typedef struct motor_output_t {
	float m1;
	float m2;
	float m3;
	float m4;
	float m5;
	float m6;
	float m7;
	float m8;
	float m9;
	float m10;
	float m11;
	float m12;

} motor_output_t;


void init_pwm_motor(void);
void set_pwm_motor(motor_output_t*);
void test_pwm_motor(void);
void calibrate_ESC(void);
#endif