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
	RCC_AHB1PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
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
		.USART_BaudRate = 9600,
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
	USART_ClearFlag(USART3, USART_FLAG_TC);

	USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

	/* NVIC Initialization */
	NVIC_InitTypeDef NVIC_InitStruct = {
		.NVIC_IRQChannel = USART3_IRQn,
		.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1,
		.NVIC_IRQChannelSubPriority = 0,
		.NVIC_IRQChannelCmd = ENABLE
	};
	NVIC_Init(&NVIC_InitStruct);
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
	enable_usart4();
	enable_usart5();
	enable_usart8();
}

void usart2_dma_init()
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

	if (USART_GetITStatus(USART3, USART_IT_TXE) != RESET) {
		xSemaphoreGiveFromISR(serial_tx_wait_sem, &lHigherPriorityTaskWoken);

		USART_ITConfig(USART3, USART_IT_TXE, DISABLE);

	}
	
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