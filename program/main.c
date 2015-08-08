//#define DEBUG
#include "stm32f4xx_conf.h"
#include "../common/delay.h"
#include "gpio.h"
#include "led.h"
#include "i2c.h"
#include "usart.h"
#include "spi.h"
#include "can.h"
#include "tim.h"
#include "flight_controller.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"

#include "global.h"
#include "communication.h"
#include "system_time.h"
#include "lea6h_ubx.h"

#include "dma_tx_stream2.h"

extern uint8_t estimator_trigger_flag;

/* FreeRTOS */
extern xSemaphoreHandle serial_tx_wait_sem;
extern xQueueHandle serial_rx_queue;
extern xQueueHandle gps_serial_queue;
extern xSemaphoreHandle usart3_dma_send_sem;
xTimerHandle xTimers[1];

void vApplicationStackOverflowHook( xTaskHandle xTask, signed char *pcTaskName );
void vApplicationIdleHook(void);
void vApplicationMallocFailedHook(void);
void boot_time_timer(void);
void gpio_rcc_init(void);
void gpio_rcc_init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | 
	RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE,  ENABLE);	
}

void vApplicationStackOverflowHook( xTaskHandle Task __attribute__ ((unused)), signed char *pcTaskName __attribute__ ((unused)))
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

uint8_t buffer1[] = "HelloEveryoneThisIsBuffer1  \r\n";
uint8_t buffer2[] = "OhMyGodICanSwapToTheBuffer2 \r\n";

void dummy_task1(void);
void dummy_task2(void);
void dummy_task3(void);
void dummy_task4(void);

void dummy_task1(void){

 	/* Generate  vTaskDelayUntil parameters */
	portTickType xLastWakeTime;
	const portTickType xFrequency = (uint32_t)12/(1000.0 / configTICK_RATE_HZ);

    // Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount();
    uint8_t text_dummy[100];
	uint8_t error_capture;
	int32_t success=0,skipped=0;
	uint16_t length=0;
    while(1){

    	length = sprintf((char *)text_dummy,"DUMMY1, s = %d, sk = %d \r\n",success,skipped);

    	error_capture = uart2_tx_stream_write(text_dummy,length, DMA_TX_TaskID_DUMMY1,DMA_TX_FH_NoRetry,DMA_TX_TCH_NoWait,30);
		xLastWakeTime = xTaskGetTickCount();
    	LED_OFF(LED1);
		vTaskDelayUntil( &xLastWakeTime, xFrequency );
    	LED_ON(LED1);


    	if(error_capture == DMA_TX_Result_AppendedIntoBuffer){

    		success++;

    	}else{

    		skipped++;

    	}
    	// LED_TOGGLE(LED1);
    } 
}
 
void dummy_task2(void){

 	/* Generate  vTaskDelayUntil parameters */
	portTickType xLastWakeTime;
	const portTickType xFrequency = (uint32_t)14/(1000.0 / configTICK_RATE_HZ);

    // Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount();
    uint8_t text_dummy[100];
	uint8_t error_capture;
	int32_t success=0,skipped=0;
	uint16_t length=0;
    while(1){

    	length = sprintf((char *)text_dummy,"DUMMY2, s = %d, sk = %d \r\n",success,skipped);

    	error_capture = uart2_tx_stream_write(text_dummy,length, DMA_TX_TaskID_DUMMY2,DMA_TX_FH_WaitReadySemaphore,DMA_TX_TCH_NoWait,30);
		xLastWakeTime = xTaskGetTickCount();
    	LED_OFF(LED2);
		vTaskDelayUntil( &xLastWakeTime, xFrequency );
    	LED_ON(LED2);


    	if(error_capture == DMA_TX_Result_AppendedIntoBuffer){

    		success++;

    	}else{

    		skipped++;

    	}
    	// LED_TOGGLE(LED2);
    } 
	
}

void dummy_task3(void){
 	/* Generate  vTaskDelayUntil parameters */
	portTickType xLastWakeTime;
	const portTickType xFrequency = (uint32_t)25/(1000.0 / configTICK_RATE_HZ);

    // Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount();
    uint8_t text_dummy[100];
	uint8_t error_capture;
	int32_t success=0,skipped=0;
	uint16_t length=0;
    while(1){

    	length = sprintf((char *)text_dummy,"DUMMY3, s = %d, sk = %d \r\n",success,skipped);

    	error_capture = uart2_tx_stream_write(text_dummy,length, DMA_TX_TaskID_DUMMY3,DMA_TX_FH_WaitReadySemaphore,DMA_TX_TCH_NoWait,30);
    	xLastWakeTime = xTaskGetTickCount();
    	LED_OFF(LED3);
		vTaskDelayUntil( &xLastWakeTime, xFrequency );
    	LED_ON(LED3);


    	if(error_capture == DMA_TX_Result_AppendedIntoBuffer){

    		success++;

    	}else{

    		skipped++;

    	}
    	// LED_TOGGLE(LED3);
    } 

	
}

void dummy_task4(void){

 	/* Generate  vTaskDelayUntil parameters */
	portTickType xLastWakeTime;
	const portTickType xFrequency = (uint32_t)15/(1000.0 / configTICK_RATE_HZ);

    // Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount();
    uint8_t text_dummy[100];
	uint8_t error_capture;
	int32_t success=0,skipped=0;
	uint16_t length=0;
    while(1){

    	length = sprintf((char *)text_dummy,"DUMMY4, s = %d, sk = %d \r\n",success,skipped);

    	error_capture = uart2_tx_stream_write(text_dummy,length, DMA_TX_TaskID_DUMMY4,DMA_TX_FH_WaitReadySemaphore,DMA_TX_TCH_WaitCompleteSemaphore,30);
    	xLastWakeTime = xTaskGetTickCount();
    	LED_OFF(LED4);
		vTaskDelayUntil( &xLastWakeTime, xFrequency );
    	LED_ON(LED4);


    	if(error_capture == DMA_TX_Result_TransmissionComplete){

    		success++;

    	}else{

    		skipped++;

    	}
    	// LED_TOGGLE(LED4);
    } 
}

