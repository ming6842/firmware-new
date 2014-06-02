#ifndef __AT24C04C_H
#define __AT24C04C_H
void eeprom_byte_write(void);
void eeprom_byte_read(void);



/*MPU6050 I2C*/
void I2C_start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction);
void I2C_write(I2C_TypeDef* I2Cx, uint8_t data);
uint8_t I2C_read_ack(I2C_TypeDef* I2Cx);
uint8_t I2C_read_nack(I2C_TypeDef* I2Cx);
void I2C_stop(I2C_TypeDef* I2Cx);

#endif
