#include "cmsis_os.h"
#include "telemetry.h"
#include "data.h"

extern osSemaphoreId sendDataSemaphoreHandle;
extern osSemaphoreId sendStateSemaphoreHandle;
extern osSemaphoreId sendErrorSemaphoreHandle;
extern osSemaphoreId receiveCommandSemaphoreHandle;
extern osSemaphoreId uart1SemHandle;
extern osSemaphoreId sendFollowingDataSemaphoreHandle;
extern osMessageQId Usart1TxModeQueueHandle;


extern inline void usart1TxCallback(void)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE; 																							/* Variable goes to true when contxt-switch is needed */
	uint8_t queueUartTransmitFlag; 																															/* Receive from queue and store into this variable */
	
	xQueueReceiveFromISR(Usart1TxModeQueueHandle, (void *)&queueUartTransmitFlag, &xHigherPriorityTaskWoken);
	
	/* Check which flag you have received and unlock the correct semaphore */
	switch(queueUartTransmitFlag) {
		
		/* To send second part of data, unlock the secont part tx task and keep the UART semaphore locked to avoit its use from other tasks */
		case SECOND_HALF_TX_FLAG:
			if(sendFollowingDataSemaphoreHandle != NULL) { 																					/* Check on system start if semaphore is already created */
				xSemaphoreGiveFromISR(sendFollowingDataSemaphoreHandle, &xHigherPriorityTaskWoken); 	/* Give semaphore to task when DMA is clear */
				portYIELD_FROM_ISR(xHigherPriorityTaskWoken); 																				/* Do context-switch if needed */
			}
			break;
		
		/* To return to normal mode, unlock the UART locking semaphore */
		case NORMAL_MODE_TX_FLAG: 
			if(uart1SemHandle != NULL) { 																														/* Check on system start if semaphore is already created */
				xSemaphoreGiveFromISR(uart1SemHandle, &xHigherPriorityTaskWoken);											/* Give semaphore to task when DMA is clear */
				portYIELD_FROM_ISR(xHigherPriorityTaskWoken); 																				/* Do context-switch if needed */
			}
			break;
		
		/* If the flag is unrecognised, raise eventually an error and return to normal mode */
		default:
			/* Eventual error throwing code to be inserted here */
			if(uart1SemHandle != NULL) { 																														/* Check on system start if semaphore is already created */
				xSemaphoreGiveFromISR(uart1SemHandle, &xHigherPriorityTaskWoken); 										/* Give semaphore to task when DMA is clear */
				portYIELD_FROM_ISR(xHigherPriorityTaskWoken); 																				/* Do context-switch if needed */
			}
			break;
	}
}

extern inline void usart1RxCallback(void)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE; 																							/* Variable goes to true when contxt-switch is needed */
	if(receiveCommandSemaphoreHandle != NULL) { 																								/* Check on system start if semaphore is already created */
		xSemaphoreGiveFromISR(receiveCommandSemaphoreHandle, &xHigherPriorityTaskWoken); 					/* Give semaphore to task when DMA is clear */
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken); 																						/* Do context-switch if needed */
	}
}

extern inline void stateSendTimCallback(void)
{	
	BaseType_t xHigherPriorityTaskWoken = pdFALSE; 																							/* Variable goes to true when contxt-switch is needed */
	if(sendStateSemaphoreHandle!=NULL) { 																												/* Check on system start if semaphore is already created */
		xSemaphoreGiveFromISR(sendStateSemaphoreHandle, &xHigherPriorityTaskWoken); 							/* Give semaphore to task when DMA is clear */
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken); 																						/* Do context-switch if needed */
	}
}

extern inline void dataSendTimCallback(void)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE; 																							/* Variable goes to true when contxt-switch is needed */
	if(sendDataSemaphoreHandle!=NULL) { 																												/* Check on system start if semaphore is already created */
		xSemaphoreGiveFromISR(sendDataSemaphoreHandle, &xHigherPriorityTaskWoken); 								/* Give semaphore to task when DMA is clear */
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken); 																						/* Do context-switch if needed */
	}
}
