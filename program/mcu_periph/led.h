//led.h
#ifndef FILE_LED_H
#define FILE_LED_H

#include "stm32f4xx_conf.h"
#define LED_TOGGLE(led_name)  GPIO_ToggleBits(led_name)
#define LED_ON(led_name)  GPIO_ResetBits(led_name)
#define LED_OFF(led_name)  GPIO_SetBits(led_name)

#define LED1 GPIOE, GPIO_Pin_8
#define LED2 GPIOE, GPIO_Pin_10
#define LED3 GPIOE, GPIO_Pin_12
#define LED4 GPIOE, GPIO_Pin_15
#define TOGGLE_DEBUG GPIOC, GPIO_Pin_9

void cycle_led(uint8_t );

#endif
