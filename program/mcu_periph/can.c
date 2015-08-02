/* CAN bus driver */
#include "can.h"

void CAN1_Config(void)
{
  CAN_InitTypeDef        CAN_InitStructure;
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;


  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* CAN GPIOs configuration **************************************************/

  /* Enable GPIO clock */
  RCC_AHB1PeriphClockCmd(CAN_GPIO_CLK, ENABLE);

  /* Connect CAN pins to AF9 */
  GPIO_PinAFConfig(CAN_GPIO_PORT, CAN_RX_SOURCE, CAN_AF_PORT);
  GPIO_PinAFConfig(CAN_GPIO_PORT, CAN_TX_SOURCE, CAN_AF_PORT); 
  
  /* Configure CAN RX and TX pins */
  GPIO_InitStructure.GPIO_Pin = CAN_RX_PIN | CAN_TX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(CAN_GPIO_PORT, &GPIO_InitStructure);

  /* CAN configuration ********************************************************/  
  /* Enable CAN clock */
  RCC_APB1PeriphClockCmd(CAN_CLK, ENABLE);
  
  /* CAN register init */
  CAN_DeInit(CAN1);

  /* CAN cell init */
  CAN_InitStructure.CAN_TTCM = DISABLE;
  CAN_InitStructure.CAN_ABOM = DISABLE;
  CAN_InitStructure.CAN_AWUM = DISABLE;
  CAN_InitStructure.CAN_NART = DISABLE;
  CAN_InitStructure.CAN_RFLM = DISABLE;
  CAN_InitStructure.CAN_TXFP = DISABLE;
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
    
  /* CAN Baudrate = 1 MBps (CAN clocked at 30 MHz) */
  CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
  CAN_InitStructure.CAN_Prescaler = 2;
  CAN_Init(CAN1, &CAN_InitStructure);

  /* CAN filter init */
  CAN_FilterInitStructure.CAN_FilterNumber = 0;
/* USE_CAN1 */
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);
  
  /* Enable FIFO 0 message pending Interrupt */
  CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}

void CAN1_Transmit(void){

	CanTxMsg TxMessage;

  /* Transmit Structure preparation */
  TxMessage.StdId = 0x321;
  TxMessage.ExtId = 0x01;
  TxMessage.RTR = CAN_RTR_DATA;
  TxMessage.IDE = CAN_ID_STD;
  TxMessage.DLC = 3;
  TxMessage.Data[0] = 0xAA;
  TxMessage.Data[1] = 64;
  TxMessage.Data[2] = 64;
  TxMessage.Data[3] = 64;
  TxMessage.Data[4] = 64;
  TxMessage.Data[5] = 64;
  TxMessage.Data[6] = 64;
  TxMessage.Data[7] = 64;
  CAN_Transmit(CAN1, &TxMessage);

}

void CAN1_NVIC_Config(void)
{
  NVIC_InitTypeDef  NVIC_InitStructure;

  NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
 /* USE_CAN1 */

  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
CanRxMsg RxMessage;

void CAN1_RX0_IRQHandler(void)
{
  CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
  
  if ((RxMessage.StdId == 0x321)&&(RxMessage.IDE == CAN_ID_STD))
  {
  		LED_TOGGLE(LED3);
  }
}




void CAN2_Config(void)
{
  CAN_InitTypeDef        CAN_InitStructure;
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;


  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* CAN GPIOs configuration **************************************************/

  /* Enable GPIO clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

  /* Connect CAN pins to AF9 */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_CAN2);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_CAN2); 
  
  /* Configure CAN RX and TX pins */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* CAN configuration ********************************************************/  
  /* Enable CAN clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);
  
  /* CAN register init */
  CAN_DeInit(CAN2);

  /* CAN cell init */
  CAN_InitStructure.CAN_TTCM = DISABLE;
  CAN_InitStructure.CAN_ABOM = DISABLE;
  CAN_InitStructure.CAN_AWUM = DISABLE;
  CAN_InitStructure.CAN_NART = DISABLE;
  CAN_InitStructure.CAN_RFLM = DISABLE;
  CAN_InitStructure.CAN_TXFP = DISABLE;
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
    
  /* CAN Baudrate = 1 MBps (CAN clocked at 30 MHz) */
  CAN_InitStructure.CAN_BS1 = CAN_BS1_2tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
  CAN_InitStructure.CAN_Prescaler = 9;
  CAN_Init(CAN2, &CAN_InitStructure);

  /* CAN filter init */
  CAN_FilterInitStructure.CAN_FilterNumber = 14;
