#include "telemetry.h"
#include "cmsis_os.h"
#include "usart.h"
#include "data.h"

uint8_t telemetryReceivedBuffer [BUFFER_COMMAND_LEN];
uint8_t setRtcReceivedBuffer [BUFFER_RTC_SET_LEN];
uint8_t commandAckMsg [COMMAND_ACK_MSG_LEN] = ACK_MSG;
uint8_t tempBuffer [50];
uint8_t errorLetter = CMD_READ_ERR_ID;
uint8_t usartLockFlag;
_Bool setRtcComing = 0;
BaseType_t UART1_StateSendxHigherPriorityTaskWoken = pdFALSE;
BaseType_t UART1_DataSendxHigherPriorityTaskWoken = pdFALSE;
extern osSemaphoreId sendDataSemaphoreHandle;
extern osSemaphoreId sendStateSemaphoreHandle;
extern osMessageQId Usart1LockQueueHandle;
extern osMessageQId ErrorQueueHandle;
extern osMessageQId startAcquisitionEventHandle;


extern inline void TELEMETRY_StateSendTimCallback(void)
{	
	if(sendStateSemaphoreHandle!=NULL) { 																														/* Check on system start if semaphore is already created */
		xSemaphoreGiveFromISR(sendStateSemaphoreHandle, &UART1_StateSendxHigherPriorityTaskWoken); 		/* Give semaphore to task when DMA is clear */
		portYIELD_FROM_ISR(UART1_StateSendxHigherPriorityTaskWoken); 																	/* Do context-switch if needed */
	}
}

extern inline void TELEMETRY_DataSendTimCallback(void)
{	
	if(sendDataSemaphoreHandle!=NULL) { 																														/* Check on system start if semaphore is already created */
		xSemaphoreGiveFromISR(sendDataSemaphoreHandle, &UART1_DataSendxHigherPriorityTaskWoken); 			/* Give semaphore to task when DMA is clear */
		portYIELD_FROM_ISR(UART1_DataSendxHigherPriorityTaskWoken); 																	/* Do context-switch if needed */
	}
}

