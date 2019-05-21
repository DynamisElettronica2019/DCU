#include "telemetry.h"
#include "cmsis_os.h"
#include "usart.h"
#include "data.h"

extern osSemaphoreId sendDataSemaphoreHandle;
extern osSemaphoreId sendStateSemaphoreHandle;
extern osSemaphoreId sendErrorSemaphoreHandle;
extern osSemaphoreId receiveCommandSemaphoreHandle;
extern osSemaphoreId sendFollowingDataSemaphoreHandle;
extern osMessageQId Usart1TxModeQueueHandle;
extern osMessageQId Usart1LockQueueHandle;
extern osMessageQId ErrorQueueHandle;
extern osMessageQId startAcquisitionEventHandle;
uint8_t telemetryReceivedBuffer [BUFFER_COMMAND_LEN];
uint8_t setRtcReceivedBuffer [BUFFER_RTC_SET_LEN];
uint8_t commandAckMsg [COMMAND_ACK_MSG_LEN] = ACK_MSG;
uint8_t tempBuffer [50];
uint8_t errorLetter = CMD_READ_ERR_ID;
uint8_t usartLockFlag;
_Bool setRtcComing = 0;


extern inline void usart1TxCallback(void)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE; 																							/* Variable goes to true when contxt-switch is needed */
	uint8_t queueUartTransmitFlag; 																															/* Receive from queue and store into this variable */
	uint8_t usart1LockFlag = UART1_CLEAR_FLAG;																									/* Set item to send to queue */
	
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
			if(Usart1LockQueueHandle != NULL) { 																										/* Check on system start if semaphore is already created */
				xQueueSendFromISR(Usart1LockQueueHandle, &usart1LockFlag, &xHigherPriorityTaskWoken);	/* Send wake up signal to task when DMA is clear */
				portYIELD_FROM_ISR(xHigherPriorityTaskWoken); 																				/* Do context-switch if needed */
			}
			break;
		
		/* If the flag is unrecognised, raise eventually an error and return to normal mode */
		default:
			/* Eventual error throwing code to be inserted here */
			if(Usart1LockQueueHandle != NULL) { 																										/* Check on system start if semaphore is already created */
				xQueueSendFromISR(Usart1LockQueueHandle, &usart1LockFlag, &xHigherPriorityTaskWoken);	/* Send wake up signal to task when DMA is clear */
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



extern inline void telemetryReceive(void)
{
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
					case RESET_TELEM_ID:																													/* Reset telemetry */
						/* Put here the code necessary for resetting telemetry */
						xQueueReceive(Usart1LockQueueHandle, &usartLockFlag, portMAX_DELAY);				/* Lock if DMA is in use */
						commandAckMsg[COMMAND_ACK_IDENTIFIER_POS] = RESET_TELEM_ID; 								/* Set the correct identifier */
						HAL_UART_Transmit_DMA(&huart1, commandAckMsg, COMMAND_ACK_MSG_LEN); 				/* Transmit ack message */
						break;
		
						case START_ACQ_ID:
							/* Put here the code necessary for starting acquisition */						
							xQueueReceive(Usart1LockQueueHandle, &usartLockFlag, portMAX_DELAY);			/* Lock if DMA is in use */
							commandAckMsg[COMMAND_ACK_IDENTIFIER_POS] = START_ACQ_ID; 								/* Set the correct identifier */
							HAL_UART_Transmit_DMA(&huart1, commandAckMsg, COMMAND_ACK_MSG_LEN); 			/* Transmit ack message */
							break;	

						case STOP_ACQ_ID:
							/* Put here the code necessary for stopping acquisition */					
							xQueueReceive(Usart1LockQueueHandle, &usartLockFlag, portMAX_DELAY);			/* Lock if DMA is in use */
							commandAckMsg[COMMAND_ACK_IDENTIFIER_POS] = STOP_ACQ_ID; 									/* Set the correct identifier */
							HAL_UART_Transmit_DMA(&huart1, commandAckMsg, COMMAND_ACK_MSG_LEN); 			/* Transmit ack message */
							break;

						case GIVE_RTC_TIME_ID:
							/* Put here the code necessary for giving RTC time */
							xQueueReceive(Usart1LockQueueHandle, &usartLockFlag, portMAX_DELAY);			/* Lock if DMA is in use */
							commandAckMsg[COMMAND_ACK_IDENTIFIER_POS] = GIVE_RTC_TIME_ID; 						/* Set the correct identifier */
							HAL_UART_Transmit_DMA(&huart1, commandAckMsg, COMMAND_ACK_MSG_LEN); 			/* Transmit ack message */
							break;

						case GIVE_RTC_DATE_ID:
							/* Here put the code necessary for giving RTC data */
							xQueueReceive(Usart1LockQueueHandle, &usartLockFlag, portMAX_DELAY);			/* Lock if DMA is in use */
							commandAckMsg[COMMAND_ACK_IDENTIFIER_POS] = GIVE_RTC_DATE_ID; 						/* Set the correct identifier */
							HAL_UART_Transmit_DMA(&huart1, commandAckMsg, COMMAND_ACK_MSG_LEN); 			/* Transmit ack message */
							break;

						default:																																		/* Message not recognised */
							xQueueSend(ErrorQueueHandle, (void *)&errorLetter, (TickType_t)0); 				/* Add error to queue */
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
