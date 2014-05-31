#include "pwm.h"
#include "stm32f4xx_conf.h"
#define MOTOR_PWM_MAX_PULSE 4000
#define MOTOR_PWM_MIN_PULSE 2000
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
#define MOTOR12_PWM_PULSE TIM3->CCR24



void init_pwm_motor(void)
{
	motor1.pulse_width = MOTOR_PWM_MIN_PULSE; 
	motor2.pulse_width = MOTOR_PWM_MIN_PULSE;
	motor3.pulse_width = MOTOR_PWM_MIN_PULSE; 
	motor4.pulse_width = MOTOR_PWM_MIN_PULSE;

}
void set_pwm_motor(pwm_motor_t * m1, pwm_motor_t * m2, pwm_motor_t * m3, pwm_motor_t * m4)
{
	Bound(m1->pulse_width, MOTOR_PWM_MIN_PULSE, MOTOR_PWM_MAX_PULSE);
	Bound(m2->pulse_width, MOTOR_PWM_MIN_PULSE, MOTOR_PWM_MAX_PULSE);
	Bound(m3->pulse_width, MOTOR_PWM_MIN_PULSE, MOTOR_PWM_MAX_PULSE);
	Bound(m4->pulse_width, MOTOR_PWM_MIN_PULSE, MOTOR_PWM_MAX_PULSE);
	MOTOR1_PWM_PULSE = m1->pulse_width;
	MOTOR2_PWM_PULSE = m2->pulse_width;
	MOTOR3_PWM_PULSE = m3->pulse_width;
	MOTOR4_PWM_PULSE = m4->pulse_width;
}