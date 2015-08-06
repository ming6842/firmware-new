#include "stm32f4xx_conf.h"
#include <string.h>
#include <stdio.h>
#include "usart.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#define PRINTF_USART UART8
/* Serial Initializaton ------------------------------------------------------*/

/* USART Initializaton ------------------------------------------------------*/

static void enable_usart1(void)
{
	/* RCC Initialization */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

	/* GPIO Initialization */
	GPIO_InitTypeDef GPIO_InitStruct = {
		.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10,
		.GPIO_Mode = GPIO_Mode_AF,
		.GPIO_OType = GPIO_OType_PP,
		.GPIO_PuPd = GPIO_PuPd_UP,
		.GPIO_Speed = GPIO_Speed_50MHz
	};

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	/* USART3 Initialization */
	USART_InitTypeDef USART_InitStruct = {
		.USART_BaudRate = 57600,
		.USART_WordLength = USART_WordLength_8b,
		.USART_StopBits = USART_StopBits_1,
		.USART_Parity = USART_Parity_No,
		.USART_HardwareFlowControl = USART_HardwareFlowControl_None,
		.USART_Mode = USART_Mode_Rx | USART_Mode_Tx
	};

	USART_Init(USART1, &USART_InitStruct);
	USART_Cmd(USART1, ENABLE);
}

static void enable_usart2(void)
{
	/* RCC Initialization */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

	/* GPIO Initialization */
	GPIO_InitTypeDef GPIO_InitStruct = {
		.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6,
		.GPIO_Mode = GPIO_Mode_AF,
		.GPIO_OType = GPIO_OType_PP,
		.GPIO_PuPd = GPIO_PuPd_UP,
		.GPIO_Speed = GPIO_Speed_50MHz
	};

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);
	GPIO_Init(GPIOD, &GPIO_InitStruct);

	/* USART2 Initialization */
	USART_InitTypeDef USART_InitStruct = {
		.USART_BaudRate = 57600,
		.USART_WordLength = USART_WordLength_8b,
		.USART_StopBits = USART_StopBits_1,
		.USART_Parity = USART_Parity_No,
		.USART_HardwareFlowControl = USART_HardwareFlowControl_None,
		.USART_Mode = USART_Mode_Rx | USART_Mode_Tx
	};

	USART_Init(USART2, &USART_InitStruct);
	USART_Cmd(USART2, ENABLE);

	/* DMA Initialization */
	DMA_DeInit(DMA1_Stream6);
	while (DMA_GetCmdStatus(DMA1_Stream6) != DISABLE);
}

static void enable_usart3(void)
{
	/* RCC Initialization */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	/* GPIO Initialization */
	GPIO_InitTypeDef GPIO_InitStruct = {
		.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9,
		.GPIO_Mode = GPIO_Mode_AF,
		.GPIO_OType = GPIO_OType_PP,
		.GPIO_PuPd = GPIO_PuPd_UP,
		.GPIO_Speed = GPIO_Speed_50MHz
	};

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);
	GPIO_Init(GPIOD, &GPIO_InitStruct);

	/* USART3 Initialization */
	USART_InitTypeDef USART_InitStruct = {
		.USART_BaudRate = 57600,
		.USART_WordLength = USART_WordLength_8b,
		.USART_StopBits = USART_StopBits_1,
		.USART_Parity = USART_Parity_No,
		.USART_HardwareFlowControl = USART_HardwareFlowControl_None,
		.USART_Mode = USART_Mode_Rx | USART_Mode_Tx
	};

	USART_Init(USART3, &USART_InitStruct);
	USART_Cmd(USART3, ENABLE);

}

static void enable_usart3_interrupt(void)
{
	NVIC_InitTypeDef nvic;

	nvic.NVIC_IRQChannel =  DMA1_Stream3_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 2;
	nvic.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&nvic);

	nvic.NVIC_IRQChannel = USART3_IRQn;

	NVIC_Init(&nvic);

	DMA_ITConfig(DMA1_Stream3, DMA_IT_TC, ENABLE);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
}

