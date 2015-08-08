/* UART DMA TX service functions */

#include "dma_tx_stream2.h"





/************************** Streaming TX Service ****************************************/

static xSemaphoreHandle dma_tx_bufferAvailableSemaphore[16];
static xSemaphoreHandle dma_tx_DMAWaitCompleteSemaphore[16];
static uint32_t total_transmitted_bytes = 0;


static uart_dma_tx_buffer_t dma_tx_buffer[2] = {


	/* Memory 0 initialization, set this one to acitve first */
	{
		.currentIndex = 0,
		.accessingFlag = 0,
		.bufferAvailableSemRequestFlag = 0,
		.waitCompleteSemRequestFlag = 0,
		.DMATransmittingFlag = BUFFER_STATUS_BufferFilling,
		.buffer[0 ... (configUSART_DMA_TX_BUFFER_SIZE-1)] = 0
	},

	/* Memory 1 initialization */
	{
		.currentIndex = 0,
		.accessingFlag = 0,
		.bufferAvailableSemRequestFlag = 0,
		.waitCompleteSemRequestFlag = 0,
		.DMATransmittingFlag = BUFFER_STATUS_DMAIdle,
		.buffer[0 ... (configUSART_DMA_TX_BUFFER_SIZE-1)] = 0
	}
};

static uint8_t DMA1_Stream6_TransmissionCompleteFlag = 0;
static DMATriggerStatus dma_trigger_current_status = DMA_TRIGGER_STATUS_WaitingForData;

/* ********************************************************* */

uart_streaming_fs_t uart2_fs;

void uartTX_stream_initialize(uart_streaming_fs_t* uart_fs){


	/* Memory 0 initialization, set this one to acitve first */
	uart_fs-> dma_tx_buffer[0].currentIndex = 0;
	uart_fs-> dma_tx_buffer[0].accessingFlag = 0;
	uart_fs-> dma_tx_buffer[0].bufferAvailableSemRequestFlag = 0;
	uart_fs-> dma_tx_buffer[0].waitCompleteSemRequestFlag = 0;
	uart_fs-> dma_tx_buffer[0].DMATransmittingFlag = BUFFER_STATUS_BufferFilling;
	// uart_fs-> dma_tx_buffer[0].buffer[0 ... (configUSART_DMA_TX_BUFFER_SIZE-1)] = 0;


	/* Memory 1 initialization */
	uart_fs-> dma_tx_buffer[1].currentIndex = 0;
	uart_fs-> dma_tx_buffer[1].accessingFlag = 0;
	uart_fs-> dma_tx_buffer[1].bufferAvailableSemRequestFlag = 0;
	uart_fs-> dma_tx_buffer[1].waitCompleteSemRequestFlag = 0;
	uart_fs-> dma_tx_buffer[1].DMATransmittingFlag = BUFFER_STATUS_DMAIdle;
	// uart_fs-> dma_tx_buffer[1].buffer[0 ... (configUSART_DMA_TX_BUFFER_SIZE-1)] = 0;



	uint8_t i=0;

	for (i = 0; i < 16; i++)
	{

		dma_tx_bufferAvailableSemaphore[i] = NULL;
		dma_tx_DMAWaitCompleteSemaphore[i] = NULL;
		
	}

	for (i = 0; i < 16; i++)
	{
		
		vSemaphoreCreateBinary(dma_tx_bufferAvailableSemaphore[i]);
		vSemaphoreCreateBinary(dma_tx_DMAWaitCompleteSemaphore[i]);
	}

	enable_usart2_dma_interrupt();

}


void DMA1_Stream6_IRQHandler(void)
{
	
	if( DMA_GetITStatus(DMA1_Stream6, DMA_IT_TCIF6) != RESET) {

		DMA1_Stream6_TransmissionCompleteFlag = 1;
		DMA_ClearITPendingBit(DMA1_Stream6, DMA_IT_TCIF6);
	}

}


