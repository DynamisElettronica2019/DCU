#include "cmsis_os.h"
#include "telemetry.h"
#include "data.h"

extern osSemaphoreId sendDataSemaphoreHandle;
extern osSemaphoreId sendStateSemaphoreHandle;
extern osSemaphoreId sendErrorSemaphoreHandle;
extern osSemaphoreId receiveCommandSemaphoreHandle;
extern osSemaphoreId sendFollowingDataSemaphoreHandle;
extern osMessageQId Usart1TxModeQueueHandle;
extern osMessageQId Usart1LockQueueHandle;

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
			if(Usart1LockQueueHandle != NULL) { 																														/* Check on system start if semaphore is already created */
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
