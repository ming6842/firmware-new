#ifndef FILE_STM32F4_SPI_H
#define FILE_STM32F4_SPI_H

#include "stm32f4xx.h"

uint8_t SPI_xfer(SPI_TypeDef *, uint8_t);

uint8_t spi_read(SPI_TypeDef *spi);
void spi_send(SPI_TypeDef *spi, uint8_t data);
void spi_init(void);
void enable_spi1(void);
void enable_spi2(void);
void enable_spi4(void);

#endif
