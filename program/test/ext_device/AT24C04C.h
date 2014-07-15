#ifndef __AT24C04C_H
#define __AT24C04C_H

// void eeprom_byte_write(I2C_TypeDef* I2Cx, uint8_t DeviceAddr, uint8_t RegisterAddr, uint8_t RegisterValue);
// uint8_t eeprom_byte_read(I2C_TypeDef* I2Cx, uint8_t DeviceAddr, uint8_t RegisterAddr);

void I2C_EE_BufferWrite(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite);
void I2C_EE_BufferRead(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead);
void I2C_EE_PageWrite(uint8_t* pBuffer, uint8_t WriteAddr, uint8_t NumByteToWrite);
void I2C_EE_WaitEepromStandbyState(void);

#endif
