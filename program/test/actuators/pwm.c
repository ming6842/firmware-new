#include "pwm.h"
#include "stm32f4xx_conf.h"
#include "bound.h"
#include "delay.h"

static motor_output_t motor;

void init_pwm_motor(void)
{
	motor.m1= 0.0f;
	motor.m2= 0.0f; 
	motor.m3= 0.0f;
	motor.m4= 0.0f; 
	motor.m5= 0.0f;
	motor.m6= 0.0f; 
	motor.m7= 0.0f;
	motor.m8= 0.0f; 
	motor.m9= 0.0f;
	motor.m10= 0.0f; 
	motor.m11 = 0.0f;
	motor.m12 = 0.0f; 

	set_pwm_motor(&motor);


}
void set_pwm_motor(motor_output_t* m)
{
	uint16_t ccr[12];
	motor.m1= bound_float(m->m1, 0.0f, 100.0f);
	motor.m2= bound_float(m->m2, 0.0f, 100.0f);
	motor.m3= bound_float(m->m3, 0.0f, 100.0f);
	motor.m4= bound_float(m->m4, 0.0f, 100.0f);
	motor.m5= bound_float(m->m5, 0.0f, 100.0f);
	motor.m6= bound_float(m->m6, 0.0f, 100.0f);
	motor.m7= bound_float(m->m7, 0.0f, 100.0f);
	motor.m8= bound_float(m->m8, 0.0f, 100.0f);
	motor.m9= bound_float(m->m9, 0.0f, 100.0f);
	motor.m10 = bound_float(m->m10, 0.0f, 100.0f);
	motor.m11 = bound_float(m->m11, 0.0f, 100.0f);
	motor.m12 = bound_float(m->m12, 0.0f, 100.0f);

	ccr[0]= (uint16_t)((motor.m1*CONTROL_OUTPUT_SCALE) + MOTOR_PWM_MIN_PULSE );
	ccr[1]= (uint16_t)((motor.m2*CONTROL_OUTPUT_SCALE) + MOTOR_PWM_MIN_PULSE );
	ccr[2]= (uint16_t)((motor.m3*CONTROL_OUTPUT_SCALE) + MOTOR_PWM_MIN_PULSE);
	ccr[3]= (uint16_t)((motor.m4*CONTROL_OUTPUT_SCALE) + MOTOR_PWM_MIN_PULSE);
	ccr[4]= (uint16_t)((motor.m5*CONTROL_OUTPUT_SCALE) + MOTOR_PWM_MIN_PULSE);
	ccr[5]= (uint16_t)((motor.m6*CONTROL_OUTPUT_SCALE) + MOTOR_PWM_MIN_PULSE);
	ccr[6]= (uint16_t)((motor.m7*CONTROL_OUTPUT_SCALE) + MOTOR_PWM_MIN_PULSE);
	ccr[7]= (uint16_t)((motor.m8*CONTROL_OUTPUT_SCALE) + MOTOR_PWM_MIN_PULSE);
	ccr[8]= (uint16_t)((motor.m9*CONTROL_OUTPUT_SCALE) + MOTOR_PWM_MIN_PULSE);
	ccr[9]= (uint16_t)((motor.m10*CONTROL_OUTPUT_SCALE)+ MOTOR_PWM_MIN_PULSE);
	ccr[10]= (uint16_t)((motor.m11*CONTROL_OUTPUT_SCALE)+ MOTOR_PWM_MIN_PULSE);
	ccr[11]= (uint16_t)((motor.m12*CONTROL_OUTPUT_SCALE)+ MOTOR_PWM_MIN_PULSE);
	MOTOR1_PWM_PULSE = bound_uint16(ccr[0], MOTOR_PWM_MIN_PULSE, MOTOR_PWM_MAX_PULSE);
	MOTOR2_PWM_PULSE = bound_uint16(ccr[1], MOTOR_PWM_MIN_PULSE, MOTOR_PWM_MAX_PULSE);
	MOTOR3_PWM_PULSE = bound_uint16(ccr[2], MOTOR_PWM_MIN_PULSE, MOTOR_PWM_MAX_PULSE);
	MOTOR4_PWM_PULSE = bound_uint16(ccr[3], MOTOR_PWM_MIN_PULSE, MOTOR_PWM_MAX_PULSE);
	MOTOR5_PWM_PULSE = bound_uint16(ccr[4], MOTOR_PWM_MIN_PULSE, MOTOR_PWM_MAX_PULSE);
	MOTOR6_PWM_PULSE = bound_uint16(ccr[5], MOTOR_PWM_MIN_PULSE, MOTOR_PWM_MAX_PULSE);
	MOTOR7_PWM_PULSE = bound_uint16(ccr[6], MOTOR_PWM_MIN_PULSE, MOTOR_PWM_MAX_PULSE);
	MOTOR8_PWM_PULSE = bound_uint16(ccr[7], MOTOR_PWM_MIN_PULSE, MOTOR_PWM_MAX_PULSE);
	MOTOR9_PWM_PULSE = bound_uint16(ccr[8], MOTOR_PWM_MIN_PULSE, MOTOR_PWM_MAX_PULSE);
	MOTOR10_PWM_PULSE = bound_uint16(ccr[9], MOTOR_PWM_MIN_PULSE, MOTOR_PWM_MAX_PULSE);
	MOTOR11_PWM_PULSE = bound_uint16(ccr[10], MOTOR_PWM_MIN_PULSE, MOTOR_PWM_MAX_PULSE);
	MOTOR12_PWM_PULSE = bound_uint16(ccr[11], MOTOR_PWM_MIN_PULSE, MOTOR_PWM_MAX_PULSE);
}
void test_pwm_motor()
{
	// motor[0].pulse_width =MOTOR_PWM_MIN_PULSE;
	// motor[1].pulse_width =MOTOR_PWM_MIN_PULSE;
	// motor[2].pulse_width =MOTOR_PWM_MIN_PULSE;
	// motor[3].pulse_width =MOTOR_PWM_MIN_PULSE;
	// motor[4].pulse_width =MOTOR_PWM_MIN_PULSE;
	// motor[5].pulse_width =MOTOR_PWM_MIN_PULSE;
	// motor[6].pulse_width =MOTOR_PWM_MIN_PULSE;
	// motor[7].pulse_width =MOTOR_PWM_MIN_PULSE;
	// motor[8].pulse_width =MOTOR_PWM_MIN_PULSE;
	// motor[9].pulse_width =MOTOR_PWM_MIN_PULSE;
	// motor[10].pulse_width =MOTOR_PWM_MIN_PULSE;
	// motor[11].pulse_width =MOTOR_PWM_MIN_PULSE;
	// set_pwm_motor(&motor[0], &motor[1], &motor[2], &motor[3],
	// 	&motor[4],&motor[5],&motor[6],&motor[7],
	// 	&motor[8],&motor[9],&motor[10],&motor[11]);
	// Delay_1us(1000);
	// motor[0].pulse_width +=100;
	// motor[1].pulse_width +=100;
	// motor[2].pulse_width +=100;
	// motor[3].pulse_width +=100;
	// motor[4].pulse_width +=100;
	// motor[5].pulse_width +=100;
	// motor[6].pulse_width +=100;
	// motor[7].pulse_width +=100;
	// motor[8].pulse_width +=100;
	// motor[9].pulse_width +=100;
	// motor[10].pulse_width +=100;
	// motor[11].pulse_width +=100;
	// set_pwm_motor(&motor[0], &motor[1], &motor[2], &motor[3],
	// 	&motor[4],&motor[5],&motor[6],&motor[7],
	// 	&motor[8],&motor[9],&motor[10],&motor[11]);
	// Delay_1us(1000);
	// motor[0].pulse_width +=100;
	// motor[1].pulse_width +=100;
	// motor[2].pulse_width +=100;
	// motor[3].pulse_width +=100;
	// motor[4].pulse_width +=100;
	// motor[5].pulse_width +=100;
	// motor[6].pulse_width +=100;
	// motor[7].pulse_width +=100;
	// motor[8].pulse_width +=100;
	// motor[9].pulse_width +=100;
	// motor[10].pulse_width +=100;
	// motor[11].pulse_width +=100;
	// set_pwm_motor(&motor[0], &motor[1], &motor[2], &motor[3],
	// 	&motor[4],&motor[5],&motor[6],&motor[7],
	// 	&motor[8],&motor[9],&motor[10],&motor[11]);
	// Delay_1us(1000);
	// motor[0].pulse_width +=100;
	// motor[1].pulse_width +=100;
	// motor[2].pulse_width +=100;
	// motor[3].pulse_width +=100;
	// motor[4].pulse_width +=100;
	// motor[5].pulse_width +=100;
	// motor[6].pulse_width +=100;
	// motor[7].pulse_width +=100;
	// motor[8].pulse_width +=100;
	// motor[9].pulse_width +=100;
	// motor[10].pulse_width +=100;
	// motor[11].pulse_width +=100;
	// set_pwm_motor(&motor[0], &motor[1], &motor[2], &motor[3],
	// 	&motor[4],&motor[5],&motor[6],&motor[7],
	// 	&motor[8],&motor[9],&motor[10],&motor[11]);
	// Delay_1us(1000);
}