#include <unistd.h>
#include <stdarg.h>
#include <ctype.h>

#include "string.h"
#include "stdlib.h"

#include "QuadCopterConfig.h"

/* Serial Initializaton ------------------------------------------------------*/

/**
  * @brief  USART3 Serial port initialization
  * @param  None
  * @retval None
  */
void Serial_Config() /* Tx:Pb10, Rx:Pb11 */
{
	/* RCC Initialization */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 |RCC_APB1Periph_USART3 | 
		RCC_APB1Periph_UART4 | RCC_APB1Periph_UART5 | 
		RCC_APB1Periph_UART8, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

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
	/*USART2*/
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);
	GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	
	USART_InitStruct.USART_BaudRate = 57600;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART2, &USART_InitStruct);
	USART_Cmd(USART2, ENABLE);

	/*USART3*/
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);
	GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	
	USART_InitStruct.USART_BaudRate = 57600;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART3, &USART_InitStruct);
	USART_Cmd(USART3, ENABLE);


	/*USART4*/
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource1, GPIO_AF_UART4);
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	
	USART_InitStruct.USART_BaudRate = 57600;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(UART4, &USART_InitStruct);
	USART_Cmd(UART4, ENABLE);

	/*USART5*/
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 ;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5);
	GPIO_Init(GPIOD, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART4);
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	
	USART_InitStruct.USART_BaudRate = 57600;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(UART5, &USART_InitStruct);
	USART_Cmd(UART5, ENABLE);



	/*USART8*/
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_PinAFConfig(GPIOE, GPIO_PinSource0, GPIO_AF_UART8);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource1, GPIO_AF_UART8);
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	
	USART_InitStruct.USART_BaudRate = 57600;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(UART8, &USART_InitStruct);
	USART_Cmd(UART8, ENABLE);


	// USART_ClearFlag(USART3, USART_FLAG_TC);

	// /* NVIC Initialization */
	// USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
	// USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

	// NVIC_InitTypeDef NVIC_InitStruct = {
	// 	.NVIC_IRQChannel = USART3_IRQn,
	// 	.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1,
	// 	.NVIC_IRQChannelSubPriority = 0,
	// 	.NVIC_IRQChannelCmd = ENABLE
	// };
	// NVIC_Init(&NVIC_InitStruct);
}

void USART3_IRQHandler()
{
	long lHigherPriorityTaskWoken = pdFALSE;

	serial_msg rx_msg;

	if (USART_GetITStatus(USART3, USART_IT_TXE) != RESET) {
		xSemaphoreGiveFromISR(serial_tx_wait_sem, &lHigherPriorityTaskWoken);

		USART_ITConfig(USART3, USART_IT_TXE, DISABLE);

	} else if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
		rx_msg.ch = USART_ReceiveData(USART3);

		if (!xQueueSendToBackFromISR(serial_rx_queue, &rx_msg, &lHigherPriorityTaskWoken))
			portEND_SWITCHING_ISR(lHigherPriorityTaskWoken);

	} else {
		while (1);
	}

	portEND_SWITCHING_ISR(lHigherPriorityTaskWoken);
}

/* Serial Structure ----------------------------------------------------------*/

/* Serial structure */
SERIAL serial = {
	.getc = getc_base,
	.putc = putc_base,
	.gets = gets_base,
	.puts = puts_base,
	.printf = printf_base
};

/**
  * Function name : serial.getc (function base)
  * @brief  get a char from the serial char queue
  * @param  None
  * @retval None
  */
char getc_base(void)
{
	serial_msg msg;

	while (!xQueueReceive(serial_rx_queue, &msg, portMAX_DELAY));

	return msg.ch;
}

/**
  * Function name : serial.putc (function base)
  * @brief  send a char through the serial
  * @param  None
  * @retval None
  */
void putc_base(char str)
{
	while (!xSemaphoreTake(serial_tx_wait_sem, portMAX_DELAY));

	USART_SendData(USART3, (uint16_t)str);
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
}

/**
  * Function name : serial.gets (function base)
  * @brief  get a string from the serial
  * @param  None
  * @retval None
  */
int gets_base(void)
{
	char str;
	str = serial.getc();
	serial.printf("%c", str);
	return 1;
}

/**
  * Function name : serial.puts (function base)
  * @brief  send a string through the serial
  * @param  None
  * @retval None
  */
int puts_base(const char *msg)
{
	for (; *msg; msg++)
		serial.putc(*msg);

	return 1;
}


int printf_base(const char *format, ...)
{
	char str[128];
	va_list para;
	va_start(para, format);
	int curr_pos = 0;
	char ch[] = {'0', '\0'};
	char integer[11];
	str[0] = '\0';

	while (format[curr_pos] != '\0') {
		if (format[curr_pos] != '%') {
			ch[0] = format[curr_pos];
			strcat(str, ch);

		} else {
			switch (format[++curr_pos]) {
			case 's':
				strcat(str, va_arg(para, char *));
				break;

			case 'c':
				ch[0] = (char)va_arg(para, int);
				strcat(str, ch);
				break;

			case 'i':
			case 'f':
				strcat(str, ftoa(va_arg(para, double)));
				break;

			case 'd':
				strcat(str, itoa(va_arg(para, int), integer));
				break;

			case 'u':
				strcat(str, itoa(va_arg(para, unsigned), integer));
				break;

			default:
				break;
			}
		}

		curr_pos++;
	}

	va_end(para);
	serial.puts(str);
	return 1;
}
