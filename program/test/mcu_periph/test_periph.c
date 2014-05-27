#include "stm32f4xx_conf.h"
#include "gpio.h"
#include "i2c.h"
#include "usart.h"
#include "spi.h"
#include "tim.h"
#include "mpu9250.h"
void Delay_1us(vu32 nCnt_1us)
{
	u32 nCnt;

	for (; nCnt_1us != 0; nCnt_1us--)
		for (nCnt = 45; nCnt != 0; nCnt--);
}
	uint8_t rcv_id;
int main(void)
{


	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE,  ENABLE);
	led_init();
	usart_init();
	spi_init();
	pwm_input_output_init();
	i2c_Init();
	uint8_t rxdata;
	uint8_t i2c1_data;
	while(1) {

	Delay_1us(10);

	GPIO_ResetBits(GPIOE,GPIO_Pin_4);
	//Delay_1us(1);
					 SPI_xfer(SPI4,0xf5);
			rcv_id = SPI_xfer(SPI4,0xff);
	GPIO_SetBits(GPIOE,GPIO_Pin_4);


	GPIO_ResetBits(GPIOE,GPIO_Pin_4);
	Delay_1us(1);

	SPI_I2S_SendData(SPI4, (uint16_t) 0xf5);
	while (SPI_I2S_GetFlagStatus(SPI4, SPI_I2S_FLAG_TXE) == RESET);
	
	while (SPI_I2S_GetFlagStatus(SPI4, SPI_FLAG_RXNE) == RESET);
		rxdata = SPI_I2S_ReceiveData(SPI4);

	SPI_I2S_SendData(SPI4, (uint16_t) 0xff);
	while (SPI_I2S_GetFlagStatus(SPI4, SPI_I2S_FLAG_TXE) == RESET);

	while (SPI_I2S_GetFlagStatus(SPI4, SPI_FLAG_RXNE) == RESET);
		rxdata = SPI_I2S_ReceiveData(SPI4);

	GPIO_SetBits(GPIOE,GPIO_Pin_4);



		GPIO_ToggleBits(GPIOE, GPIO_Pin_8 | GPIO_Pin_10 | GPIO_Pin_12 | GPIO_Pin_15);
		USART_SendData( USART1, (uint16_t) 0xaa);
		USART_SendData( USART2, (uint16_t) 0xaa);
		USART_SendData( USART3, (uint16_t) 0xaa);
		USART_SendData( UART4, (uint16_t) 0xaa);
		USART_SendData( UART5, (uint16_t) 0xaa);
		USART_SendData( UART8, (uint16_t) 0xaa);
		Delay_1us(100000);
	}

	return 0;
}

