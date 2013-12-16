/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "QCopterFC.h"
#include "QCopterFC_ctrl.h"
#include "QCopterFC_ahrs.h"
#include "QCopterFC_transport.h"
#include "module_board.h"
#include "module_rs232.h"
#include "module_motor.h"
#include "module_sensor.h"
#include "module_nrf24l01.h"
#include "sys_manager.h"
#include "test.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
/*=====================================================================================================*/
#define PRINT_DEBUG(var1) printf("DEBUG PRINT"#var1"\r\n")
/*=====================================================================================================*/
void vApplicationTickHook( void )
{
}


void vApplicationStackOverflowHook( xTaskHandle pxTask, signed char *pcTaskName )
{
}

void vApplicationIdleHook( void )
{
}
void System_Init(void)
{
	SystemInit();

	LED_Config();
	KEY_Config();
	RS232_Config();
	Motor_Config();
	PWM_Capture_Config();
	Sensor_Config();
	nRF24L01_Config();

	PID_Init(&PID_Yaw);
	PID_Init(&PID_Roll);
	PID_Init(&PID_Pitch);

	PID_Pitch.Kp = +1.5f;
	PID_Pitch.Ki = +0.002f;
	PID_Pitch.Kd = +1.0f;

	PID_Roll.Kp  = +1.5f;
	PID_Roll.Ki  = +0.002f;
	PID_Roll.Kd  = +1.0f;

	PID_Yaw.Kp   = +0.0f;
	PID_Yaw.Ki   = +0.0f;
	PID_Yaw.Kd   = +0.0f;

	Delay_10ms(2);
}
/*=====================================================================================================*/

void nrf()
{
} 

void flightControl_Task()
{
	u8 Sta = ERROR;

	/* Throttle Config */
	if (KEY == 1) {
		LED_B = 0;
		Motor_Control(PWM_MOTOR_MAX, PWM_MOTOR_MAX, PWM_MOTOR_MAX, PWM_MOTOR_MAX);
	}

	while (KEY == 1);

	LED_B = 1;
	Motor_Control(PWM_MOTOR_MIN, PWM_MOTOR_MIN, PWM_MOTOR_MIN, PWM_MOTOR_MIN);

	/* nRF Check */
	while (Sta == ERROR)
		Sta = nRF_Check();

	/* Sensor Init */
	if (Sensor_Init() == SUCCESS)
		LED_G = 0;

	Delay_10ms(10);

	/* Systick Config */
	if (SysTick_Config(SystemCoreClock / SampleRateFreg)) { // SampleRateFreg = 500 Hz
		while (1);
	}

	/* Wait Correction */
	while (SensorMode != Mode_Algorithm);

	/* Lock */
	LED_R = 1;
	LED_G = 1;
	LED_B = 1;

	while (!KEY) {
		LED_B = ~LED_B;
		Delay_10ms(1);
		Transport_Send(TxBuf[0]);
		if ( global_var[NO_RC_SIGNAL_MSG].param == 1 ) {

			printf("!WARNING: NO SIGNAL!");

		} else {
			printf("Roll%f,Pitch%f,Yaw%f,CH1 %f(%f),CH2 %f(%f),CH3 %f(%f),CH4 %f(%f),CH5 %f()\r\n",
			       AngE.Roll, AngE.Pitch, AngE.Yaw,
			       global_var[PWM1_CCR].param, global_var[RC_EXP_ROLL].param,
			       global_var[PWM2_CCR].param, global_var[RC_EXP_PITCH].param,
			       global_var[PWM3_CCR].param, global_var[RC_EXP_THR].param,
			       global_var[PWM4_CCR].param, global_var[RC_EXP_YAW].param,
			       global_var[PWM5_CCR].param);
		}
		//PRINT_DEBUG(global_var[PWM5_CCR].param);
	}

	LED_B = 1;

	while(1);
}

/*=====================================================================================================*/
int main(void)
{

	/* System Init */
	System_Init();
	test_printf();
	PRINT_DEBUG(555);

	xTaskCreate(flightControl_Task,
		(signed portCHAR *) "Flight Control",
		512, NULL,
		tskIDLE_PRIORITY + 5, NULL);

	vTaskStartScheduler();

	return 0;
}
/*=====================================================================================================*/
/*=====================================================================================================*/
