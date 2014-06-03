#include "stm32f4xx_conf.h"
#include "AT24C04C.h"


void eeprom_byte_write()
{
	I2C_GenerateSTART(I2C1, ENABLE);
	/*wait EV5 */
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress( I2C1, 0xAF, I2C_Direction_Transmitter);
	/*Wait EV6*/
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	
	/*Wait Word address*/
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING));
	I2C_SendData(I2C1, (uint8_t) 0xA8);
	
	/*Wait Data1*/
	//I2C_SendData(I2C1, (uint8_t) 0x01);
	
	I2C_GenerateSTOP(I2C1, ENABLE);
	//while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
}

void eeprom_byte_read()
{
	

}