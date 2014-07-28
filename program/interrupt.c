#include "stm32f4xx_conf.h"
#include "interrupt.h"
void TIM1_BRK_TIM9_IRQHandler()
{
        if (TIM_GetITStatus(TIM9, TIM_IT_Update) != RESET){
 //       	GPIO_ToggleBits(GPIOE, GPIO_Pin_8 | GPIO_Pin_10 | GPIO_Pin_12 | GPIO_Pin_15);
        	estimator_trigger_flag=1;
				TIM_ClearITPendingBit(TIM9, TIM_IT_Update);
        }
}

void TIM8_BRK_TIM12_IRQHandler()
{
        if (TIM_GetITStatus(TIM12, TIM_IT_Update) != RESET){
				TIM_ClearITPendingBit(TIM12, TIM_IT_Update);
        }
}
void TIM1_UP_TIM10_IRQHandler()
{
	if (TIM_GetITStatus(TIM10, TIM_IT_Update) != RESET){
				TIM_ClearITPendingBit(TIM10, TIM_IT_Update);
        }
}

void **HARDFAULT_PSP;
register void *stack_pointer asm("sp");
void HardFault_Handler(void)
{
#if 0
	//Hijack the process stack pointer to make backtrace work
	asm("mrs %0, psp" : "=r"(HARDFAULT_PSP) : :);
	stack_pointer = HARDFAULT_PSP;
#endif
	while (1);
}
void MemManage_Handler(void)
{
	while (1);
}
void BusFault_Handler(void)
{
	while (1);
}
void UsageFault_Handler(void)
{
	while (1);
}
void DebugMon_Handler(void)
{
	while (1);
}
void NMI_Handler(void)
{
	while (1);
}
