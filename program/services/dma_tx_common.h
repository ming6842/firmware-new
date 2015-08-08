#ifndef FILE_DMA_TX_COMMON_H
#define FILE_DMA_TX_COMMON_H

#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "usart.h"

/************************** Streaming TX Service definition ****************************************/


typedef enum {INACTIVE = 0, ACTIVE = !INACTIVE} BufferActiveStatus;

typedef enum {
	DMA_TX_TaskID_MAIN=0,
	DMA_TX_TaskID_FLIGHT_STABILIZER,
	DMA_TX_TaskID_FLIGHT_CONTROLLER,
	DMA_TX_TaskID_NAVIGATION,
	DMA_TX_TaskID_MAVLINK,
	DMA_TX_TaskID_MAVLINK_PARAMETER,
	DMA_TX_TaskID_MAVLINK_MISSION,
	DMA_TX_TaskID_MAVLINK_WAYPOINT,
	DMA_TX_TaskID_FCU_DUMMY,
	DMA_TX_TaskID_DUMMY1,
	DMA_TX_TaskID_DUMMY2,
	DMA_TX_TaskID_DUMMY3,
	DMA_TX_TaskID_DUMMY4,
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



#endif