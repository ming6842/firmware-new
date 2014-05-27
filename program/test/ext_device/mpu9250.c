#include "stm32f4xx_conf.h"
#include "mpu9250.h"

#define MPU9250_CS_PIN_LOW GPIO_ResetBits(MPU9250_SPI_Port,MPU9250_SPI_CS_PIN)
#define MPU9250_CS_PIN_HIGH GPIO_SetBits(MPU9250_SPI_Port,MPU9250_SPI_CS_PIN)
void mpu9250_read_who_am_i()
{
	uint8_t rxdata;
	MPU9250_CS_PIN_LOW;
	Delay_1us(1);
	
	while (SPI_I2S_GetFlagStatus(MPU6050_SPI_RCC_Periph, SPI_I2S_FLAG_TXE) == RESET);
		SPI_I2S_SendData(MPU6050_SPI_RCC_Periph, (uint16_t) 0xf5);
	while (SPI_I2S_GetFlagStatus(MPU6050_SPI_RCC_Periph, SPI_FLAG_RXNE) == RESET);
		rxdata = SPI_I2S_ReceiveData(MPU6050_SPI_RCC_Periph);

	SPI_I2S_SendData(MPU6050_SPI_RCC_Periph, (uint16_t) 0xff);
	while (SPI_I2S_GetFlagStatus(MPU6050_SPI_RCC_Periph, SPI_I2S_FLAG_TXE) == RESET);

	while (SPI_I2S_GetFlagStatus(MPU6050_SPI_RCC_Periph, SPI_FLAG_RXNE) == RESET);
		rxdata = SPI_I2S_ReceiveData(MPU6050_SPI_RCC_Periph);
	MPU9250_CS_PIN_HIGH;
}
	