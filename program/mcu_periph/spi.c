#include "stm32f4xx_conf.h"
#include "spi.h"

uint8_t SPI_xfer(SPI_TypeDef *SPIx, uint8_t  WriteByte)
{
	uint8_t rxdata;

	SPI_I2S_SendData(SPIx, (uint16_t) WriteByte);

	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);

	while (SPI_I2S_GetFlagStatus(SPIx, SPI_FLAG_RXNE) == RESET);

	rxdata = SPI_I2S_ReceiveData(SPIx);


	return rxdata;
}


void SPI_WriteByte(SPI_TypeDef *SPIx, u8 WriteByte)
{
	while ((SPIx->SR & SPI_I2S_FLAG_TXE) == (u16)RESET);

	SPIx->DR = WriteByte;

	while ((SPIx->SR & SPI_I2S_FLAG_RXNE) == (u16)RESET);

	SPIx->DR;
}

uint8_t SPI_ReadByte(SPI_TypeDef *SPIx)
{
	while ((SPIx->SR & SPI_I2S_FLAG_TXE) == (u16)RESET);

	SPIx->DR = 0xFF;

	while ((SPIx->SR & SPI_I2S_FLAG_RXNE) == (u16)RESET);

	return SPIx->DR;
}


void enable_spi1()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
	/* CSN PB12 */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 |
				   GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex; // 雙線全雙工
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master; // 主模式
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b; // 數據大小8位
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low; // 時鐘極性，空閒時為低
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge; // 第1個邊沿有效，上升沿為采樣時刻
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft; // NSS信號由軟件產生
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8; // 8分頻，9MHz
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB; // 高位在前
	SPI_InitStruct.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStruct);
	//IDLE=0 and SAMPLE_FALL

	SPI_Cmd(SPI1, ENABLE);
}
/*spi2 initialize */
void enable_spi2()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

	//GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);
	/* CSN PB12 */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 |
				   GPIO_Pin_15 | GPIO_Pin_14;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);


	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex; // 雙線全雙工
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master; // 主模式
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b; // 數據大小8位
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_High; // 時鐘極性，空閒時為低
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge; // 第1個邊沿有效，上升沿為采樣時刻
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft; // NSS信號由軟件產生
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2; // 8分頻，9MHz
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB; // 高位在前
	SPI_InitStruct.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStruct);

	SPI_Cmd(SPI2, ENABLE);
}

/*spi4 initialize */
void enable_spi4()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI4, ENABLE);

	GPIO_PinAFConfig(GPIOE, GPIO_PinSource2, GPIO_AF_SPI4);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_SPI4);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource6, GPIO_AF_SPI4);
	/* CSN PB12 */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 |
				   GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOE, &GPIO_InitStruct);

	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex; // 雙線全雙工
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master; // 主模式
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b; // 數據大小8位
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_High; // 時鐘極性，空閒時為低
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge; // 第1個邊沿有效，上升沿為采樣時刻
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft; // NSS信號由軟件產生
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8; // 8分頻，9MHz
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB; // 高位在前
	SPI_InitStruct.SPI_CRCPolynomial = 7;
	SPI_Init(SPI4, &SPI_InitStruct);

	SPI_Cmd(SPI4, ENABLE);
}

void spi_init(void)
{
	enable_spi1();
	enable_spi2();
	enable_spi4();

}