static void enable_usart4(void)
{
	/* RCC Initialization */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

	/* GPIO Initialization */
	GPIO_InitTypeDef GPIO_InitStruct = {
		.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11,
		.GPIO_Mode = GPIO_Mode_AF,
		.GPIO_OType = GPIO_OType_PP,
		.GPIO_PuPd = GPIO_PuPd_UP,
		.GPIO_Speed = GPIO_Speed_50MHz
	};

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4);
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	/* USART4 Initialization */
	USART_InitTypeDef USART_InitStruct = {
		.USART_BaudRate = 57600,
		.USART_WordLength = USART_WordLength_8b,
		.USART_StopBits = USART_StopBits_1,
		.USART_Parity = USART_Parity_No,
		.USART_HardwareFlowControl = USART_HardwareFlowControl_None,
		.USART_Mode = USART_Mode_Rx | USART_Mode_Tx
	};

	USART_Init(UART4, &USART_InitStruct);
	USART_Cmd(UART4, ENABLE);
}

static void enable_usart5(void)
{
	/* RCC Initialization */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);

	/* GPIO Initialization */
	GPIO_InitTypeDef GPIO_InitStruct = {
		.GPIO_Pin = GPIO_Pin_2,
		.GPIO_Mode = GPIO_Mode_AF,
		.GPIO_OType = GPIO_OType_PP,
		.GPIO_PuPd = GPIO_PuPd_UP,
		.GPIO_Speed = GPIO_Speed_50MHz
	};

	GPIO_Init(GPIOD, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;

	GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5);

	/* USART5 Initialization */
	USART_InitTypeDef USART_InitStruct = {
		.USART_BaudRate = 57600,
		.USART_WordLength = USART_WordLength_8b,
		.USART_StopBits = USART_StopBits_1,
		.USART_Parity = USART_Parity_No,
		.USART_HardwareFlowControl = USART_HardwareFlowControl_None,
		.USART_Mode = USART_Mode_Rx | USART_Mode_Tx
	};

	USART_Init(UART5, &USART_InitStruct);
	USART_Cmd(UART5, ENABLE);
}

static void enable_usart8(void)
{
	/* RCC Initialization */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART8, ENABLE);

	/* GPIO Initialization */
	GPIO_InitTypeDef GPIO_InitStruct = {
		.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1,
		.GPIO_Mode = GPIO_Mode_AF,
		.GPIO_OType = GPIO_OType_PP,
		.GPIO_PuPd = GPIO_PuPd_UP,
		.GPIO_Speed = GPIO_Speed_50MHz
	};

	GPIO_PinAFConfig(GPIOE, GPIO_PinSource0, GPIO_AF_UART8);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource1, GPIO_AF_UART8);
	GPIO_Init(GPIOE, &GPIO_InitStruct);

	/* USART8 Initialization */
	USART_InitTypeDef USART_InitStruct = {
		.USART_BaudRate = 57600,
		.USART_WordLength = USART_WordLength_8b,
		.USART_StopBits = USART_StopBits_1,
		.USART_Parity = USART_Parity_No,
		.USART_HardwareFlowControl = USART_HardwareFlowControl_None,
		.USART_Mode = USART_Mode_Rx | USART_Mode_Tx
	};

	USART_Init(UART8, &USART_InitStruct);
	USART_Cmd(UART8, ENABLE);
	
	/* DMA Initialization */
	DMA_DeInit(DMA1_Stream6);

	while (DMA_GetCmdStatus(DMA1_Stream6) != DISABLE);
}

void usart_init()
{
	enable_usart1();
	enable_usart2();
	enable_usart3();
	enable_usart3_interrupt();
	enable_usart4();
	enable_usart5();
	enable_usart8();
}