ErrorMessage uartTX_stream_append_data_to_buffer(uart_streaming_fs_t* uart_fs, uint8_t *s,uint16_t len, DMATransmitTaskID task_id){

	ErrorMessage errorStatus = NO_ERROR;
	uint8_t selected_buffer;
	/* Check for active buffer to fill data into it */
	if(uart_fs-> dma_tx_buffer[0].DMATransmittingFlag == BUFFER_STATUS_BufferFilling){

		selected_buffer = 0;

	}else if(uart_fs-> dma_tx_buffer[1].DMATransmittingFlag == BUFFER_STATUS_BufferFilling){

		selected_buffer = 1;

	}else {
	/* No available buffer, return error */
		errorStatus = NO_ACTIVE_BUFFER;
		return errorStatus;
	}

	/* Check if busy flag is already set */
	if(( uart_fs-> dma_tx_buffer[selected_buffer].accessingFlag & (uint32_t)(1<<task_id)) == 1){

		errorStatus = PERMISSION_ALREADY_OCCUPIED;
		return errorStatus;
	}

	/* Check if buffer size is enough for the packet */
	if (len <(configUSART_DMA_TX_BUFFER_SIZE - uart_fs-> dma_tx_buffer[selected_buffer].currentIndex)){

		/* Shift current index of buffer away to reserve space */
		uint16_t startIndex = uart_fs-> dma_tx_buffer[selected_buffer].currentIndex;
		uart_fs-> dma_tx_buffer[selected_buffer].currentIndex += len;

		/* set occupy flag */
		uart_fs-> dma_tx_buffer[selected_buffer].accessingFlag = uart_fs-> dma_tx_buffer[selected_buffer].accessingFlag | (uint32_t)(1<<task_id);

		/* Fill in the buffer */
		memcpy(&uart_fs-> dma_tx_buffer[selected_buffer].buffer[startIndex],s,len);

		/* Reset occupy flag */
		uart_fs-> dma_tx_buffer[selected_buffer].accessingFlag = uart_fs-> dma_tx_buffer[selected_buffer].accessingFlag & ~(uint32_t)(1<<task_id);


	}else{

		/* Not enough buffer space */
		errorStatus = BUFFER_FULL;
		return errorStatus;

	 }

	return errorStatus;
}