void flight_control_dummy_task(void){

 	/* Generate  vTaskDelayUntil parameters */
	portTickType xLastWakeTime;
	const portTickType xFrequency = (uint32_t)1/(1000.0 / configTICK_RATE_HZ);

    // Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount();
    uint8_t text_dummy[100];
	uint8_t error_capture;
	int32_t wait_data=0,transmitting=0,wait_flag=0;
	uint16_t length=0;
	uint32_t report_prescaler=4000;
	uint32_t transmitted_bytes;
	DMATriggerStatus dma_status=0;

    while(1){

    	if(report_prescaler-- == 0){
    		report_prescaler = 1000;


			transmitted_bytes = uartTX_stream_getTransmittedBytes();
	    	length = sprintf((char *)text_dummy,"transmitted_bytes : %ld rate : %ld \r\n",transmitted_bytes,uartTX_stream_getTransmissionRate(1.0f));
	    	uart2_tx_stream_write(text_dummy,length, DMA_TX_TaskID_MAIN,DMA_TX_FH_NoRetry,DMA_TX_TCH_NoWait,30);
		
    	}

		dma_status = uart2_tx_stream_dma_trigger();

		if(dma_status == DMA_TRIGGER_STATUS_WaitingForData){

			wait_data++;

		}else if(dma_status == DMA_TRIGGER_STATUS_WaitingForDMATransmissionComplete){

			transmitting++;

		}else if(dma_status == DMA_TRIGGER_STATUS_WaitAccessFlag){

			wait_flag++;
		}


    	// LED_TOGGLE(LED4);
		vTaskDelayUntil( &xLastWakeTime, xFrequency );
    }
}


int main(void)
{
	vSemaphoreCreateBinary(serial_tx_wait_sem);
	serial_rx_queue = xQueueCreate(5, sizeof(serial_msg));
	gps_serial_queue = xQueueCreate(5, sizeof(serial_msg));
	vSemaphoreCreateBinary(flight_control_sem);
	vSemaphoreCreateBinary(usart3_dma_send_sem);
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

	CAN2_Config();
	CAN2_NVIC_Config();

	/* 	Starting dma test part */
	LED_OFF(LED1);
	LED_OFF(LED2);
	LED_OFF(LED3);
	LED_OFF(LED4);
	enable_usart2_dma_interrupt();
	/* initialize dma interrupt */
	uart2_tx_stream_initialize();


	// while(1){
	// 		uartTX_stream_append_data_to_buffer(text_to_test,10, ACCESSING_FLAG_TASK_MAIN);
	// 		uartTX_stream_append_data_to_buffer(text_to_test,10, ACCESSING_FLAG_TASK_MAIN);
	// 		uartTX_stream_append_data_to_buffer(text_to_test2,18, ACCESSING_FLAG_TASK_MAIN);
	// 		Delay_1us(200);
	// 		LED_TOGGLE(LED1);

	// 		// usart2_dma_burst_send(text_to_test,20);
	// 		//usart2_dma_send(text_to_test);
	// }



	/* Register the FreeRTOS task */
	/* Flight control task */
	// xTaskCreate(
	// 	(pdTASK_CODE)flight_control_task,
	// 	(signed portCHAR*)"flight control task",
	// 	4096,
	// 	NULL,
	// 	tskIDLE_PRIORITY + 9,
	// 	NULL
	// );
	/* NEEED CLEAN UP, FOR TEST PURPOSE ONLY */

	xTaskCreate(
		(pdTASK_CODE)flight_control_dummy_task,
		(signed portCHAR*)"fcu_dummy_task",
		512,
		NULL,
		tskIDLE_PRIORITY + 8,
		NULL
	);

	xTaskCreate(
		(pdTASK_CODE)dummy_task1,
		(signed portCHAR*)"dummy_task1",
		512,
		NULL,
		tskIDLE_PRIORITY + 5,
		NULL
	);

	xTaskCreate(
		(pdTASK_CODE)dummy_task2,
		(signed portCHAR*)"dummy_task2",
		512,
		NULL,
		tskIDLE_PRIORITY + 4,
		NULL
	);

	xTaskCreate(
		(pdTASK_CODE)dummy_task3,
		(signed portCHAR*)"dummy_task3",
		512,
		NULL,
		tskIDLE_PRIORITY + 3,
		NULL
	);

	xTaskCreate(
		(pdTASK_CODE)dummy_task4,
		(signed portCHAR*)"dummy_task4",
		512,
		NULL,
		tskIDLE_PRIORITY + 3,
		NULL
	);




#if 0

	/* Navigation task */
	xTaskCreate(
		(pdTASK_CODE)navigation_task,
		(signed portCHAR*)"navigation task",
		512,
		NULL,
		tskIDLE_PRIORITY + 7,
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
		4096,
		NULL,
		tskIDLE_PRIORITY + 7, NULL
	);

	xTaskCreate(
		(pdTASK_CODE)gps_receive_task,

		(signed portCHAR *) "gps receive task",
		2048,
		NULL,
		tskIDLE_PRIORITY + 8, NULL

	);
#endif

	vTaskStartScheduler();

	return 0;
}


