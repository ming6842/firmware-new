#include "pwm.h"
#include "stm32f4xx_conf.h"
#include "bound.h"
#include "delay.h"


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



void init_pwm_motor(void)
{
	motor[0].pulse_width = MOTOR_PWM_MIN_PULSE; 
	motor[2].pulse_width = MOTOR_PWM_MIN_PULSE;
	motor[3].pulse_width = MOTOR_PWM_MIN_PULSE; 
	motor[4].pulse_width = MOTOR_PWM_MIN_PULSE;
	motor[5].pulse_width = MOTOR_PWM_MIN_PULSE; 
	motor[6].pulse_width = MOTOR_PWM_MIN_PULSE;
	motor[7].pulse_width = MOTOR_PWM_MIN_PULSE; 
	motor[8].pulse_width = MOTOR_PWM_MIN_PULSE;
	motor[9].pulse_width = MOTOR_PWM_MIN_PULSE; 
	motor[10].pulse_width = MOTOR_PWM_MIN_PULSE;
	motor[11].pulse_width = MOTOR_PWM_MIN_PULSE; 
	motor[12].pulse_width = MOTOR_PWM_MIN_PULSE;

}
void set_pwm_motor(pwm_motor_t * m1, pwm_motor_t * m2, pwm_motor_t * m3, pwm_motor_t * m4,
	pwm_motor_t * m5, pwm_motor_t * m6, pwm_motor_t * m7, pwm_motor_t * m8,
	pwm_motor_t * m9, pwm_motor_t * m10, pwm_motor_t * m11, pwm_motor_t * m12)
{
	m1->pulse_width = bound_int32(m1->pulse_width, MOTOR_PWM_MIN_PULSE, MOTOR_PWM_MAX_PULSE);
	m2->pulse_width = bound_int32(m2->pulse_width, MOTOR_PWM_MIN_PULSE, MOTOR_PWM_MAX_PULSE);
	m3->pulse_width = bound_int32(m3->pulse_width, MOTOR_PWM_MIN_PULSE, MOTOR_PWM_MAX_PULSE);
	m4->pulse_width = bound_int32(m4->pulse_width, MOTOR_PWM_MIN_PULSE, MOTOR_PWM_MAX_PULSE);
	m5->pulse_width = bound_int32(m5->pulse_width, MOTOR_PWM_MIN_PULSE, MOTOR_PWM_MAX_PULSE);
	m6->pulse_width = bound_int32(m6->pulse_width, MOTOR_PWM_MIN_PULSE, MOTOR_PWM_MAX_PULSE);
	m7->pulse_width = bound_int32(m7->pulse_width, MOTOR_PWM_MIN_PULSE, MOTOR_PWM_MAX_PULSE);
	m8->pulse_width = bound_int32(m8->pulse_width, MOTOR_PWM_MIN_PULSE, MOTOR_PWM_MAX_PULSE);
	m9->pulse_width = bound_int32(m9->pulse_width, MOTOR_PWM_MIN_PULSE, MOTOR_PWM_MAX_PULSE);
	m10->pulse_width = bound_int32(m10->pulse_width, MOTOR_PWM_MIN_PULSE, MOTOR_PWM_MAX_PULSE);
	m11->pulse_width = bound_int32(m11->pulse_width, MOTOR_PWM_MIN_PULSE, MOTOR_PWM_MAX_PULSE);
	m12->pulse_width = bound_int32(m12->pulse_width, MOTOR_PWM_MIN_PULSE, MOTOR_PWM_MAX_PULSE);
	MOTOR1_PWM_PULSE = m1->pulse_width;
	MOTOR2_PWM_PULSE = m2->pulse_width;
	MOTOR3_PWM_PULSE = m3->pulse_width;
	MOTOR4_PWM_PULSE = m4->pulse_width;
	MOTOR5_PWM_PULSE = m5->pulse_width;
	MOTOR6_PWM_PULSE = m6->pulse_width;
	MOTOR7_PWM_PULSE = m7->pulse_width;
	MOTOR8_PWM_PULSE = m8->pulse_width;
	MOTOR9_PWM_PULSE = m9->pulse_width;
	MOTOR10_PWM_PULSE = m10->pulse_width;
	MOTOR11_PWM_PULSE = m11->pulse_width;
	MOTOR12_PWM_PULSE = m12->pulse_width;
}
void test_pwm_motor()
{
	motor[0].pulse_width =MOTOR_PWM_MIN_PULSE;
	motor[1].pulse_width =MOTOR_PWM_MIN_PULSE;
	motor[2].pulse_width =MOTOR_PWM_MIN_PULSE;
	motor[3].pulse_width =MOTOR_PWM_MIN_PULSE;
	motor[4].pulse_width =MOTOR_PWM_MIN_PULSE;
	motor[5].pulse_width =MOTOR_PWM_MIN_PULSE;
	motor[6].pulse_width =MOTOR_PWM_MIN_PULSE;
	motor[7].pulse_width =MOTOR_PWM_MIN_PULSE;
	motor[8].pulse_width =MOTOR_PWM_MIN_PULSE;
	motor[9].pulse_width =MOTOR_PWM_MIN_PULSE;
	motor[10].pulse_width =MOTOR_PWM_MIN_PULSE;
	motor[11].pulse_width =MOTOR_PWM_MIN_PULSE;
	set_pwm_motor(&motor[0], &motor[1], &motor[2], &motor[3],
		&motor[4],&motor[5],&motor[6],&motor[7],
		&motor[8],&motor[9],&motor[10],&motor[11]);
	Delay_1us(1000);
	motor[0].pulse_width +=100;
	motor[1].pulse_width +=100;
	motor[2].pulse_width +=100;
	motor[3].pulse_width +=100;
	motor[4].pulse_width +=100;
	motor[5].pulse_width +=100;
	motor[6].pulse_width +=100;
	motor[7].pulse_width +=100;
	motor[8].pulse_width +=100;
	motor[9].pulse_width +=100;
	motor[10].pulse_width +=100;
	motor[11].pulse_width +=100;
	set_pwm_motor(&motor[0], &motor[1], &motor[2], &motor[3],
		&motor[4],&motor[5],&motor[6],&motor[7],
		&motor[8],&motor[9],&motor[10],&motor[11]);
	Delay_1us(1000);
	motor[0].pulse_width +=100;
	motor[1].pulse_width +=100;
	motor[2].pulse_width +=100;
	motor[3].pulse_width +=100;
	motor[4].pulse_width +=100;
	motor[5].pulse_width +=100;
	motor[6].pulse_width +=100;
	motor[7].pulse_width +=100;
	motor[8].pulse_width +=100;
	motor[9].pulse_width +=100;
	motor[10].pulse_width +=100;
	motor[11].pulse_width +=100;
	set_pwm_motor(&motor[0], &motor[1], &motor[2], &motor[3],
		&motor[4],&motor[5],&motor[6],&motor[7],
		&motor[8],&motor[9],&motor[10],&motor[11]);
	Delay_1us(1000);
	motor[0].pulse_width +=100;
	motor[1].pulse_width +=100;
	motor[2].pulse_width +=100;
	motor[3].pulse_width +=100;
	motor[4].pulse_width +=100;
	motor[5].pulse_width +=100;
	motor[6].pulse_width +=100;
	motor[7].pulse_width +=100;
	motor[8].pulse_width +=100;
	motor[9].pulse_width +=100;
	motor[10].pulse_width +=100;
	motor[11].pulse_width +=100;
	set_pwm_motor(&motor[0], &motor[1], &motor[2], &motor[3],
		&motor[4],&motor[5],&motor[6],&motor[7],
		&motor[8],&motor[9],&motor[10],&motor[11]);
	Delay_1us(1000);
}