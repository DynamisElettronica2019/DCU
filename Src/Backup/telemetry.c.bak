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
					case SET_TELEM_ID:																													
						DATA_SetTelemetryState();																										/* Start telemetry */
						xQueueReceive(Usart1LockQueueHandle, &usartLockFlag, portMAX_DELAY);				/* Lock if DMA is in use */
						commandAckMsg[COMMAND_ACK_IDENTIFIER_POS] = SET_TELEM_ID; 									/* Set the correct identifier */
						HAL_UART_Transmit_DMA(&huart1, commandAckMsg, COMMAND_ACK_MSG_LEN); 				/* Transmit ack message */
						break;
					
					case RESET_TELEM_ID:																													
						DATA_ResetTelemetryState();																									/* Stop telemetry */
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

/*
* Function for setting lower nibble (lower part of byte) for encoding
*/
uint8_t setLowerNibble(uint8_t editByte, uint8_t nibble) {
	uint8_t editedByte = editByte;
	editedByte &= 0xF0; // Clear out the lower nibble
	editedByte |= (nibble & 0x0F); // OR in the upper mask
	return editedByte;
}

/*
* Function for setting upper nibble (upper part of byte) for encoding
*/
uint8_t setUpperNibble(uint8_t editByte, uint8_t nibble) {
	uint8_t editedByte = editByte;
	editedByte &= 0x0F; // Clear out the upper nibble
	editedByte |= ((nibble << 4) & 0xF0); // OR in the lower mask
	return editedByte;
}

