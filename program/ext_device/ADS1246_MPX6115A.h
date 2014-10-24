//ADS1246_MPX6115A.h

#ifndef FILE_ADS1246_MPX6115A_H
#define FILE_ADS1246_MPX6115A_H

#include "stm32f4xx_conf.h"


/* ====================================================================== */
/*						ADS1246 Related define							  */
/* ====================================================================== */
#define ADS1246_MPX6115A_SPI SPI1

#define ADS1246_CS_LOW()  GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define ADS1246_CS_HIGH()  GPIO_SetBits(GPIOA, GPIO_Pin_4)

#define ADS1246_DRDY_LOW()  GPIO_ResetBits(GPIOC, GPIO_Pin_4)
#define ADS1246_DRDY_HIGH()  GPIO_SetBits(GPIOC, GPIO_Pin_4) // Temporary only

#define ADS1246_START_LOW()  GPIO_ResetBits(GPIOB, GPIO_Pin_0)
#define ADS1246_START_HIGH()  GPIO_SetBits(GPIOB, GPIO_Pin_0) 

#define ADS1246_RESET_LOW()  GPIO_ResetBits(GPIOC, GPIO_Pin_5)
#define ADS1246_RESET_HIGH()  GPIO_SetBits(GPIOC, GPIO_Pin_5) 

#define ADS1246_DRDY_PIN_STATE() GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4)

/* ====================================================================== */
/*						MPX6115A Related define							  */
/* ====================================================================== */



#define ALT_PER_LSB 0.055992861f //0.0447942885376f  //in cm (2.048 ref)
/* ====================================================================== */

void ads1246_delay(volatile uint32_t);
void ads1246_reset(void);
uint8_t ads1246_read1byte(uint8_t addr);
void ads1246_write1byte(uint8_t , uint8_t );
void ads1246_initialize(void);
float MPX6115_get_raw_altitude(int32_t );
int32_t ads1246_readADCconversion(void);
void MPX6115_update_tare_value(void);

#endif