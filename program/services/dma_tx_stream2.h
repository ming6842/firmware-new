#ifndef FILE_DMA_TX_STREAM2_H
#define FILE_DMA_TX_STREAM2_H

#include "dma_tx_common.h"


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

#endif
