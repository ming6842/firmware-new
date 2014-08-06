#ifndef __BOARD_CONFIG_H
#define __BOARD_CONFIG_H

#ifdef configUSE_QCOPTER_V2_BOARD
#endif

/* LED */
#define LED_RED 
#define LED_GREEN
#define LED_BLUE

void led_init(void);
void led_on(GPIO_TypeDef *led);
void led_off(GPIO_TypeDef *led);
void led_toggle(GPIO_TypeDef *led);

/* USART */
#define SERIAL

void usart_init(void);
void usart_send(UART_TypeDef *usart, uint8_t data);
uint8_t usart_read(UART_TypeDef *usart);

/* I2C */
#define MPU9150
#define MS5611

void i2c_init(void);
void i2c_start(I2C_TypeDef *i2c);
void i2c_send(I2C_TypeDef *port, uint8_t i2c);
void i2c_read_ack(I2C_TypeDef *i2c);
void i2c_read_nack(I2C_TypeDef *i2c);
void i2c_stop(I2C_TypeDef *i2c);

#endif
