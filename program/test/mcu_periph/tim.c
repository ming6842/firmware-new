#include "stm32f4_system.h"
#include "stm32f4xx_conf.h"
/* TIM2 PWM3  PA0 */	/* TIM2 PWM4  PA1 */	/* TIM2 PWM5  PA2 */	/* TIM2 PWM8  PA3 */
/* TIM3 PWM9  PA6 */	/* TIM3 PWM10 PA7 */

/* TIM3 PWM11 PB0 */	/* TIM3 PWM12 PB1 */	/* TIM4 PWM1  PB6 */	/* TIM4 PWM2  PB7 */


void enable_tim1(void)
{

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	/* -- GPIO Configuration ---------------------------------------------------- */
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_TIM1);

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_9 | GPIO_Pin_11 |
					GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_Init(GPIOE, &GPIO_InitStruct);

	/* -- Timer Configuration --------------------------------------------------- */
	TIM_DeInit(TIM1);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
	TIM_TimeBaseStruct.TIM_Period = 2500 - 1;  //2.5ms , 400kHz
	TIM_TimeBaseStruct.TIM_Prescaler = 18*10 - 1; //84 = 1M(1us)
	TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStruct);


	/*TIM2 TIM3 TIM4 TIM8 */
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 2000;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCNPolarity_High;
  	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;
  	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCIdleState_Reset;

	TIM_OC1Init(TIM1, &TIM_OCInitStruct);
	TIM_OC2Init(TIM1, &TIM_OCInitStruct);
	TIM_OC3Init(TIM1, &TIM_OCInitStruct);
	TIM_OC4Init(TIM1, &TIM_OCInitStruct);

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM1, ENABLE);
	TIM_Cmd(TIM1, ENABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

void enable_tim2(void)
{

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	/* -- GPIO Configuration ---------------------------------------------------- */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_TIM2);

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_2 | GPIO_Pin_3 |
					GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	/* -- Timer Configuration --------------------------------------------------- */
	TIM_DeInit(TIM2);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
	TIM_TimeBaseStruct.TIM_Period = 2500 - 1;  //2.5ms , 400kHz
	TIM_TimeBaseStruct.TIM_Prescaler = 84 - 1; //84 = 1M(1us)
	TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStruct);


	/*TIM2 TIM3 TIM4 TIM8 */
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 2000;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC1Init(TIM2, &TIM_OCInitStruct);
	TIM_OC2Init(TIM2, &TIM_OCInitStruct);
	TIM_OC3Init(TIM2, &TIM_OCInitStruct);
	TIM_OC4Init(TIM2, &TIM_OCInitStruct);

	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM2, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
}
void enable_tim3(void)
{

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	/* -- GPIO Configuration ---------------------------------------------------- */
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM3);

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7 |
					GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_Init(GPIOC, &GPIO_InitStruct);

	/* -- Timer Configuration --------------------------------------------------- */
	TIM_DeInit(TIM3);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
	TIM_TimeBaseStruct.TIM_Period = 2500 - 1;  //2.5ms , 400kHz
	TIM_TimeBaseStruct.TIM_Prescaler = 84 - 1; //84 = 1M(1us)
	TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStruct);


	/*TIM2 TIM3 TIM4 TIM8 */
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 2000;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC1Init(TIM3, &TIM_OCInitStruct);
	TIM_OC2Init(TIM3, &TIM_OCInitStruct);
	TIM_OC3Init(TIM3, &TIM_OCInitStruct);
	TIM_OC4Init(TIM3, &TIM_OCInitStruct);

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM3, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
}

void enable_tim4(void)
{

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	/* -- GPIO Configuration ---------------------------------------------------- */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_12 | GPIO_Pin_13 |
					GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_Init(GPIOD, &GPIO_InitStruct);

	/* -- Timer Configuration --------------------------------------------------- */
	TIM_DeInit(TIM4);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
	TIM_TimeBaseStruct.TIM_Period = 2500 - 1;  //2.5ms , 400kHz
	TIM_TimeBaseStruct.TIM_Prescaler = 84 - 1; //84 = 1M(1us)
	TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStruct);


	/*TIM2 TIM3 TIM4 TIM8 */
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 2000;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC1Init(TIM4, &TIM_OCInitStruct);
	TIM_OC2Init(TIM4, &TIM_OCInitStruct);
	TIM_OC3Init(TIM4, &TIM_OCInitStruct);
	TIM_OC4Init(TIM4, &TIM_OCInitStruct);

	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM4, ENABLE);
	TIM_Cmd(TIM4, ENABLE);
}

