
#include "stm32f4xx_conf.h"	
#include "i2c.h"
#define  EEP_Firstpage 0x10

void enable_i2c1()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	

	// 啟用 I2C1 的 RCC 時鐘
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	// 啟用 GPIOB 時鐘，主要是啟用 PB6 腳位的 SCL 時鐘、PB7 腳位的 SDA 時鐘
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_I2C1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);
	/* 設定 SCL 與 SDA 腳位
    *
    * 配對腳位：
    *         SCL = PB8
    *         SDA = PB9
    */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Pin = EEPROM_A2_PIN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(EEPROM_A2_PIN_GROUP, &GPIO_InitStruct);
	GPIO_SetBits(EEPROM_A2_PIN_GROUP, EEPROM_A2_PIN);

    I2C_InitTypeDef I2C_InitStruct;

	I2C_InitStruct.I2C_ClockSpeed =400000;
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_OwnAddress1 = 0x0A;   //0x00
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable; // disable acknowledge when reading (can be changed later on)  I2C_Ack_Enable;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_Init(I2C1, &I2C_InitStruct);

	I2C_AcknowledgeConfig(I2C1,ENABLE);
	I2C_Cmd(I2C1, ENABLE);

	

	

} 

void enable_i2c2()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	I2C_InitTypeDef I2C_InitStruct;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_I2C2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_I2C2);

	/* SCL I2Cx_SCL_PIN */  /* SDA I2Cx_SDA_PIN*/
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_OwnAddress1 = 0x00;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStruct.I2C_ClockSpeed =100000;
	I2C_Cmd(I2C2, ENABLE);
	I2C_Init(I2C2, &I2C_InitStruct);

}
void i2c_Init()
{
	enable_i2c1();
	enable_i2c2();
}



void I2C_Test()
{
	uint16_t i;
	uint8_t I2c_Buf_Write[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
	uint8_t I2c_Buf_Read[16];

	I2C_EE_PageWrite(I2c_Buf_Write, EEP_Firstpage, 16);
	I2C_EE_WaitEepromStandbyState();

	I2C_EE_BufferRead(I2c_Buf_Read, EEP_Firstpage, 16);


}