DMATriggerStatus uartTX_stream_dma_trigger(uart_streaming_fs_t* uart_fs){

	uint8_t current_buffer;
	/* Get current trigger condition */

	switch(dma_trigger_current_status ){
		case DMA_TRIGGER_STATUS_WaitingForData:

			/* Check for active buffer to fill data into it */
			if(uart_fs-> dma_tx_buffer[0].DMATransmittingFlag == BUFFER_STATUS_BufferFilling){

				current_buffer = 0;

			}else if(uart_fs-> dma_tx_buffer[1].DMATransmittingFlag == BUFFER_STATUS_BufferFilling){

				current_buffer = 1;

			}

			/* check if there is any data inside buffer */
			if(uart_fs-> dma_tx_buffer[current_buffer].currentIndex >0){

				/* Close current buffer */
				uart_fs-> dma_tx_buffer[current_buffer].DMATransmittingFlag = BUFFER_STATUS_ClosedWaitForTransmit;

				/* Enable and reset counter for another buffer */
				if(current_buffer == 0){
					uart_fs-> dma_tx_buffer[1].DMATransmittingFlag = BUFFER_STATUS_BufferFilling;
					uart_fs-> dma_tx_buffer[1].currentIndex = 0;

				}else{
					uart_fs-> dma_tx_buffer[0].DMATransmittingFlag = BUFFER_STATUS_BufferFilling;
					uart_fs-> dma_tx_buffer[0].currentIndex = 0;
				}


				/* Give semaphore to wake up the requested task (buffer available) */
				int i;
				for(i=0; i<DMA_TX_TaskID_COUNT ;i++){

					 if((uart_fs-> dma_tx_buffer[!current_buffer].bufferAvailableSemRequestFlag & (uint32_t)(1<<i)) != 0){

					 		/* Give semaphore */
							xSemaphoreGive( dma_tx_bufferAvailableSemaphore[i]);

					 }

				}

				/* Clear the buffer available request sem flag */
				uart_fs-> dma_tx_buffer[!current_buffer].bufferAvailableSemRequestFlag =0;

				/* check for access flag */

				if(uart_fs-> dma_tx_buffer[current_buffer].accessingFlag ==0){

					/* Not busy, ready for transmit */

					//////////////Set DMA////////////
					usart2_dma_burst_send(uart_fs-> dma_tx_buffer[current_buffer].buffer,uart_fs-> dma_tx_buffer[current_buffer].currentIndex);
					/* Set status flags */
					uart_fs-> dma_tx_buffer[current_buffer].DMATransmittingFlag = BUFFER_STATUS_DMATransmitting;
					dma_trigger_current_status = DMA_TRIGGER_STATUS_WaitingForDMATransmissionComplete;
				}else{

					/* Some task is still filling in buffer, put trigger on pending */
					dma_trigger_current_status = DMA_TRIGGER_STATUS_WaitAccessFlag;
				}


			}else{

				/* No data inside buffer, no need to transmit */

			}


 
		break;

		case DMA_TRIGGER_STATUS_WaitingForDMATransmissionComplete:

			if( DMA1_Stream6_TransmissionCompleteFlag == 1){

				/* Check which buffer was being transmitted */

				/* Check which buffer is waiting to send */
				if(uart_fs-> dma_tx_buffer[0].DMATransmittingFlag == BUFFER_STATUS_DMATransmitting){

					current_buffer = 0;

				}else if(uart_fs-> dma_tx_buffer[1].DMATransmittingFlag == BUFFER_STATUS_DMATransmitting){

					current_buffer = 1;

				}

				/* clear the flag !! */
				DMA1_Stream6_TransmissionCompleteFlag = 0;
				/* Transmission is now complete, set the flag to idle */
				uart_fs-> dma_tx_buffer[current_buffer].DMATransmittingFlag = BUFFER_STATUS_DMAIdle;

				/* Record the transmitted bytes to log */
				total_transmitted_bytes += uart_fs-> dma_tx_buffer[current_buffer].currentIndex;

				/* Set trigger state to send mode */
				dma_trigger_current_status = DMA_TRIGGER_STATUS_WaitingForData;

				/* Give semaphore to wake up the requested task (send finished) */

				int i;
				for(i=0; i<DMA_TX_TaskID_COUNT ;i++){

					 if((uart_fs-> dma_tx_buffer[current_buffer].waitCompleteSemRequestFlag & (uint32_t)(1<<i)) != 0){

					 		/* Give semaphore */
							xSemaphoreGive( dma_tx_DMAWaitCompleteSemaphore [i]);

					 }

				}
			}else{

				/* Go and drink coffee dude ~ nothing to do now */
			}

		break;

		case DMA_TRIGGER_STATUS_WaitAccessFlag:


			/* Check which buffer is waiting to send */
			if(uart_fs-> dma_tx_buffer[0].DMATransmittingFlag == BUFFER_STATUS_ClosedWaitForTransmit){

				current_buffer = 0;

			}else if(uart_fs-> dma_tx_buffer[1].DMATransmittingFlag == BUFFER_STATUS_ClosedWaitForTransmit){

				current_buffer = 1;

			}

			if(uart_fs-> dma_tx_buffer[current_buffer].accessingFlag == 0){

						/* Not busy, ready for transmit */

						//////////////Set DMA////////////
						// usart2_dma_burst_send(buffer1,10);
						
						usart2_dma_burst_send(uart_fs-> dma_tx_buffer[current_buffer].buffer,uart_fs-> dma_tx_buffer[current_buffer].currentIndex);
						/* Set status flags */
						uart_fs-> dma_tx_buffer[current_buffer].DMATransmittingFlag = BUFFER_STATUS_DMATransmitting;
						dma_trigger_current_status = DMA_TRIGGER_STATUS_WaitingForDMATransmissionComplete;
			}else{

				/* keep waiting */

			}

		break;

	}

	return dma_trigger_current_status;

}

