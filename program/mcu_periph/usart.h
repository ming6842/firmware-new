#ifndef FILE_USART_H
#define FILE_USART_H
#include <stdint.h>
#include <stdbool.h>

#include "led.h"
#include "delay.h"


#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#define USART_NOT_AVAILABLE -1

typedef struct {
        char ch;
} serial_msg;


void usart_init(void);
void retarget_init(void);
int _write(int fd, char *ptr, int len);
int _read(int fd, char *ptr, int len);
void _ttywrch(int ch);
void usart2_dma_send(uint8_t *s);
void usart2_dma_init(void);
void usart2_dma_burst_send(uint8_t *s,uint16_t len);
void enable_usart2_dma_interrupt(void);

int usart3_read(uint32_t delay_tick);
void usart3_send(char str);
bool is_usart3_rx_available(void);
void USART3_IRQHandler(void);
void uart8_puts(uint8_t *ptr);
void DMA1_Stream3_IRQHandler(void);
void usart3_dma_send(uint8_t *, uint16_t );
void usart3_dma_burst_send(uint8_t *s,uint16_t len);
void enable_usart3_dma_interrupt(void);

/************************** Streaming TX Service definition ****************************************/


typedef enum {INACTIVE = 0, ACTIVE = !INACTIVE} BufferActiveStatus;

typedef enum {
	DMA_TX_TaskID_MAIN=0,
	DMA_TX_TaskID_GPS_DECODER,
	DMA_TX_TaskID_FLIGHT_STABILIZER,
	DMA_TX_TaskID_FLIGHT_CONTROLLER,
	DMA_TX_TaskID_NAVIGATION,
	DMA_TX_TaskID_MAVLINK,
	DMA_TX_TaskID_MAVLINK_BROADCAST,
	DMA_TX_TaskID_MAVLINK_RECEIVER,
	DMA_TX_TaskID_EEPROM_MANAGER_TASK,
	DMA_TX_TaskID_COUNT
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
	DMA_TX_FH_NoRetry=0,
	DMA_TX_FH_WaitReadySemaphore
} FailureHandler;


typedef enum {
	DMA_TX_TCH_NoWait=0,
	DMA_TX_TCH_WaitCompleteSemaphore

}TCHandler ;

typedef enum {
	DMA_TX_Result_AppendedIntoBuffer=0,
	DMA_TX_Result_BufferAppended_CompleteTimedout,
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

typedef struct uart_streaming_fs_t{

	uart_dma_tx_buffer_t dma_tx_buffer[2];
	uint8_t dmaISRTransmissionCompleteFlag;
	DMATriggerStatus dma_trigger_current_status;
	xSemaphoreHandle dma_tx_bufferAvailableSemaphore[16];
	xSemaphoreHandle dma_tx_DMAWaitCompleteSemaphore[16];
	uint32_t total_transmitted_bytes;
	uint32_t prev_transmitted_bytes;
	void (*dma_send)(uint8_t *s,uint16_t len);
} uart_streaming_fs_t;


void DMA1_Stream6_IRQHandler(void);
void uart2_tx_stream_initialize(void);
ErrorMessage uart2_tx_stream_append_data_to_buffer(uint8_t *s,uint16_t len, DMATransmitTaskID task_id);
DMATriggerStatus uart2_tx_stream_dma_trigger(void);
DMATXTransmissionResult uart2_tx_stream_write( uint8_t *s,uint16_t len, DMATransmitTaskID task_id,FailureHandler routineIfFailed, TCHandler waitcomplete,uint32_t blockTime_ms);
uint32_t uart2_tx_stream_getTransmittedBytes(void);
uint32_t uart2_tx_stream_getTransmissionRate(float updateRateHz);


void uart3_tx_stream_initialize(void);
ErrorMessage uart3_tx_stream_append_data_to_buffer(uint8_t *s,uint16_t len, DMATransmitTaskID task_id);
DMATriggerStatus uart3_tx_stream_dma_trigger(void);
DMATXTransmissionResult uart3_tx_stream_write( uint8_t *s,uint16_t len, DMATransmitTaskID task_id,FailureHandler routineIfFailed, TCHandler waitcomplete,uint32_t blockTime_ms);
uint32_t uart3_tx_stream_getTransmittedBytes(void);
uint32_t uart3_tx_stream_getTransmissionRate(float updateRateHz);

void UART_TX_service_task(void);

#define flight_control_serial_write_mavlink(buffer,length) uart3_tx_stream_write(buffer,length,  DMA_TX_TaskID_FLIGHT_CONTROLLER, DMA_TX_FH_NoRetry , DMA_TX_TCH_NoWait ,30)
#define flight_control_serial_write_stream2(buffer,length) uart2_tx_stream_write(buffer,length,  DMA_TX_TaskID_FLIGHT_CONTROLLER, DMA_TX_FH_NoRetry , DMA_TX_TCH_NoWait ,30)

#define mavlink_receiver_serial_write(buffer,length) uart3_tx_stream_write(buffer,length,  DMA_TX_TaskID_MAVLINK_RECEIVER, DMA_TX_FH_WaitReadySemaphore , DMA_TX_TCH_NoWait ,30)
#define status_mavlink_serial_write(buffer,length) uart3_tx_stream_write(buffer,length,  DMA_TX_TaskID_MAVLINK_BROADCAST, DMA_TX_FH_WaitReadySemaphore , DMA_TX_TCH_NoWait ,30)

#endif
