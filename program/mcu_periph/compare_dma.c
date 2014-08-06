//main.c

#include "stm32f4xx.h"

#include "stdio.h"

char usart_tx_buf[100];

void USART_Configuration(void);

void Delay(__IO uint32_t nCount);

void DMA_Config(void);

void USART2_DMA_Retransmit(char *tx_buf, uint32_t len);

int main()
{

	int i = 0;

	USART_Configuration();

	DMA_Config();

	Delay(500);



	usart_tx_buf[0] = 0xAA;

	usart_tx_buf[1] = 0xAA;

	usart_tx_buf[2] = 0xAA;

	usart_tx_buf[3] = 0xAA;

	usart_tx_buf[4] = 0xAA;

	usart_tx_buf[5] = 0xAA;

	usart_tx_buf[6] = 0xA6;

	usart_tx_buf[7] = 0xA7;

	usart_tx_buf[8] = 0xA9;

	DMA_Cmd(DMA1_Stream6, ENABLE);

	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);



	while (1) {







		USART2_DMA_Retransmit(usart_tx_buf, 5);



		//        Delay(50000);

	}

}

void Delay(__IO uint32_t nCount)

{

	while (nCount--)

	{

	}

}

//*********************************************************

// Function: Configure USART Setting

// Input:  N/A

// Return: N/A

// Remark: N/A

//*********************************************************

void USART_Configuration(void)

{

	GPIO_InitTypeDef GPIO_InitStructure;

	USART_InitTypeDef USART_InitStructure;

	NVIC_InitTypeDef NVIC_InitStructure;

	//Enable Clock

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	//Rx Pin //PA3

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_InitStructure.GPIO_Mode =         GPIO_Mode_AF;

	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;

	GPIO_InitStructure.GPIO_Pin =         GPIO_Pin_3;        // USART1 Rx (PA.3)

	GPIO_Init(GPIOA, &GPIO_InitStructure);



	//Tx Pin //PA5

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_InitStructure.GPIO_Mode =         GPIO_Mode_AF;

	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL  ;

	GPIO_InitStructure.GPIO_Pin =         GPIO_Pin_2;                // USART1 Tx (PA.5)

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//Parameters for USART2

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);



	USART_InitStructure.USART_BaudRate = 56700 * 20;

	USART_InitStructure.USART_WordLength = USART_WordLength_8b;

	USART_InitStructure.USART_StopBits = USART_StopBits_1;

	USART_InitStructure.USART_Parity = USART_Parity_No;

	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

	USART_Init(USART2, &USART_InitStructure);

	USART_Cmd(USART2, ENABLE);

	while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)        ;







	/* Enable the USART1 Interrupt */

	//        NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;

	//        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;

	//        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;

	//        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	//        NVIC_Init(&NVIC_InitStructure);

}

void DMA_Config(void)

{

	NVIC_InitTypeDef NVIC_InitStructure;

	DMA_InitTypeDef  DMA_InitStructure;



	/* Enable DMA clock */

	// RCC_AHB1PeriphClockCmd(DMA1_Stream6_CLOCK, ENABLE);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);



	/* Reset DMA Stream registers (for debug purpose) */

	DMA_DeInit(DMA1_Stream6);

	/* Check if the DMA Stream is disabled before enabling it.

	   Note that this step is useful when the same Stream is used multiple times:

	   enabled, then disabled then re-enabled... In this case, the DMA Stream disable

	   will be effective only at the end of the ongoing data transfer and it will

	   not be possible to re-configure it before making sure that the Enable bit

	   has been cleared by hardware. If the Stream is used only once, this step might

	   be bypassed. */

	while (DMA_GetCmdStatus(DMA1_Stream6) != DISABLE)

	{

	}

#define BUFFER_SIZE 5

	/* Configure DMA Stream */

	DMA_InitStructure.DMA_Channel = DMA_Channel_4;

	DMA_InitStructure.DMA_PeripheralBaseAddr = USART2_BASE + 0x04;

	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)usart_tx_buf;

	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//DMA_DIR_MemoryToMemory;

	DMA_InitStructure.DMA_BufferSize = (uint32_t)BUFFER_SIZE;

	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;

	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;

	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//DMA_MemoryDataSize_Word;

	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;

	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium ;

	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;

	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;

	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;

	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

	DMA_Init(DMA1_Stream6, &DMA_InitStructure);

	/* Enable DMA Stream Transfer Complete interrupt */

	//  DMA_ITConfig(DMA1_Stream6, DMA_IT_TC, ENABLE);

	/* DMA Stream enable */

	DMA_Cmd(DMA1_Stream6, ENABLE);

	/* Check if the DMA Stream has been effectively enabled.

	   The DMA Stream Enable bit is cleared immediately by hardware if there is an

	   error in the configuration parameters and the transfer is no started (ie. when

	   wrong FIFO threshold is configured ...) */

	/* Enable the DMA Stream IRQ Channel */

	//  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream6_IRQ;

	//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;

	//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;

	//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	//  NVIC_Init(&NVIC_InitStructure);

}

void USART2_DMA_Retransmit(char *tx_buf, uint32_t len)
{



	DMA_InitTypeDef  DMA_InitStructure;



	while (SET != DMA_GetFlagStatus(DMA1_Stream6, DMA_FLAG_TCIF6));

	DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6);



	DMA_InitStructure.DMA_Channel = DMA_Channel_4;

	DMA_InitStructure.DMA_PeripheralBaseAddr = USART2_BASE + 0x04;

	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)tx_buf;

	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//DMA_DIR_MemoryToMemory;

	DMA_InitStructure.DMA_BufferSize = (uint32_t)len;

	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;

	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;

	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//DMA_MemoryDataSize_Word;

	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;

	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium ;

	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;

	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;

	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;

	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

	DMA_Init(DMA1_Stream6, &DMA_InitStructure);



	DMA_Cmd(DMA1_Stream6, ENABLE);



}