void enable_tim5(void)
{

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

	/* -- GPIO Configuration ---------------------------------------------------- */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5);


	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_Init(GPIOA, &GPIO_InitStruct);

	/* -- Timer Configuration --------------------------------------------------- */
	TIM_DeInit(TIM5);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
	TIM_TimeBaseStruct.TIM_Period = 2500 - 1;  //2.5ms , 400kHz
	TIM_TimeBaseStruct.TIM_Prescaler = 84 - 1; //84 = 1M(1us)
	TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStruct);


	/*TIM2 TIM3 TIM4 TIM8 */
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 2000;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC1Init(TIM5, &TIM_OCInitStruct);
	TIM_OC2Init(TIM5, &TIM_OCInitStruct);
	TIM_OC3Init(TIM5, &TIM_OCInitStruct);
	TIM_OC4Init(TIM5, &TIM_OCInitStruct);

	TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM5, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM5, ENABLE);
	TIM_Cmd(TIM5, ENABLE);
}
void enable_tim9()
{
#define TIM_CLK 180000000

#define ARR_VALUE 20
#define PRESCALER_VALUE 180
#define TIM_PERIOD TIM_CKL/(ARR_VALUE*TIM_PERIOD)

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable the TIM2 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel =  TIM1_BRK_TIM9_IRQn ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);

	/* -- Timer Configuration --------------------------------------------------- */
	TIM_DeInit(TIM9);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
	TIM_TimeBaseStruct.TIM_Period = ARR_VALUE - 1-1;  //2.5ms , 400kHz
	TIM_TimeBaseStruct.TIM_Prescaler = PRESCALER_VALUE - 1; //84 = 1M(1us)
	TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM9, &TIM_TimeBaseStruct);
	TIM_ITConfig( TIM9, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM9, ENABLE);
}


void enable_tim10()
{
#define TIM_CLK 180000000

#define ARR_VALUE 20*50
#define PRESCALER_VALUE 180
#define TIM_PERIOD TIM_CKL/(ARR_VALUE*TIM_PERIOD)

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable the TIM2 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel =  TIM1_UP_TIM10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);

	/* -- Timer Configuration --------------------------------------------------- */
	TIM_DeInit(TIM10);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
	TIM_TimeBaseStruct.TIM_Period = ARR_VALUE - 60;  //2.5ms , 400kHz
	TIM_TimeBaseStruct.TIM_Prescaler = PRESCALER_VALUE - 1; //84 = 1M(1us)
	TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM10, &TIM_TimeBaseStruct);
	TIM_ITConfig( TIM10, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM10, ENABLE);
}
void enable_tim12()
{
#define TIM_CLK 180000000

#define ARR_VALUE 20
#define PRESCALER_VALUE 180
#define TIM_PERIOD TIM_CKL/(ARR_VALUE*TIM_PERIOD)

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable the TIM2 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel =  TIM8_BRK_TIM12_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);

	/* -- Timer Configuration --------------------------------------------------- */
	TIM_DeInit(TIM12);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
	TIM_TimeBaseStruct.TIM_Period = ARR_VALUE - 1;  //2.5ms , 400kHz
	TIM_TimeBaseStruct.TIM_Prescaler = PRESCALER_VALUE - 1; //84 = 1M(1us)
	TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM12, &TIM_TimeBaseStruct);
	TIM_ITConfig( TIM12, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM12, ENABLE);
}
void pwm_input_output_init()
{
	enable_tim1();
	enable_tim2();
	enable_tim3();
	enable_tim4();
	enable_tim5();
	enable_tim9();
	enable_tim10();
	enable_tim12();
}