/*
* Function for encoding strings, parameters are in string reference, out string reference and in string lenght
* Gives back the out string lenght
*/
uint16_t encodeString(uint8_t* string, uint8_t* outString, uint16_t len) {
	uint16_t outStrIndex = 0; // Declaring variable for index
	
	outString[outStrIndex] = setUpperNibble(outString[outStrIndex], ENCODE_OPEN_SQUARE); //Setting open bracket on start
	outString[outStrIndex] = setLowerNibble(outString[outStrIndex], ENCODE_BLANK); //Setting open bracket on start
	outStrIndex++;
	
	switch(string[0]) { // Set upper nibble with corresponding 4 bits from message recogniser
			case '0':
				outString[outStrIndex] = setUpperNibble(outString[outStrIndex], ENCODE_0);
				break;
			case '1':
				outString[outStrIndex] = setUpperNibble(outString[outStrIndex], ENCODE_1);
				break;
			case '2':
				outString[outStrIndex] = setUpperNibble(outString[outStrIndex], ENCODE_2);
				break;
			case '3':
				outString[outStrIndex] = setUpperNibble(outString[outStrIndex], ENCODE_3);
				break;
			case '4':
				outString[outStrIndex] = setUpperNibble(outString[outStrIndex], ENCODE_4);
				break;
			case '5':
				outString[outStrIndex] = setUpperNibble(outString[outStrIndex], ENCODE_5);
				break;
			case '6':
				outString[outStrIndex] = setUpperNibble(outString[outStrIndex], ENCODE_6);
				break;
			case '7':
				outString[outStrIndex] = setUpperNibble(outString[outStrIndex], ENCODE_7);
				break;
			case '8':
				outString[outStrIndex] = setUpperNibble(outString[outStrIndex], ENCODE_8);
				break;
			case '9':
				outString[outStrIndex] = setUpperNibble(outString[outStrIndex], ENCODE_9);
				break;
			case ';':
				outString[outStrIndex] = setUpperNibble(outString[outStrIndex], ENCODE_SEMICOLON);
				break;
			case '.':
				outString[outStrIndex] = setUpperNibble(outString[outStrIndex], ENCODE_COLON);
				break;
			default:
				outString[outStrIndex] = setUpperNibble(outString[outStrIndex], ENCODE_BLANK);
				break;
	}
	outString[outStrIndex] = setLowerNibble(outString[outStrIndex], ENCODE_BLANK); //Set lower part of the bit on blank
	outStrIndex++;
	
	for (int i = 1; i < len-1; i += 2) { // Take characters 2 by 2
		switch(string[i]) { // Set upper nibble with corresponding 4 bits
			case '0':
				outString[outStrIndex] = setUpperNibble(outString[outStrIndex], ENCODE_0);
				break;
			case '1':
				outString[outStrIndex] = setUpperNibble(outString[outStrIndex], ENCODE_1);
				break;
			case '2':
				outString[outStrIndex] = setUpperNibble(outString[outStrIndex], ENCODE_2);
				break;
			case '3':
				outString[outStrIndex] = setUpperNibble(outString[outStrIndex], ENCODE_3);
				break;
			case '4':
				outString[outStrIndex] = setUpperNibble(outString[outStrIndex], ENCODE_4);
				break;
			case '5':
				outString[outStrIndex] = setUpperNibble(outString[outStrIndex], ENCODE_5);
				break;
			case '6':
				outString[outStrIndex] = setUpperNibble(outString[outStrIndex], ENCODE_6);
				break;
			case '7':
				outString[outStrIndex] = setUpperNibble(outString[outStrIndex], ENCODE_7);
				break;
			case '8':
				outString[outStrIndex] = setUpperNibble(outString[outStrIndex], ENCODE_8);
				break;
			case '9':
				outString[outStrIndex] = setUpperNibble(outString[outStrIndex], ENCODE_9);
				break;
			case ';':
				outString[outStrIndex] = setUpperNibble(outString[outStrIndex], ENCODE_SEMICOLON);
				break;
			case '.':
				outString[outStrIndex] = setUpperNibble(outString[outStrIndex], ENCODE_COLON);
				break;
			default:
				outString[outStrIndex] = setUpperNibble(outString[outStrIndex], ENCODE_BLANK);
				break;
		}
		
		switch(string[i+1]) { // Set lower nibble with corresponding 4 bits
			case '0':
				outString[outStrIndex] = setLowerNibble(outString[outStrIndex], ENCODE_0);
				break;
			case '1':
				outString[outStrIndex] = setLowerNibble(outString[outStrIndex], ENCODE_1);
				break;
			case '2':
				outString[outStrIndex] = setLowerNibble(outString[outStrIndex], ENCODE_2);
				break;
			case '3':
				outString[outStrIndex] = setLowerNibble(outString[outStrIndex], ENCODE_3);
				break;
			case '4':
				outString[outStrIndex] = setLowerNibble(outString[outStrIndex], ENCODE_4);
				break;
			case '5':
				outString[outStrIndex] = setLowerNibble(outString[outStrIndex], ENCODE_5);
				break;
			case '6':
				outString[outStrIndex] = setLowerNibble(outString[outStrIndex], ENCODE_6);
				break;
			case '7':
				outString[outStrIndex] = setLowerNibble(outString[outStrIndex], ENCODE_7);
				break;
			case '8':
				outString[outStrIndex] = setLowerNibble(outString[outStrIndex], ENCODE_8);
				break;
			case '9':
				outString[outStrIndex] = setLowerNibble(outString[outStrIndex], ENCODE_9);
				break;
			case ';':
				outString[outStrIndex] = setLowerNibble(outString[outStrIndex], ENCODE_SEMICOLON);
				break;
			case '.':
				outString[outStrIndex] = setLowerNibble(outString[outStrIndex], ENCODE_COLON);
				break;
			default:
				outString[outStrIndex] = setLowerNibble(outString[outStrIndex], ENCODE_BLANK);
				break;
		}
		
		outStrIndex++; // Increment outString index
	}
	
	if (len%2 == 0) { // If the lenght of the string is even (you are missing the last character)
			switch(string[len-1]) { // Set upper nibble with corresponding 4 bits
			case '0':
				outString[outStrIndex] = setUpperNibble(outString[outStrIndex], ENCODE_0);
				break;
			case '1':
				outString[outStrIndex] = setUpperNibble(outString[outStrIndex], ENCODE_1);
				break;
			case '2':
				outString[outStrIndex] = setUpperNibble(outString[outStrIndex], ENCODE_2);
				break;
			case '3':
				outString[outStrIndex] = setUpperNibble(outString[outStrIndex], ENCODE_3);
				break;
			case '4':
				outString[outStrIndex] = setUpperNibble(outString[outStrIndex], ENCODE_4);
				break;
			case '5':
				outString[outStrIndex] = setUpperNibble(outString[outStrIndex], ENCODE_5);
				break;
			case '6':
				outString[outStrIndex] = setUpperNibble(outString[outStrIndex], ENCODE_6);
				break;
			case '7':
				outString[outStrIndex] = setUpperNibble(outString[outStrIndex], ENCODE_7);
				break;
			case '8':
				outString[outStrIndex] = setUpperNibble(outString[outStrIndex], ENCODE_8);
				break;
			case '9':
				outString[outStrIndex] = setUpperNibble(outString[outStrIndex], ENCODE_9);
				break;
			case ';':
				outString[outStrIndex] = setUpperNibble(outString[outStrIndex], ENCODE_SEMICOLON);
				break;
			case ',':
				outString[outStrIndex] = setUpperNibble(outString[outStrIndex], ENCODE_COLON);
				break;
			default:
				outString[outStrIndex] = setUpperNibble(outString[outStrIndex], ENCODE_BLANK);
				break;
		}
			
		outString[outStrIndex] = setLowerNibble(outString[outStrIndex], ENCODE_BLANK); // Set blank on second side
		outStrIndex++; // Increment outString index
	}
	
	outString[outStrIndex] = setUpperNibble(outString[outStrIndex], ENCODE_CLOSE_SQUARE); // Set terminator on the end
	outString[outStrIndex] = setLowerNibble(outString[outStrIndex], ENCODE_BLANK); //Setting open bracket on start
	
	return (outStrIndex+1); // Return the lenght of the string
}
