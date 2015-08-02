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



/* This define busy flag for every task slot */
#define	ACCESSING_FLAG_TASK_MAIN   					((uint16_t)0x0001)	
#define	ACCESSING_FLAG_TASK_FLIGHT_STABILIZER   	((uint16_t)0x0002)		
#define	ACCESSING_FLAG_TASK_FLIGHT_CONTROLLER   	((uint16_t)0x0004)	
#define	ACCESSING_FLAG_TASK_NAVIGATION			   	((uint16_t)0x0008)	
#define	ACCESSING_FLAG_TASK_MAVLINK				   	((uint16_t)0x0010)	
#define	ACCESSING_FLAG_TASK_MAVLINK_PARAMETER		((uint16_t)0x0020)	
#define	ACCESSING_FLAG_TASK_MAVLINK_MISSION			((uint16_t)0x0040)	
#define	ACCESSING_FLAG_TASK_MAVLINK_WAYPOINT		((uint16_t)0x0080)		


typedef enum {INACTIVE = 0, ACTIVE = !INACTIVE} BufferActiveStatus;
typedef enum {
	BUFFER_STATUS_BufferFilling = 0, 
	BUFFER_STATUS_ClosedWaitForTransmit, 
	BUFFER_STATUS_DMATransmitting, 
	BUFFER_STATUS_DMAIdle
} DMATransmitStatus;


typedef enum {
	NO_ERROR=0,
	BUFFER_FULL, 
	NO_ACTIVE_BUFFER,
	PERMISSION_ALREADY_OCCUPIED
} ErrorMessage;

typedef enum {
	DMA_TRIGGER_STATUS_WaitingForData=0,
	DMA_TRIGGER_STATUS_WaitingForDMATransmissionComplete,
	DMA_TRIGGER_STATUS_WaitAccessFlag

} DMATriggerStatus;

// typedef enum {
// 	SKIP_PACKET=0,
// 	WAIT_U, 
// 	NO_ACTIVE_BUFFER,
// 	PERMISSION_OCCUPIED
// } ErrorMessage;

#define configUSART_DMA_TX_BUFFER_SIZE 256
typedef struct uart_dma_tx_buffer_t{

	uint16_t currentIndex;
	uint16_t accessingFlag;
	DMATransmitStatus DMATransmittingFlag;
	uint8_t buffer[configUSART_DMA_TX_BUFFER_SIZE];

} uart_dma_tx_buffer_t;

static uart_dma_tx_buffer_t dma_tx_buffer[2] = {


	/* Memory 0 initialization, set this one to acitve first */
	{
		.currentIndex = 0,
		.accessingFlag = 0,
		.DMATransmittingFlag = BUFFER_STATUS_BufferFilling,
		.buffer[0 ... (configUSART_DMA_TX_BUFFER_SIZE-1)] = 0
	},

	/* Memory 1 initialization */
	{
		.currentIndex = 0,
		.accessingFlag = 0,
		.DMATransmittingFlag = BUFFER_STATUS_DMAIdle,
		.buffer[0 ... (configUSART_DMA_TX_BUFFER_SIZE-1)] = 0
	}
};

