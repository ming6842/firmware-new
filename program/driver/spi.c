#include "QuadCopterConfig.h"

/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : SPI_WriteByte
**功能 : Transmit 1Byte Data
**輸入 : SPIx, WriteByte
**輸出 : None
**使用 : SPI_WriteByte(SPI1, 0xFF);
**=====================================================================================================*/
/*=====================================================================================================*/
void SPI_WriteByte(SPI_TypeDef *SPIx, u8 WriteByte)
{
	while ((SPIx->SR & SPI_I2S_FLAG_TXE) == (u16)RESET);

	SPIx->DR = WriteByte;

	while ((SPIx->SR & SPI_I2S_FLAG_RXNE) == (u16)RESET);

	SPIx->DR;
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : SPI_ReadByte
**功能 : Receive 1Byte Data
**輸入 : SPIx
**輸出 : None
**使用 : Read = SPI_ReadByte(SPI1);
**=====================================================================================================*/
/*=====================================================================================================*/
u8 SPI_ReadByte(SPI_TypeDef *SPIx)
{
	while ((SPIx->SR & SPI_I2S_FLAG_TXE) == (u16)RESET);

	SPIx->DR = 0xFF;

	while ((SPIx->SR & SPI_I2S_FLAG_RXNE) == (u16)RESET);

	return SPIx->DR;
}
/*=====================================================================================================*/
/*=====================================================================================================*/
void SPI_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
	/* CSN PB12 */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 |
					GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	

	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex; // 雙線全雙工
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master; // 主模式
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b; // 數據大小8位
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low; // 時鐘極性，空閒時為低
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge; // 第1個邊沿有效，上升沿為采樣時刻
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft; // NSS信號由軟件產生
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2; // 8分頻，9MHz
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB; // 高位在前
	SPI_InitStruct.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStruct);

	SPI_Cmd(SPI1, ENABLE);
}