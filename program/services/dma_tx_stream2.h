#ifndef FILE_DMA_TX_STREAM2_H
#define FILE_DMA_TX_STREAM2_H

#include "dma_tx_common.h"


void DMA1_Stream6_IRQHandler(void);
void uartTX_stream2_initialize(void);
ErrorMessage uartTX_stream2_append_data_to_buffer(uint8_t *s,uint16_t len, DMATransmitTaskID task_id);
DMATriggerStatus  uartTX_stream2_dma_trigger(void);
DMATXTransmissionResult  uartTX_stream2_write(uint8_t *s,uint16_t len, DMATransmitTaskID task_id,FailureHandler routineIfFailed, TCHandler waitcomplete,uint32_t blockTime_ms);
uint32_t uartTX_stream2_getTransmittedBytes(void);
uint32_t uartTX_stream2_getTransmissionRate(float updateRateHz);

#endif
