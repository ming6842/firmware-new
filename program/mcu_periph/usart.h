#ifndef FILE_USART_H
#define FILE_USART_H
#include <stdint.h>
#include "led.h"
#include "delay.h"
typedef struct {
        char ch;
} serial_msg;


/************************** Streaming TX Service ****************************************/


typedef enum {INACTIVE = 0, ACTIVE = !INACTIVE} BufferActiveStatus;

typedef enum {
	DMA_TX_Task_ID_MAIN=0,
	DMA_TX_Task_ID_FLIGHT_STABILIZER,
	DMA_TX_Task_ID_FLIGHT_CONTROLLER,
	DMA_TX_Task_ID_NAVIGATION,
	DMA_TX_Task_ID_MAVLINK,
	DMA_TX_Task_ID_MAVLINK_PARAMETER,
	DMA_TX_Task_ID_MAVLINK_MISSION,
	DMA_TX_Task_ID_MAVLINK_WAYPOINT,
	DMA_TX_Task_ID_FCU_DUMMY,
	DMA_TX_Task_ID_DUMMY1,
	DMA_TX_Task_ID_DUMMY2,
	DMA_TX_Task_ID_DUMMY3,
	DMA_TX_Task_ID_DUMMY4,
	DMA_TX_Task_ID_COUNT
} DMATransmitTaskID;

typedef enum {
	BUFFER_STATUS_BufferFilling = 0, 
	BUFFER_STATUS_ClosedWaitForTransmit, 
	BUFFER_STATUS_DMATransmitting, 
	BUFFER_STATUS_DMAIdle
} DMATransmitStatus;


typedef enum {
	NO_ERROR=0,
	BUFFER_FULL, 
	NO_ACTIVE_BUFFER,
	PERMISSION_ALREADY_OCCUPIED
} ErrorMessage;

typedef enum {
	DMA_TRIGGER_STATUS_WaitingForData=0,
	DMA_TRIGGER_STATUS_WaitingForDMATransmissionComplete,
	DMA_TRIGGER_STATUS_WaitAccessFlag

} DMATriggerStatus;


typedef enum {
	DMA_TX_FailureHandler_SkipCurrentPacket=0,
	DMA_TX_FailureHandler_WaitReadySemaphore
} FailureHandler;


typedef enum {
	DMA_TX_CompleteFlagHandler_NoWait=0,
	DMA_TX_CompleteFlagHandler_WaitCompleteSemaphore

}CompleteFlagHandler ;

typedef enum {
	DMA_TX_Result_AppendedIntoBuffer=0,
	DMA_TX_Result_TransmissionComplete,
	DMA_TX_Result_TransmissionSkipped,
	DMA_TX_Result_TransmissionFailed

}DMATXTransmissionResult ;

#define configUSART_DMA_TX_BUFFER_SIZE 256
typedef struct uart_dma_tx_buffer_t{

	uint16_t currentIndex;
	uint32_t accessingFlag;
	DMATransmitStatus DMATransmittingFlag;
	uint32_t bufferAvailableSemRequestFlag;
	uint32_t waitCompleteSemRequestFlag;
	uint8_t buffer[configUSART_DMA_TX_BUFFER_SIZE];

} uart_dma_tx_buffer_t;


void usart_init(void);
void retarget_init(void);
int _write(int fd, char *ptr, int len);
int _read(int fd, char *ptr, int len);
void _ttywrch(int ch);
void usart2_dma_send(uint8_t *s);
void usart2_dma_init(void);
void usart2_dma_burst_send(uint8_t *s,uint16_t len);
void enable_usart2_dma_interrupt(void);

char usart3_read(void);
void usart3_send(char str);
void USART3_IRQHandler(void);
void uart8_puts(uint8_t *ptr);
void DMA1_Stream3_IRQHandler(void);
void usart3_dma_send(uint8_t *, uint16_t );


void DMA1_Stream6_IRQHandler(void);
void streaming_dma_tx_initialize(void);
ErrorMessage streaming_dma_tx_append_data_to_buffer(uint8_t *s,uint16_t len, DMATransmitTaskID task_id);
DMATriggerStatus  streaming_dma_tx_dma_trigger(void);
DMATXTransmissionResult  streaming_dma_tx_write(uint8_t *s,uint16_t len, DMATransmitTaskID task_id,FailureHandler routineIfFailed, CompleteFlagHandler waitcomplete);




#endif