void usart2_dma_init(void)
{

	uint8_t dummy = 0;
	DMA_InitTypeDef DMA_InitStructure = {
	/* Configure DMA Initialization Structure */
		.DMA_BufferSize = (uint32_t)1,
		.DMA_FIFOMode = DMA_FIFOMode_Disable,
		.DMA_FIFOThreshold = DMA_FIFOThreshold_Full,
		.DMA_MemoryBurst = DMA_MemoryBurst_Single,
		.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte,
		.DMA_MemoryInc = DMA_MemoryInc_Enable,
		.DMA_Mode = DMA_Mode_Normal,
		.DMA_PeripheralBaseAddr = (uint32_t)(&(USART2->DR)),
		.DMA_PeripheralBurst = DMA_PeripheralBurst_Single,
		.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte,
		.DMA_PeripheralInc = DMA_PeripheralInc_Disable,
		.DMA_Priority = DMA_Priority_Medium,
		/* Configure TX DMA */
		.DMA_Channel = DMA_Channel_4,
		.DMA_DIR = DMA_DIR_MemoryToPeripheral,
		.DMA_Memory0BaseAddr = (uint32_t)&dummy
	};

	DMA_Init(DMA1_Stream6, &DMA_InitStructure);
	DMA_Cmd(DMA1_Stream6, ENABLE);

	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);

}
void usart2_dma_send(uint8_t *s)
{
	while (DMA_GetFlagStatus(DMA1_Stream6, DMA_FLAG_TCIF6) == RESET);

	DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6);

	DMA_InitTypeDef  DMA_InitStructure = {
		/* Configure DMA Initialization Structure */
		.DMA_BufferSize = (uint32_t)strlen((const char *) s),
		.DMA_FIFOMode = DMA_FIFOMode_Disable,
		.DMA_FIFOThreshold = DMA_FIFOThreshold_Full,
		.DMA_MemoryBurst = DMA_MemoryBurst_Single,
		.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte,
		.DMA_MemoryInc = DMA_MemoryInc_Enable,
		.DMA_Mode = DMA_Mode_Normal,
		.DMA_PeripheralBaseAddr = (uint32_t)(&(USART2->DR)),
		.DMA_PeripheralBurst = DMA_PeripheralBurst_Single,
		.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte,
		.DMA_PeripheralInc = DMA_PeripheralInc_Disable,
		.DMA_Priority = DMA_Priority_Medium,
		/* Configure TX DMA */
		.DMA_Channel = DMA_Channel_4,
		.DMA_DIR = DMA_DIR_MemoryToPeripheral,
		.DMA_Memory0BaseAddr = (uint32_t)s
	};
	DMA_Init(DMA1_Stream6, &DMA_InitStructure);

	DMA_Cmd(DMA1_Stream6, ENABLE);

	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);


}

void enable_usart2_dma_interrupt(void){

    /* NVIC Initialization */
    NVIC_InitTypeDef NVIC_InitStruct = {
      .NVIC_IRQChannel = DMA1_Stream6_IRQn,
      .NVIC_IRQChannelPreemptionPriority = 0,// Because i'm not using OS right now // configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 2,
      .NVIC_IRQChannelCmd = ENABLE
    };
    NVIC_Init(&NVIC_InitStruct);


	DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6);
	DMA_ClearITPendingBit(DMA1_Stream6, DMA_IT_TCIF6);

	DMA_ITConfig(DMA1_Stream6, DMA_IT_TC, ENABLE);
}

void usart2_dma_burst_send(uint8_t *s,uint16_t len)
{

	DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6);
	DMA_InitTypeDef  DMA_InitStructure = {
		/* Configure DMA Initialization Structure */
		.DMA_BufferSize = (uint32_t)len,
		.DMA_FIFOMode = DMA_FIFOMode_Disable,
		.DMA_FIFOThreshold = DMA_FIFOThreshold_Full,
		.DMA_MemoryBurst = DMA_MemoryBurst_Single,
		.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte,
		.DMA_MemoryInc = DMA_MemoryInc_Enable,
		.DMA_Mode = DMA_Mode_Normal,
		.DMA_PeripheralBaseAddr = (uint32_t)(&(USART2->DR)),
		.DMA_PeripheralBurst = DMA_PeripheralBurst_Single,
		.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte,
		.DMA_PeripheralInc = DMA_PeripheralInc_Disable,
		.DMA_Priority = DMA_Priority_Medium,
		/* Configure TX DMA */
		.DMA_Channel = DMA_Channel_4,
		.DMA_DIR = DMA_DIR_MemoryToPeripheral,
		.DMA_Memory0BaseAddr = (uint32_t)s
	};
	DMA_Init(DMA1_Stream6, &DMA_InitStructure);

	DMA_Cmd(DMA1_Stream6, ENABLE);

	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
}



