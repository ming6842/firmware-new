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
	DMA_TX_Result_BufferAppended_CompleteTimedout,
	DMA_TX_Result_TransmissionComplete,
	DMA_TX_Result_TransmissionSkipped,
	DMA_TX_Result_TransmissionFailed

}DMATXTransmissionResult ;



#endif