extern inline void TELEMETRY_Receive(void)
{
	uint8_t startAquisitionEvent = ACQUISITION_IDLE_REQUEST;
	BaseType_t startAcquisition_xHigherPriorityTaskWoken = pdFALSE;
	
	if(setRtcComing == 1) { 																															/* If you are waiting for set rtc parameter */
		if(setRtcReceivedBuffer[BUFFER_RTC_SET_LEN - 1] == MESSAGE_END_ID) { 								/* Check if message ends correctly */
			/* Put here the code for the RTC setting */
			xQueueReceive(Usart1LockQueueHandle, &usartLockFlag, portMAX_DELAY);							/* Lock if DMA is in use */
			commandAckMsg[COMMAND_ACK_IDENTIFIER_POS] = SET_RTC_ID; 													/* Set the correct identifier */
			HAL_UART_Transmit_DMA(&huart1, commandAckMsg, COMMAND_ACK_MSG_LEN); 							/* Transmit ack message */
			HAL_UART_Receive_DMA(&huart1, telemetryReceivedBuffer, BUFFER_COMMAND_LEN); 			/* Re enable receiving */
		}
		else { 																																							/* If message does not end correctly */
			xQueueSend(ErrorQueueHandle, (void *)&errorLetter, (TickType_t)0);			 					/* Add error to queue */
			HAL_UART_Receive_DMA(&huart1, telemetryReceivedBuffer, BUFFER_COMMAND_LEN); 			/* Re enable receiving */
		}
	}
	
	else { 																																								/* If you are waiting for standard message */
		if(telemetryReceivedBuffer[0] == MESSAGE_INIT_ID) { 																/* If message starts correctly with [ */
			if(telemetryReceivedBuffer[BUFFER_COMMAND_LEN - 1] == MESSAGE_END_ID) { 					/* If message ends correctly as standard message */
					
				switch(telemetryReceivedBuffer[BUFFER_COMMAND_LEN - 2]) {
					case RESET_TELEM_ID:																													
						DATA_ResetTelemetryState();																									/* Reset telemetry */
						xQueueReceive(Usart1LockQueueHandle, &usartLockFlag, portMAX_DELAY);				/* Lock if DMA is in use */
						commandAckMsg[COMMAND_ACK_IDENTIFIER_POS] = RESET_TELEM_ID; 								/* Set the correct identifier */
						HAL_UART_Transmit_DMA(&huart1, commandAckMsg, COMMAND_ACK_MSG_LEN); 				/* Transmit ack message */
						break;
		
					case START_ACQ_ID:
						startAquisitionEvent = ACQUISITION_ON_TELEMETRY_REQUEST;										/* Start acquisition */
						xQueueSendFromISR(startAcquisitionEventHandle, &startAquisitionEvent, &startAcquisition_xHigherPriorityTaskWoken);
						xQueueReceive(Usart1LockQueueHandle, &usartLockFlag, portMAX_DELAY);				/* Lock if DMA is in use */
						commandAckMsg[COMMAND_ACK_IDENTIFIER_POS] = START_ACQ_ID; 									/* Set the correct identifier */
						HAL_UART_Transmit_DMA(&huart1, commandAckMsg, COMMAND_ACK_MSG_LEN); 				/* Transmit ack message */
						break;	

					case STOP_ACQ_ID:
						startAquisitionEvent = ACQUISITION_OFF_TELEMETRY_REQUEST;										/* Stop acquisition */
						xQueueSendFromISR(startAcquisitionEventHandle, &startAquisitionEvent, &startAcquisition_xHigherPriorityTaskWoken);			
						xQueueReceive(Usart1LockQueueHandle, &usartLockFlag, portMAX_DELAY);				/* Lock if DMA is in use */
						commandAckMsg[COMMAND_ACK_IDENTIFIER_POS] = STOP_ACQ_ID; 										/* Set the correct identifier */
						HAL_UART_Transmit_DMA(&huart1, commandAckMsg, COMMAND_ACK_MSG_LEN); 				/* Transmit ack message */
						break;

					case GIVE_RTC_TIME_ID:
						/* Put here the code necessary for giving RTC time */
						xQueueReceive(Usart1LockQueueHandle, &usartLockFlag, portMAX_DELAY);				/* Lock if DMA is in use */
						commandAckMsg[COMMAND_ACK_IDENTIFIER_POS] = GIVE_RTC_TIME_ID; 							/* Set the correct identifier */
						HAL_UART_Transmit_DMA(&huart1, commandAckMsg, COMMAND_ACK_MSG_LEN); 				/* Transmit ack message */
						break;

					case GIVE_RTC_DATE_ID:
						/* Here put the code necessary for giving RTC data */
						xQueueReceive(Usart1LockQueueHandle, &usartLockFlag, portMAX_DELAY);				/* Lock if DMA is in use */
						commandAckMsg[COMMAND_ACK_IDENTIFIER_POS] = GIVE_RTC_DATE_ID; 							/* Set the correct identifier */
						HAL_UART_Transmit_DMA(&huart1, commandAckMsg, COMMAND_ACK_MSG_LEN); 				/* Transmit ack message */
						break;

					default:																																			/* Message not recognised */
						xQueueSend(ErrorQueueHandle, (void *)&errorLetter, (TickType_t)0); 					/* Add error to queue */
						break;
					}
				
				HAL_UART_Receive_DMA(&huart1, telemetryReceivedBuffer, BUFFER_COMMAND_LEN); 		/* Re enable receiving */
			}
			
			else { 																																						/* If message does not end here */
				if(telemetryReceivedBuffer[BUFFER_COMMAND_LEN - 2] == SET_RTC_ID) { 						/* If the message type is set RTC, wait for the parameters */
					setRtcComing = 1;
					HAL_UART_Receive_DMA(&huart1, setRtcReceivedBuffer, BUFFER_RTC_SET_LEN); 			/* Re enable receiving, wait for parameter */
				}
				
				else { 																																					/* Not set RTC message */
					HAL_UART_Receive_DMA(&huart1, telemetryReceivedBuffer, BUFFER_COMMAND_LEN); 	/* Re enable receiving */
					xQueueSend(ErrorQueueHandle, (void *)&errorLetter, (TickType_t)0); 						/* Add error to queue */
				}
			}
		}
		
		else {																																							/* If message does not start correctly */
			HAL_UART_Receive(&huart1, tempBuffer, 50, 50);
			HAL_UART_Receive_DMA(&huart1, telemetryReceivedBuffer, BUFFER_COMMAND_LEN); 			/* Re enable receiving */
			xQueueSend(ErrorQueueHandle, ( void * ) &errorLetter, ( TickType_t ) 0 ); 				/* Add error to queue */				
		}
	}
}
