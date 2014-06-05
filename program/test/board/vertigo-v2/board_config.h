#ifndef __BOARD_CONFIG_H
#define __BOARD_CONFIG_H

#ifdef configUSE_VERTIGO_V2_BOARD

/* LED */
#define LED1 
#define LED2
#define LED3
#define LED4

void led_init(void);
void led_on(int led);
void led_off(int led);
void led_toggle(int led);

/* USART */
#define SERIAL
#define GPS

void usart_init(void);
void usart_send(USART_TypeDef *usart, uint8_t data);
uint8_t usart_read(USART_TypeDef *usart);

/* SPI */
#define MPU9250

void spi_init(void);
uint8_t spi_read(SPI_TypeDef *spi);
void spi_send(SPI_TypeDef *spi);

/* I2C */
#define AT24C04C

void i2c_init(void);
void i2c_start(I2C_TypeDef *i2c);
void i2c_send(I2C_TypeDef *port, uint8_t i2c);
void i2c_read_ack(I2C_TypeDef *i2c);
void i2c_read_nack(I2C_TypeDef *i2c);
void i2c_stop(I2C_TypeDef *i2c);

#endif

#endif
