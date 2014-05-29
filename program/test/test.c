#include "QuadCopterConfig.h"
#include "test.h"
/*

*/
static void test_Update_RC_Control()
{

	int16_t Roll, Pitch, Yaw, Thr; 
	uint8_t safety;
	
	//test the condition out of maximum value
	system.variable[PWM1_CCR].value = MAX_PWM1_INPUT + 200;
	system.variable[PWM2_CCR].value = MAX_PWM2_INPUT + 200;
	system.variable[PWM3_CCR].value = MAX_PWM3_INPUT + 200;
	system.variable[PWM4_CCR].value = MAX_PWM4_INPUT + 200;
	system.variable[PWM5_CCR].value = MAX_PWM5_INPUT + 200;

	Update_RC_Control( &Roll, &Pitch, &Yaw, &Thr, &safety);
	
	TEST_ASSERT(Roll >= MAX_CTRL_ROLL);
	TEST_ASSERT(Pitch >= MAX_CTRL_PITCH);
	TEST_ASSERT(Yaw >= MAX_CTRL_YAW);
	TEST_ASSERT(Thr == PWM_MOTOR_MAX);
	TEST_ASSERT(safety == ENGINE_OFF);

	//test the condition in normal 
	system.variable[PWM1_CCR].value = MAX_PWM1_INPUT - 200;
	system.variable[PWM2_CCR].value = MAX_PWM2_INPUT - 200;
	system.variable[PWM3_CCR].value = MAX_PWM3_INPUT - 200;
	system.variable[PWM4_CCR].value = MAX_PWM4_INPUT - 200;
	system.variable[PWM5_CCR].value = MAX_PWM5_INPUT - 200;

	Update_RC_Control( &Roll, &Pitch, &Yaw, &Thr, &safety);
	TEST_ASSERT( ( Roll >= MIN_CTRL_ROLL) && (Roll <= MAX_CTRL_ROLL) );
	TEST_ASSERT( ( Pitch >= MIN_CTRL_PITCH) && (Pitch <= MAX_CTRL_PITCH) );
	TEST_ASSERT( ( Yaw >= MIN_CTRL_YAW) && (Yaw <= MAX_CTRL_YAW) );
	TEST_ASSERT( ( Thr >= PWM_MOTOR_MIN) && (Thr <= PWM_MOTOR_MAX) );
	TEST_ASSERT( safety == ENGINE_OFF );

	////test the condition: close to minimun value
	system.variable[PWM1_CCR].value = MIN_PWM1_INPUT + 200;
	system.variable[PWM2_CCR].value = MIN_PWM2_INPUT + 200;
	system.variable[PWM3_CCR].value = MIN_PWM3_INPUT + 200;
	system.variable[PWM4_CCR].value = MIN_PWM4_INPUT + 200;
	system.variable[PWM5_CCR].value = MIN_PWM5_INPUT + 200;

	Update_RC_Control( &Roll, &Pitch, &Yaw, &Thr, &safety);
	TEST_ASSERT( ( Roll >= MIN_CTRL_ROLL) && (Roll <= MAX_CTRL_ROLL) );
	TEST_ASSERT( ( Pitch >= MIN_CTRL_PITCH) && (Pitch <= MAX_CTRL_PITCH) );
	TEST_ASSERT( ( Yaw >= MIN_CTRL_YAW) && (Yaw <= MAX_CTRL_YAW) );
	TEST_ASSERT( ( Thr >= PWM_MOTOR_MIN) && (Thr <= PWM_MOTOR_MAX) );
	TEST_ASSERT( safety == ENGINE_ON );

	////test the condition: lower to minimun value
	system.variable[PWM1_CCR].value = MIN_PWM1_INPUT - 200;
	system.variable[PWM2_CCR].value = MIN_PWM2_INPUT - 200;
	system.variable[PWM3_CCR].value = MIN_PWM3_INPUT - 200;
	system.variable[PWM4_CCR].value = MIN_PWM4_INPUT - 200;
	system.variable[PWM5_CCR].value = MIN_PWM5_INPUT - 200;

	Update_RC_Control( &Roll, &Pitch, &Yaw, &Thr, &safety);
	TEST_ASSERT( Roll <= MIN_CTRL_ROLL );
	TEST_ASSERT( Pitch <= MIN_CTRL_PITCH );
	TEST_ASSERT( Yaw <= MIN_CTRL_YAW );
	TEST_ASSERT( Thr == PWM_MOTOR_MIN );
	TEST_ASSERT( safety == ENGINE_ON );
	test_pass_count++;
}
void run_tests()
{
	test_Update_RC_Control();
	printf("\r\nPass:%d,Failed:%d\r\n\r\n", test_pass_count, test_failed_count);
}

void delay(uint32_t count){

	while(count--);

}
int main()
{	
	led_init();

	while(1){

		delay(1000000);
		GPIO_ToggleBits(GPIOE,GPIO_Pin_8 | GPIO_Pin_10 | GPIO_Pin_12 | GPIO_Pin_15);

	}

}