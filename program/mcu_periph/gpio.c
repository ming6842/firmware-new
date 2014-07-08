#include "stm32f4xx_conf.h"
#include "gpio.h"
#include "spi.h"


void led_init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct = {
		/* LED_R PC13 */  /* LED_G PC14 */  /* LED_B PC15 */
		.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_10 | GPIO_Pin_12 | GPIO_Pin_15,
		.GPIO_Mode = GPIO_Mode_OUT,
		.GPIO_OType = GPIO_OType_PP,
		.GPIO_Speed = GPIO_Speed_50MHz,
		.GPIO_PuPd = GPIO_PuPd_NOPULL
	};
	GPIO_Init(GPIOE, &GPIO_InitStruct);

		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOC, &GPIO_InitStruct);

		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOC, &GPIO_InitStruct);

		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);

		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);

}