int _write(int fd, char *ptr, int len)
{
	/* Write "len" of char from "ptr" to file id "fd"
	 * Return number of char written.
	 * Need implementing with UART here. */
	int i = 0;
	fd=fd;
	for (i = 0; i < len ; i++) {
		USART_SendData(PRINTF_USART, (uint8_t) *ptr);

		/* Loop until USART2 DR register is empty */
		while (USART_GetFlagStatus(PRINTF_USART, USART_FLAG_TXE) == RESET);

		ptr++;
	}

	return len;
}


xSemaphoreHandle serial_tx_wait_sem = NULL;
xQueueHandle serial_rx_queue = NULL;
xQueueHandle gps_serial_queue = NULL;
void USART3_IRQHandler(void)
{
	long lHigherPriorityTaskWoken = pdFALSE;

	serial_msg rx_msg;

	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
		rx_msg.ch = USART_ReceiveData(USART3);

		if (!xQueueSendToBackFromISR(serial_rx_queue, &rx_msg, &lHigherPriorityTaskWoken))
			portEND_SWITCHING_ISR(lHigherPriorityTaskWoken);

	}

	portEND_SWITCHING_ISR(lHigherPriorityTaskWoken);
}

char usart3_read(void)
{
	serial_msg msg;

	while (!xQueueReceive(serial_rx_queue, &msg, portMAX_DELAY));

	return msg.ch;
}

void usart3_send(char str)
{
	while (!xSemaphoreTake(serial_tx_wait_sem, portMAX_DELAY));

	USART_SendData(USART3, (uint16_t)str);
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
}

void uart8_puts(uint8_t *ptr)
{
	while(*ptr!='\0'){

		USART_SendData(PRINTF_USART, (uint8_t) *ptr);

		/* Loop until USART8 DR register is empty */
		while (USART_GetFlagStatus(PRINTF_USART, USART_FLAG_TXE) == RESET);
		ptr++;
	}

}

xSemaphoreHandle usart3_dma_send_sem = NULL;

void usart3_dma_send(uint8_t *ptr, uint16_t size)
{

	DMA_InitTypeDef  DMA_InitStructure = {
		/* Configure DMA Initialization Structure */
		.DMA_BufferSize = size,
		.DMA_FIFOMode = DMA_FIFOMode_Disable,
		.DMA_FIFOThreshold = DMA_FIFOThreshold_Full,
		.DMA_MemoryBurst = DMA_MemoryBurst_Single,
		.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte,
		.DMA_MemoryInc = DMA_MemoryInc_Enable,
		.DMA_Mode = DMA_Mode_Normal,
		.DMA_PeripheralBaseAddr = (uint32_t)(&(USART3->DR)),
		.DMA_PeripheralBurst = DMA_PeripheralBurst_Single,
		.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte,
		.DMA_PeripheralInc = DMA_PeripheralInc_Disable,
		.DMA_Priority = DMA_Priority_Medium,
		/* Configure TX DMA */
		.DMA_Channel = DMA_Channel_4,
		.DMA_DIR = DMA_DIR_MemoryToPeripheral,
		.DMA_Memory0BaseAddr = (uint32_t)ptr
	};

	if ( xSemaphoreTake(usart3_dma_send_sem, portMAX_DELAY) == pdTRUE) {

		DMA_Init(DMA1_Stream3, &DMA_InitStructure);
		DMA_Cmd(DMA1_Stream3, ENABLE);
		USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);
	}

}

