/* #include "stm32f4_i2c.h" */

#ifndef __I2C_H
#define __I2C_H

#include "stm32f4xx.h"

void i2c_Init(void);
void enable_i2c1(void);
void enable_i2c2(void);
#define EEPROM_A2_PIN_GROUP GPIOB
#define EEPROM_A2_PIN GPIO_Pin_7

#endif
