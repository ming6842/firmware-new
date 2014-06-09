#ifndef FILE_HMC5983_H
#define FILE_HMC5983_H

#include "stm32f4xx_conf.h"
#include "imu.h"
#include "spi.h"

#define HMC5983_CONFIG_REG_A 0x00
#define HMC5983_CONFIG_REG_B 0x01
#define HMC5983_MODE_REG 0x02
#define HMC5983_DATA_OUTOUT_X_H 0x03

#define HMC5983_SPI SPI1
#define HMC5983_SELECT() 	GPIO_ResetBits(GPIOE,GPIO_Pin_4)
#define HMC5983_DESELECT() 	GPIO_SetBits(GPIOE,GPIO_Pin_4)


void hmc5983_delay(volatile uint32_t );
void hmc5983_initialize_config(void);
void hmc5983_update(void);
#endif