/* #include "module_motor.h" */

#ifndef FILE_TIM_H
#define FILE_TIM_H

#include "stm32f4xx.h"
#include "FreeRTOS.h"
void pwm_init(void);
void pwm_input_output_init(void);
void enable_tim1(void);
void enable_tim2(void);
void enable_tim3(void);
void enable_tim4(void);
void enable_tim5(void);
void enable_tim9(void);
void enable_tim10(void);
void enable_tim12(void);
#endif
