#include "stm32f4xx_conf.h"
#include "gpio.h"
#include "i2c.h"
#include "usart.h"
#include "spi.h"
#include "tim.h"
#include "mpu9250.h"
#include <stdio.h>
void Delay_1us(vu32 nCnt_1us)
{
	u32 nCnt;

	for (; nCnt_1us != 0; nCnt_1us--)
		for (nCnt = 45; nCnt != 0; nCnt--);
}
int main(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE,  ENABLE);
	led_init();
	usart_init();
	spi_init();
	pwm_input_output_init();
	i2c_Init();
	usart2_dma_init();
	uint8_t rxdata;
	uint8_t i2c1_data;
	while(1) {
		printf("071071071 yoyoyoy\r\n ");
		usart2_dma_send("test dma\r\n");
		eeprom_byte_write();
		mpu9250_read_who_am_i();
		SPI_I2S_SendData(SPI1, (uint16_t) 0xa5);
		SPI_I2S_SendData(SPI1, (uint16_t) 0xa5);
		SPI_I2S_SendData(SPI2, (uint16_t) 0xa5);
		SPI_I2S_SendData(SPI2, (uint16_t) 0xa5);
		// SPI_I2S_SendData(SPI4, (uint16_t) 0xa5);
		// SPI_I2S_SendData(SPI4, (uint16_t) 0xa5);
		GPIO_ToggleBits(GPIOE, GPIO_Pin_8 | GPIO_Pin_10 | GPIO_Pin_12 | GPIO_Pin_15);
		USART_SendData( USART1, (uint16_t) 0xaa);
		USART_SendData( USART3, (uint16_t) 0xaa);
		USART_SendData( UART4, (uint16_t) 0xaa);
		USART_SendData( UART5, (uint16_t) 0xaa);
		//USART_SendData( UART8, (uint16_t) 0xaa);
		Delay_1us(100);

	}

	return 0;
}