void DMA1_Stream3_IRQHandler(void)
{
	portBASE_TYPE lHigherPriorityTaskWoken = pdFALSE;

	if( DMA_GetITStatus(DMA1_Stream3, DMA_IT_TCIF3) != RESET) {

		xSemaphoreGiveFromISR(usart3_dma_send_sem, &lHigherPriorityTaskWoken);//if unblock a task, set pdTRUE to lHigherPriorityTaskWoken
		DMA_ClearITPendingBit(DMA1_Stream3, DMA_IT_TCIF3);

	}

	portEND_SWITCHING_ISR(lHigherPriorityTaskWoken);//force to do context switch
}




/************************** Streaming TX Service ****************************************/

xSemaphoreHandle dma_tx_bufferAvailableSemaphore[16];
xSemaphoreHandle dma_tx_DMAWaitComplete[16];

void streaming_dma_tx_initilize(void){

	uint8_t i=0;

	for (i = 0; i < 16; i++)
	{

		dma_tx_bufferAvailableSemaphore[i] = NULL;
		dma_tx_DMAWaitComplete[i] = NULL;
		
	}

	for (i = 0; i < 16; i++)
	{
		
		vSemaphoreCreateBinary(dma_tx_bufferAvailableSemaphore[i]);
		vSemaphoreCreateBinary(dma_tx_DMAWaitComplete[i]);
	}

	enable_usart2_dma_interrupt();

}


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

uint8_t DMA1_Stream6_TransmissionCompleteFlag = 0;

void DMA1_Stream6_IRQHandler(void)
{
	
	if( DMA_GetITStatus(DMA1_Stream6, DMA_IT_TCIF6) != RESET) {

		DMA1_Stream6_TransmissionCompleteFlag = 1;
		DMA_ClearITPendingBit(DMA1_Stream6, DMA_IT_TCIF6);
	}

}


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
DMATriggerStatus streaming_dma_tx_dma_trigger(void){

	uint8_t current_buffer;
	/* Get current trigger condition */

	switch(dma_trigger_current_status ){
		case DMA_TRIGGER_STATUS_WaitingForData:

			/* Check for active buffer to fill data into it */
			if(dma_tx_buffer[0].DMATransmittingFlag == BUFFER_STATUS_BufferFilling){

				current_buffer = 0;

			}else if(dma_tx_buffer[1].DMATransmittingFlag == BUFFER_STATUS_BufferFilling){

				current_buffer = 1;

			}

			/* check if there is any data inside buffer */
			if(dma_tx_buffer[current_buffer].currentIndex >0){

				/* Close current buffer */
				dma_tx_buffer[current_buffer].DMATransmittingFlag = BUFFER_STATUS_ClosedWaitForTransmit;

				/* Enable and reset counter for another buffer */
				if(current_buffer == 0){
					// LED_ON(LED3);
					// LED_OFF(LED2);
					dma_tx_buffer[1].DMATransmittingFlag = BUFFER_STATUS_BufferFilling;
					dma_tx_buffer[1].currentIndex = 0;

				}else{
					// LED_ON(LED2);
					// LED_OFF(LED3);

					dma_tx_buffer[0].DMATransmittingFlag = BUFFER_STATUS_BufferFilling;
					dma_tx_buffer[0].currentIndex = 0;


				}

				/* check for access flag */

				if(dma_tx_buffer[current_buffer].accessingFlag ==0){

					/* Not busy, ready for transmit */

					//////////////Set DMA////////////
					usart2_dma_burst_send(dma_tx_buffer[current_buffer].buffer,dma_tx_buffer[current_buffer].currentIndex);
					/* Set status flags */
					dma_tx_buffer[current_buffer].DMATransmittingFlag = BUFFER_STATUS_DMATransmitting;
					dma_trigger_current_status = DMA_TRIGGER_STATUS_WaitingForDMATransmissionComplete;
				}else{

					/* Some task is still filling in buffer, put trigger on pending */
					dma_trigger_current_status = DMA_TRIGGER_STATUS_WaitAccessFlag;
				}



			}else{

				/* No data inside buffer, no need to transmit */

			}

 
 
		break;

		case DMA_TRIGGER_STATUS_WaitingForDMATransmissionComplete:

			if( DMA1_Stream6_TransmissionCompleteFlag == 1){

				/* Check which buffer was being transmitted */

				/* Check which buffer is waiting to send */
				if(dma_tx_buffer[0].DMATransmittingFlag == BUFFER_STATUS_DMATransmitting){

					current_buffer = 0;

				}else if(dma_tx_buffer[1].DMATransmittingFlag == BUFFER_STATUS_DMATransmitting){

					current_buffer = 1;

				}

				/* clear the flag !! */
				DMA1_Stream6_TransmissionCompleteFlag = 0;
				/* Transmission is now complete, set the flag to idle */
				dma_tx_buffer[current_buffer].DMATransmittingFlag = BUFFER_STATUS_DMAIdle;

				/* Set trigger state to send mode */
				dma_trigger_current_status = DMA_TRIGGER_STATUS_WaitingForData;

				/* Give semaphore to wake up the requested task (buffer available) */


			}else{

				/* Go and drink coffee dude ~ nothing to do now */
			}

		break;

		case DMA_TRIGGER_STATUS_WaitAccessFlag:


			/* Check which buffer is waiting to send */
			if(dma_tx_buffer[0].DMATransmittingFlag == BUFFER_STATUS_ClosedWaitForTransmit){

				current_buffer = 0;

			}else if(dma_tx_buffer[1].DMATransmittingFlag == BUFFER_STATUS_ClosedWaitForTransmit){

				current_buffer = 1;

			}

			if(dma_tx_buffer[current_buffer].accessingFlag == 0){

						/* Not busy, ready for transmit */

						//////////////Set DMA////////////
						// usart2_dma_burst_send(buffer1,10);
						
						usart2_dma_burst_send(dma_tx_buffer[current_buffer].buffer,dma_tx_buffer[current_buffer].currentIndex);
						/* Set status flags */
						dma_tx_buffer[current_buffer].DMATransmittingFlag = BUFFER_STATUS_DMATransmitting;
						dma_trigger_current_status = DMA_TRIGGER_STATUS_WaitingForDMATransmissionComplete;
			}else{

				/* keep waiting */

			}

		break;



	}

	return dma_trigger_current_status;

}