ErrorMessage streaming_dma_tx_append_data_to_buffer(uint8_t *s,uint16_t len, uint16_t task_id){

	ErrorMessage errorStatus = NO_ERROR;
	uint8_t selected_buffer;
	/* Check for active buffer to fill data into it */
	if(dma_tx_buffer[0].DMATransmittingFlag == BUFFER_STATUS_BufferFilling){

		selected_buffer = 0;

	}else if(dma_tx_buffer[1].DMATransmittingFlag == BUFFER_STATUS_BufferFilling){

		selected_buffer = 1;

	}else {
	/* No available buffer, return error */
		errorStatus = NO_ACTIVE_BUFFER;
		return errorStatus;
	}

	/* Check if busy flag is already set */
	if(( dma_tx_buffer[selected_buffer].accessingFlag & task_id) == 1){

		errorStatus = PERMISSION_ALREADY_OCCUPIED;
		return errorStatus;
	}

	/* Check if buffer size is enough for the packet */
	if (len <(configUSART_DMA_TX_BUFFER_SIZE - dma_tx_buffer[selected_buffer].currentIndex)){

		/* Shift current index of buffer away to reserve space */
		uint16_t startIndex = dma_tx_buffer[selected_buffer].currentIndex;
		dma_tx_buffer[selected_buffer].currentIndex += len;

		/* set occupy flag */
		dma_tx_buffer[selected_buffer].accessingFlag = dma_tx_buffer[selected_buffer].accessingFlag | task_id;

		/* Fill in the buffer */
		memcpy(&dma_tx_buffer[selected_buffer].buffer[startIndex],s,len);

		/* Reset occupy flag */
		dma_tx_buffer[selected_buffer].accessingFlag = dma_tx_buffer[selected_buffer].accessingFlag & ~task_id;


	}else{

		/* Not enough buffer space */
		errorStatus = BUFFER_FULL;
		return errorStatus;

	 }

	return errorStatus;
}

static DMATriggerStatus dma_trigger_current_status = DMA_TRIGGER_STATUS_WaitingForData;
void streaming_dma_tx_dma_trigger(void){

	/* Get current trigger condition */

	switch(dma_trigger_current_status ){
		case DMA_TRIGGER_STATUS_WaitingForData:

    

 
 
		break;

		case DMA_TRIGGER_STATUS_WaitingForDMATransmissionComplete:



		break;

		case DMA_TRIGGER_STATUS_WaitAccessFlag:


		break;



	}



	/* Check DMA current status */

	/* if DMA has finished transmission, close filling buffer and swap buffer immediately */

	/* check status of closed buffer if there is any busy flag. if there is, pend it to next loop, if not, set DMA */


}

void usart2_dma_double_buffer_init()
{

	uint8_t dummy = 0;
	DMA_InitTypeDef DMA_InitStructure = {
	/* Configure DMA Initialization Structure */
		.DMA_BufferSize = (uint32_t)30,
		.DMA_FIFOMode = DMA_FIFOMode_Disable,
		.DMA_FIFOThreshold = DMA_FIFOThreshold_Full,
		.DMA_MemoryBurst = DMA_MemoryBurst_Single,
		.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte,
		.DMA_MemoryInc = DMA_MemoryInc_Enable,
		.DMA_Mode = DMA_Mode_Circular,  // Enable circular mode
		.DMA_PeripheralBaseAddr = (uint32_t)(&(USART2->DR)),
		.DMA_PeripheralBurst = DMA_PeripheralBurst_Single,
		.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte,
		.DMA_PeripheralInc = DMA_PeripheralInc_Disable,
		.DMA_Priority = DMA_Priority_Medium,
		/* Configure TX DMA */
		.DMA_Channel = DMA_Channel_4,
		.DMA_DIR = DMA_DIR_MemoryToPeripheral,
		.DMA_Memory0BaseAddr = (uint32_t)buffer1
	};

	/*Connect DMA Pointer to buffer*/
	DMA_DoubleBufferModeConfig(DMA1_Stream6, (uint32_t)buffer2, DMA_Memory_0);
	/*Enable double buffer mode*/
    DMA_DoubleBufferModeCmd(DMA1_Stream6, ENABLE);

	DMA_Init(DMA1_Stream6, &DMA_InitStructure);
	DMA_Cmd(DMA1_Stream6, ENABLE);

	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);

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

	uint8_t text_to_test[] = "1234567890123456789012345678901234567890123456789012345";
	uint8_t error_capture = streaming_dma_tx_append_data_to_buffer(text_to_test,50, ACCESSING_FLAG_TASK_MAIN);


	while(1){

	}
	/* Register the FreeRTOS task */
	/* Flight control task */
	xTaskCreate(
		(pdTASK_CODE)flight_control_task,
		(signed portCHAR*)"flight control task",
		4096,
		NULL,
		tskIDLE_PRIORITY + 9,
		NULL
	);

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
	vTaskStartScheduler();

	return 0;
}