/* USE_CAN2 */
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);
  
  /* Enable FIFO 0 message pending Interrupt */
  CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);
}

void CAN2_NVIC_Config(void)
{
  NVIC_InitTypeDef  NVIC_InitStructure;

  NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
 /* USE_CAN1 */

  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


/* Message buffer area */
CanRxMsg CAN2RxMessage;
can_message_receivedFlag_t canMessageFlag;
CanRxMsg CANMsgDecoded[CAN_MESSAGE_SIZE];


void CAN2_RX0_IRQHandler(void)
{
  CAN_Receive(CAN2, CAN_FIFO0, &CAN2RxMessage);
      

  if (((CAN2RxMessage.ExtId & 0xFFFF0000) == 0x00030000)&&(CAN2RxMessage.IDE == CAN_ID_EXT ))
  {
      canMessageFlag.MagnetometerUpdated = 1;
      CANMsgDecoded[CAN_MESSAGE_MAGNETOMETER] = CAN2RxMessage;
  }else if (((CAN2RxMessage.ExtId & 0xFFFF0000) == 0x00020000)&&(CAN2RxMessage.IDE == CAN_ID_EXT ))
  {
      canMessageFlag.BarometerUpdated = 1;
      CANMsgDecoded[CAN_MESSAGE_BAROMETER] = CAN2RxMessage;
  }
}


void CAN2_Transmit(void){

  CanTxMsg TxMessage;

  /* Transmit Structure preparation */
  TxMessage.StdId = 0x321;
  TxMessage.ExtId = 0x01;
  TxMessage.RTR = CAN_RTR_DATA;
  TxMessage.IDE = CAN_ID_STD;
  TxMessage.DLC = 3;
  TxMessage.Data[0] = 0xAA;
  TxMessage.Data[1] = 64;
  TxMessage.Data[2] = 64;
  TxMessage.Data[3] = 64;
  TxMessage.Data[4] = 64;
  TxMessage.Data[5] = 64;
  TxMessage.Data[6] = 64;
  TxMessage.Data[7] = 64;
  CAN_Transmit(CAN2, &TxMessage);

}
void CAN2_BroadcastMode(uint8_t mode, uint8_t safetystatus){

  CanTxMsg TxMessage;

  /* Transmit Structure preparation */
  TxMessage.StdId = 0x321;
  TxMessage.ExtId = 0x1EEE0101;
  TxMessage.RTR = CAN_RTR_DATA;
  TxMessage.IDE = CAN_ID_EXT;
  TxMessage.DLC = 2;
  TxMessage.Data[0] = mode;
  TxMessage.Data[1] = safetystatus;
  CAN_Transmit(CAN2, &TxMessage);

}

CanRxMsg CAN2_PassRXMessage(uint8_t messageID){


  return CANMsgDecoded[messageID];
}


uint8_t CAN2_CheckMessageStatusFlag(uint8_t messageID){

    if( messageID == CAN_MESSAGE_MAGNETOMETER){

          if ( canMessageFlag.MagnetometerUpdated == 1)
          {
            return 1;
          }else{

            return 0;
          }



    }else if(messageID == CAN_MESSAGE_BAROMETER){


          if ( canMessageFlag.BarometerUpdated == 1)
          {
            return 1;
          }else{

            return 0;
          }
    }

    return 0;
}

void CAN2_ClearMessageStatusFlag(uint8_t messageID){

    if( messageID == CAN_MESSAGE_MAGNETOMETER){

          canMessageFlag.MagnetometerUpdated = 0;

    }else if(messageID == CAN_MESSAGE_BAROMETER){


          canMessageFlag.BarometerUpdated = 0;
    }
}
