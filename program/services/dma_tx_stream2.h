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

void DMA1_Stream6_IRQHandler(void);
void streaming_dma_tx_initialize(void);
ErrorMessage streaming_dma_tx_append_data_to_buffer(uint8_t *s,uint16_t len, DMATransmitTaskID task_id);
DMATriggerStatus  streaming_dma_tx_dma_trigger(void);
DMATXTransmissionResult  streaming_dma_tx_write(uint8_t *s,uint16_t len, DMATransmitTaskID task_id,FailureHandler routineIfFailed, TCHandler waitcomplete,uint32_t blockTime_ms);
uint32_t streaming_dma_tx_getTransmittedBytes(void);
uint32_t streaming_dma_tx_getTransmissionRate(float updateRateHz);

#endif
