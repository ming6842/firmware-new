#include "stm32f4xx_conf.h"
#include "board_config.h"

/* LED Initialization */

void led_init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct = {
		.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_10 | GPIO_Pin_12 | GPIO_Pin_15,
		.GPIO_Mode = GPIO_Mode_OUT,
		.GPIO_OType = GPIO_OType_PP,
		.GPIO_Speed = GPIO_Speed_50MHz,
		.GPIO_PuPd = GPIO_PuPd_DOWN
	};
	GPIO_Init(GPIOE, &GPIO_InitStruct);
}

/* LED Operating Functions */

void led_on(int led)
{
	GPIO_SetBits(GPIOE, led);
}

void led_off(int led)
{
	GPIO_ResetBits(GPIOE, led);
}

void led_toggle(int led)
{
	GPIO_ToggleBits(GPIOE, led);
}
