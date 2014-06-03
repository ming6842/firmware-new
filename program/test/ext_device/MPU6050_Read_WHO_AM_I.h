#ifndef __MPU6050_Read_WHO_AM_I_H
#define __MPU6050_Read_WHO_AM_I_H

/*MPU6050 I2C*/
void I2C_start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction);
void I2C_write(I2C_TypeDef* I2Cx, uint8_t data);
uint8_t I2C_read_ack(I2C_TypeDef* I2Cx);
uint8_t I2C_read_nack(I2C_TypeDef* I2Cx);
void I2C_stop(I2C_TypeDef* I2Cx);

void I2C_start_write(I2C_TypeDef* I2Cx, uint8_t address, uint8_t data);
void I2C_start_read(I2C_TypeDef* I2Cx, uint8_t address, uint8_t data, uint8_t* pBuffer, uint16_t NumByteToRead);

#endif