DMATXTransmissionResult uartTX_stream_write(uart_streaming_fs_t* uart_fs, uint8_t *s,uint16_t len, DMATransmitTaskID task_id,FailureHandler routineIfFailed, TCHandler waitcomplete,uint32_t blockTime_ms){

	uint8_t transmissionResult = DMA_TX_Result_TransmissionFailed;
	uint8_t shouldEnd=0;
	uint8_t req_buffer;
	ErrorMessage err;

	while(!shouldEnd){

		err = uartTX_stream_append_data_to_buffer(uart_fs, s,len, task_id);

		if(err == NO_ERROR){

			shouldEnd = 1;
			transmissionResult = DMA_TX_Result_AppendedIntoBuffer;

		}else if(err == BUFFER_FULL){

			/* Handle buffer full according to user choice */
			if(routineIfFailed == DMA_TX_FH_WaitReadySemaphore){

			/* Set flag to request semaphore when buffer is available */
				/* Check which buffer is being filled */
				if(uart_fs-> dma_tx_buffer[0].DMATransmittingFlag == BUFFER_STATUS_BufferFilling){
					req_buffer = 1;
					/* set request flag into another buffer */
					uart_fs-> dma_tx_buffer[1].bufferAvailableSemRequestFlag = uart_fs-> dma_tx_buffer[1].bufferAvailableSemRequestFlag | (uint32_t)(1<<task_id);


				}else if(uart_fs-> dma_tx_buffer[1].DMATransmittingFlag == BUFFER_STATUS_BufferFilling){
					req_buffer = 0;

					/* set request flag into another buffer */
					uart_fs-> dma_tx_buffer[0].bufferAvailableSemRequestFlag = uart_fs-> dma_tx_buffer[0].bufferAvailableSemRequestFlag | (uint32_t)(1<<task_id);

				}


				/* Wait semaphore */

					if(xSemaphoreTake(dma_tx_bufferAvailableSemaphore[task_id], (uint32_t)blockTime_ms/(1000.0 / configTICK_RATE_HZ)) == pdTRUE){

						/* Just loop */

					}else{

						/* clear semaphore request flag */

					uart_fs-> dma_tx_buffer[req_buffer].bufferAvailableSemRequestFlag = uart_fs-> dma_tx_buffer[req_buffer].bufferAvailableSemRequestFlag & ~(uint32_t)(1<<task_id);

						/* quit */
						shouldEnd = 1;
						return DMA_TX_Result_TransmissionFailed;
					}

					/* Semaphore is taken now, let it roll again */

			}else{

				/* Just skip packet, exit now */
				return DMA_TX_Result_TransmissionSkipped;

			}

		}else{

			/* Transmission failed, routine error */
			shouldEnd = 1;
			transmissionResult = DMA_TX_Result_TransmissionFailed;
		}



	}


	/* Wait complete handler */

	/* Check if data is appended to buffer */
	if( transmissionResult == DMA_TX_Result_AppendedIntoBuffer){


		if(waitcomplete == DMA_TX_TCH_NoWait){

			/* No wait, just exit */
			return DMA_TX_Result_AppendedIntoBuffer;

		}else if(waitcomplete == DMA_TX_TCH_WaitCompleteSemaphore){


			if(uart_fs-> dma_tx_buffer[0].DMATransmittingFlag == BUFFER_STATUS_BufferFilling){

				/* set request flag into current buffer */
				uart_fs-> dma_tx_buffer[0].waitCompleteSemRequestFlag = uart_fs-> dma_tx_buffer[0].waitCompleteSemRequestFlag | (uint32_t)(1<<task_id);


			}else if(uart_fs-> dma_tx_buffer[1].DMATransmittingFlag == BUFFER_STATUS_BufferFilling){

				/* set request flag into current buffer */
				uart_fs-> dma_tx_buffer[1].waitCompleteSemRequestFlag = uart_fs-> dma_tx_buffer[1].waitCompleteSemRequestFlag | (uint32_t)(1<<task_id);

			}

			/* Wait transmission complete semaphore */

				if(xSemaphoreTake(dma_tx_DMAWaitCompleteSemaphore [task_id], (uint32_t)blockTime_ms/(1000.0 / configTICK_RATE_HZ)) == pdTRUE){

						return DMA_TX_Result_TransmissionComplete;

				}else{

						/* clear semaphore request flag */

				uart_fs-> dma_tx_buffer[0].waitCompleteSemRequestFlag = uart_fs-> dma_tx_buffer[0].waitCompleteSemRequestFlag & ~(uint32_t)(1<<task_id);


					return DMA_TX_Result_BufferAppended_CompleteTimedout;
				}



		}else {
			/* Should not come here */

			return 0x99;
		}



	}else{

		return DMA_TX_Result_TransmissionFailed;

	}

	return 0;

}

uint32_t uartTX_stream_getTransmittedBytes(void){

	return total_transmitted_bytes;

}
static uint32_t prev_transmitted_bytes=0;

uint32_t uartTX_stream_getTransmissionRate(float updateRateHz){

	uint32_t diff = total_transmitted_bytes - prev_transmitted_bytes;
	prev_transmitted_bytes = total_transmitted_bytes;

	return (uint32_t)((float)diff * updateRateHz);


}


/* UART2 specific code */

void uart2_tx_stream_initialize(void){

	uartTX_stream_initialize(&uart2_fs);

}

ErrorMessage uart2_tx_stream_append_data_to_buffer(uint8_t *s,uint16_t len, DMATransmitTaskID task_id){

	return uartTX_stream_append_data_to_buffer(&uart2_fs, s,len, task_id);


}


DMATriggerStatus uart2_tx_stream_dma_trigger(void){

	return uartTX_stream_dma_trigger(&uart2_fs);

}

DMATXTransmissionResult uart2_tx_stream_write( uint8_t *s,uint16_t len, DMATransmitTaskID task_id,FailureHandler routineIfFailed, TCHandler waitcomplete,uint32_t blockTime_ms){

	return uartTX_stream_write(&uart2_fs, s,len, task_id,routineIfFailed, waitcomplete,blockTime_ms);
}
