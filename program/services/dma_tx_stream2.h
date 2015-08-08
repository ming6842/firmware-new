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

} uart_streaming_fs_t;


void DMA1_Stream6_IRQHandler(void);
void uartTX_stream_initialize(uart_streaming_fs_t* uart_fs);
ErrorMessage uartTX_stream_append_data_to_buffer(uart_streaming_fs_t* uart_fs, uint8_t *s,uint16_t len, DMATransmitTaskID task_id);
DMATriggerStatus  uartTX_stream_dma_trigger(uart_streaming_fs_t* uart_fs);
DMATXTransmissionResult  uartTX_stream_write(uart_streaming_fs_t* uart_fs,uint8_t *s,uint16_t len, DMATransmitTaskID task_id,FailureHandler routineIfFailed, TCHandler waitcomplete,uint32_t blockTime_ms);
uint32_t uartTX_stream_getTransmittedBytes(void);
uint32_t uartTX_stream_getTransmissionRate(float updateRateHz);


void uart2_tx_stream_initialize(void);
ErrorMessage uart2_tx_stream_append_data_to_buffer(uint8_t *s,uint16_t len, DMATransmitTaskID task_id);
DMATriggerStatus uart2_tx_stream_dma_trigger(void);
DMATXTransmissionResult uart2_tx_stream_write( uint8_t *s,uint16_t len, DMATransmitTaskID task_id,FailureHandler routineIfFailed, TCHandler waitcomplete,uint32_t blockTime_ms);


#endif
