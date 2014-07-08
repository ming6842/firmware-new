#ifndef FILE_HMC5983_H
#define FILE_HMC5983_H

#include "stm32f4xx_conf.h"
#include "imu.h"
#include "spi.h"

#define HMC5983_CONFIG_REG_A 0x00
#define HMC5983_CONFIG_REG_B 0x01
#define HMC5983_MODE_REG 0x02
#define HMC5983_DATA_OUTOUT_X_H 0x03

#define HMC5983_SPI SPI2
#define HMC5983_SELECT() 	GPIO_ResetBits(GPIOB,GPIO_Pin_12)
#define HMC5983_DESELECT() 	GPIO_SetBits(GPIOB,GPIO_Pin_12)

uint8_t hmc5983_readByte(uint8_t addr);
void hmc5983_delay(volatile uint32_t );
void hmc5983_initialize_config(void);
void hmc5983_update(imu_unscaled_data_t *imu_unscaledData);
void hmc5983_convert_to_scale(imu_unscaled_data_t *, imu_data_t *, imu_calibrated_offset_t *);
void hmc5983_apply_mag_calibration(imu_calibrated_offset_t *imu_offset);
void hmc5983_initialize_system(imu_calibrated_offset_t *imu_offset);

#endif