DMATXTransmissionResult  streaming_dma_tx_write(uint8_t *s,uint16_t len, uint16_t task_id,FailureHandler routineIfFailed, CompleteFlagHandler waitcomplete){

	uint8_t transmissionResult = DMA_TX_Result_TransmissionFailed;
	uint8_t shouldEnd=0;
	ErrorMessage err;

	while(!shouldEnd){

		err = streaming_dma_tx_append_data_to_buffer(s,len, task_id);

		if(err == NO_ERROR){

			shouldEnd = 1;
			transmissionResult = DMA_TX_Result_AppendedIntoBuffer;

		}else if(err == BUFFER_FULL){

			/* Handle buffer full according to user choice */
			if(routineIfFailed == DMA_TX_FailureHandler_WaitReadySemaphore){
				/* Set flag to request semaphore when buffer is available */

					//////////////////////////////////

				/* Wait semaphore */

					//////////////////////////////////

			}else{

				/* Just skip packet, exit now */
				return DMA_TX_Result_TransmissionFailed;

			}

		}else{

			/* Transmission failed, routine error */
			shouldEnd = 1;
			transmissionResult = DMA_TX_Result_TransmissionFailed;
		}



	}



	/* Wait complete handler */

	/* Check if data is appended to buffer */
	if( transmissionResult == DMA_TX_Result_AppendedIntoBuffer){


		if(waitcomplete == DMA_TX_CompleteFlagHandler_NoWait){

			/* No wait, just exit */
			return DMA_TX_Result_AppendedIntoBuffer;

		}else if(waitcomplete == DMA_TX_CompleteFlagHandler_WaitCompleteSemaphore){

					//////////////////////////////////
					//////////////////////////////////

		}else {
			/* Should not come here */

			return 0x99;
		}



	}else{

		return DMA_TX_Result_TransmissionFailed;

	}



	return 0;

}



// xSemaphoreTake(serial_tx_wait_sem, portMAX_DELAY));

// xSemaphoreGive( xSemaphore )