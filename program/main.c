//#define DEBUG
#include "stm32f4xx_conf.h"
#include "../common/delay.h"
#include "gpio.h"
#include "led.h"
#include "i2c.h"
#include "usart.h"
#include "spi.h"
#include "tim.h"
#include <stdio.h>
#include "attitude_estimator.h"
#include "vertical_estimator.h"
#include "attitude_stabilizer.h"
#include "pwm.h"
#include "radio_control.h"
#include "test_common.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"

#include "global.h"
#include "communication.h"

attitude_stablizer_pid_t pid_roll_info;
attitude_stablizer_pid_t pid_pitch_info;
attitude_stablizer_pid_t pid_yaw_info;

extern uint8_t estimator_trigger_flag;

/* FreeRTOS */
extern xSemaphoreHandle serial_tx_wait_sem;
extern xQueueHandle serial_rx_queue;

xTimerHandle xTimers[1];

void vApplicationStackOverflowHook( xTaskHandle xTask, signed char *pcTaskName );
void vApplicationIdleHook(void);
void vApplicationMallocFailedHook(void);

void gpio_rcc_init(void);
void gpio_rcc_init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | 
		RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE,  ENABLE);	
}

void vApplicationStackOverflowHook( xTaskHandle xTask, signed char *pcTaskName )
{
	while(1);

}
void vApplicationIdleHook(void)
{
	
}
void vApplicationMallocFailedHook(void)
{
	while(1);
}

void flight_control_task(void)
{
	uint8_t buffer[100];
	imu_unscaled_data_t imu_unscaled_data;
	imu_data_t imu_raw_data;
	imu_data_t imu_filtered_data;
	imu_calibrated_offset_t imu_offset;
	attitude_t attitude;
	vector3d_t lowpassed_acc_data;
	vector3d_t predicted_g_data;

	vertical_data vertical_raw_data;
	vertical_data vertical_filtered_data;
	radio_controller_t my_rc;

	/* Set the PID gain */
	set_global_data_float(ROLL_KP, 2.20);
	set_global_data_float(ROLL_KD, 2.27);
	set_global_data_float(ROLL_KI, 0.0);
	pid_roll_info.setpoint =0.0;

	set_global_data_float(PITCH_KP, 0.20);
	set_global_data_float(PITCH_KD, 0.07);
	set_global_data_float(PITCH_KI, 0.0);
	pid_pitch_info.setpoint =0.0;

	set_global_data_float(YAW_KP, 0.0);
	set_global_data_float(YAW_KD, 1.7);
	set_global_data_float(YAW_KI, 0.0);
	pid_yaw_info.setpoint =0.0;

	attitude_estimator_init(&attitude,&imu_raw_data, &imu_filtered_data,&predicted_g_data);
	vertical_estimator_init(&vertical_raw_data,&vertical_filtered_data);

	//Delay_1us(2000000);
	imu_initialize(&imu_offset,30000);

	check_rc_safety_init(&my_rc);

 	//barometer_initialize();

	portTickType xLastWakeTime;
	const portTickType xFrequency = 20;


	// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();

	while(1){

	//vTaskSuspendAll();

	//taskENTER_CRITICAL();

	attitude_update(&attitude, &lowpassed_acc_data, &predicted_g_data,
		&imu_unscaled_data, &imu_raw_data, &imu_offset);

	vertical_sense(&vertical_filtered_data,&vertical_raw_data,&attitude, &imu_raw_data);
	PID_attitude_roll(&pid_roll_info,&imu_raw_data,&attitude);
	LED_TOGGLE(LED2);
	PID_attitude_pitch(&pid_pitch_info,&imu_raw_data,&attitude);
	PID_attitude_yaw(&pid_yaw_info,&imu_raw_data,&attitude);
	PID_output(&my_rc,&pid_roll_info,&pid_pitch_info,&pid_yaw_info);
	update_radio_control_input(&my_rc);
	//LED_OFF(LED2);
	PID_attitude_rc_pass_command(&pid_roll_info,&pid_pitch_info,&pid_yaw_info,&my_rc);

	//taskEXIT_CRITICAL();
	//xTaskResumeAll();

	vTaskDelayUntil( &xLastWakeTime, xFrequency );

#ifdef DEBUG
		test_bound();
#endif
	}
}

#define BOOT_TIME_TIMER 0
static uint32_t counter = 0;
void boot_time_timer(void)
{
	counter++;
	set_global_data_int(BOOT_TIME, counter);
}

int main(void)
{
	vSemaphoreCreateBinary(serial_tx_wait_sem);
	serial_rx_queue = xQueueCreate(1, sizeof(serial_msg));

	/* Global data initialazition */
	init_global_data();

	/* Hardware initialization */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	gpio_rcc_init();
	led_init();
	usart_init();
	spi_init();
	pwm_input_output_init();
	init_pwm_motor();
	i2c_Init();
	usart2_dma_init();


	cycle_led(5);

	/* Register the FreeRTOS task */
	/* Flight control task */
	xTaskCreate(
		(pdTASK_CODE)flight_control_task,
		(signed portCHAR*)"flight control task",
		2048,
		NULL,
		tskIDLE_PRIORITY + 9,
		NULL
	);

	/* Ground station communication task */	
        xTaskCreate(
		(pdTASK_CODE)ground_station_task,
		(signed portCHAR *)"ground station send task",
		2048,
		NULL,
		tskIDLE_PRIORITY + 5,
		NULL
	);

	xTaskCreate(
		(pdTASK_CODE)mavlink_receiver_task,
		(signed portCHAR *) "ground station receive task",
		2048,
		NULL,
		tskIDLE_PRIORITY + 6, NULL
	);

	/* Timer */
	xTimers[BOOT_TIME_TIMER] = xTimerCreate(
		    (signed portCHAR *) "boot time",
		    configTICK_RATE_HZ,
		    pdTRUE,
		    BOOT_TIME_TIMER,
		    (tmrTIMER_CALLBACK)boot_time_timer
	);

	xTimerStart(xTimers[BOOT_TIME_TIMER], 0);
	vTaskStartScheduler();

	return 0;
}

