#ifndef __INTERRUPT_H
#include "stm32f4xx_conf.h"
uint8_t estimator_trigger_flag;
void SysTick_Handler(void);
void TIM1_BRK_TIM9_IRQHandler(void);

void TIM8_BRK_TIM12_IRQHandler(void);
void TIM1_UP_TIM10_IRQHandler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void DebugMon_Handler(void);
void NMI_Handler(void);
#endif