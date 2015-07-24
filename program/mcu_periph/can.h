
#ifndef FILE_CAN_H
#define FILE_CAN_H

#include "stm32f4xx_conf.h"
#include "stm32f4xx_can.h"
#include "led.h"

#define CANx                       CAN1
#define CAN_CLK                    RCC_APB1Periph_CAN1
#define CAN_RX_PIN                 GPIO_Pin_0
#define CAN_TX_PIN                 GPIO_Pin_1
#define CAN_GPIO_PORT              GPIOD
#define CAN_GPIO_CLK               RCC_AHB1Periph_GPIOD
#define CAN_AF_PORT                GPIO_AF_CAN1
#define CAN_RX_SOURCE              GPIO_PinSource0
#define CAN_TX_SOURCE              GPIO_PinSource1     


typedef struct can_message_receivedFlag_t {
	uint8_t MagnetometerUpdated;
	uint8_t BarometerUpdated;
} can_message_receivedFlag_t;


typedef enum {
	CAN_MESSAGE_MAGNETOMETER = 0,
	CAN_MESSAGE_BAROMETER,
	CAN_MESSAGE_SIZE,
} CanMessageType;


void can1_init(void);
uint8_t can_loopback_test(void);

void CAN1_Config(void);
void CAN1_Transmit(void);
void CAN1_NVIC_Config(void);
void CAN1_RX0_IRQHandler(void);

void CAN2_Config(void);
void CAN2_Transmit(void);
void CAN2_NVIC_Config(void);
void CAN2_RX0_IRQHandler(void);

void CAN2_BroadcastMode(uint8_t mode, uint8_t safetystatus);

CanRxMsg CAN2_PassRXMessage(uint8_t messageID);
uint8_t CAN2_CheckMessageStatusFlag(uint8_t messageID);
void CAN2_ClearMessageStatusFlag(uint8_t messageID);

#endif
