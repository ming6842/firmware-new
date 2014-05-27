#include "stm32f4xx_conf.h"
#include "mpu9250.h"

u8 SPI_xfer(SPI_TypeDef *SPIx, uint8_t  WriteByte)
{
	uint8_t rxdata;

	SPI_I2S_SendData(SPIx, (uint16_t) WriteByte);
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);

	while (SPI_I2S_GetFlagStatus(SPIx, SPI_FLAG_RXNE) == RESET);
		rxdata = SPI_I2S_ReceiveData(SPIx);


		return rxdata;
}

void mpu9250_read_who_am_i()
{
	uint8_t rxdata;
	/*Keep CS Pin Low */
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
}