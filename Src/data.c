#include <stdio.h>
#include <stdint.h>
#include "fatfs.h"
#include "data.h"

uint8_t blockBuffer[BUFFER_BLOCK_LEN];
uint8_t dcuStateBuffer[BUFFER_STATE_LEN] = "[S;0;0;0;0;0;0;0;0]";
static uint8_t acquisitionState = ACQUISITION_OFF_STATE;


extern inline void startAcquisitionStateMachine(uint8_t startAcquisitionEvent)
{
	/* NOTE: Events are coded using chars NOT numbers! */
	switch(acquisitionState) {
		case ACQUISITION_OFF_STATE:
			USB_CloseFile(); 			/* Check if acquisition is on, close USB file and set the state variable */
			
			/* State LED coding, only for debug purposes */
			HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET); 					/* Green LED on */
			HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_RESET); 			/* Yellow LED off */
			HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET); 						/* Red LED off */

			/* Get next state, based on the request received */
			switch(startAcquisitionEvent) {
				case ACQUISITION_ON_SW_REQUEST:
					acquisitionState = ACQUISITION_ON_FROM_SW_STATE;
					break;
						
				case ACQUISITION_ON_TELEMETRY_REQUEST:
					acquisitionState = ACQUISITION_ON_FROM_TELEMETRY_STATE;
					break;
				
				case ACQUISITION_ON_AUTO_REQUEST:
					acquisitionState = ACQUISITION_ON_FROM_AUTO_STATE;
					break;
				
				case ACQUISITION_ON_DEBUG_REQUEST:
					acquisitionState = ACQUISITION_ON_FROM_DEBUG_STATE;
					break;
				
				default:
					acquisitionState = ACQUISITION_OFF_STATE;
					break;
			}
					
			break;
		
		case ACQUISITION_ON_FROM_SW_STATE:
			USB_OpenFile(); 			/* Check if acquisition is off, open USB file and set the state variable */
		
			/* State LED coding, only for debug purposes */
			HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET); 					/* Green LED on */
			HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_SET); 				/* Yellow LED on */
			HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET); 						/* Red LED off */

			
			/* Get next state, based on the request received */
			if((startAcquisitionEvent == ACQUISITION_OFF_SW_REQUEST) || 
				 (startAcquisitionEvent == ACQUISITION_OFF_TELEMETRY_REQUEST) ||
				 (startAcquisitionEvent == ACQUISITION_OFF_DEBUG_REQUEST)) {
				acquisitionState = ACQUISITION_OFF_STATE;
			} else {
				acquisitionState = ACQUISITION_ON_FROM_SW_STATE;
			}
		
			break;
		
		case ACQUISITION_ON_FROM_TELEMETRY_STATE:
			USB_OpenFile(); 			/* Check if acquisition is off, open USB file and set the state variable */
		
			/* State LED coding, only for debug purposes */
			HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET); 				/* Green LED off */
			HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_SET); 				/* Yellow LED on */
			HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET); 						/* Red LED off */
			
			/* Get next state, based on the request received */
					if((startAcquisitionEvent == ACQUISITION_OFF_TELEMETRY_REQUEST) || 
						 (startAcquisitionEvent == ACQUISITION_OFF_SW_REQUEST) ||
				     (startAcquisitionEvent == ACQUISITION_OFF_DEBUG_REQUEST)) {
				acquisitionState = ACQUISITION_OFF_STATE;
			} else {
				acquisitionState = ACQUISITION_ON_FROM_TELEMETRY_STATE;
			}
			
			break;
		
		case ACQUISITION_ON_FROM_AUTO_STATE:
			USB_OpenFile(); 			/* Check if acquisition is off, open USB file and set the state variable */
		
			/* State LED coding, only for debug purposes */
			HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET); 				/* Green LED off */
			HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_RESET); 			/* Yellow LED off */
			HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET); 							/* Red LED on */
			
			/* Get next state, based on the request received */
			if((startAcquisitionEvent == ACQUISITION_OFF_SW_REQUEST) || 
			   (startAcquisitionEvent == ACQUISITION_OFF_TELEMETRY_REQUEST) ||
				 (startAcquisitionEvent == ACQUISITION_OFF_AUTO_REQUEST) ||
				 (startAcquisitionEvent == ACQUISITION_OFF_DEBUG_REQUEST)) {
				acquisitionState = ACQUISITION_OFF_STATE;
			} else {
				acquisitionState = ACQUISITION_ON_FROM_AUTO_STATE;
			}
			
			break;
		
		case ACQUISITION_ON_FROM_DEBUG_STATE:
			USB_OpenFile(); 			/* Check if acquisition is off, open USB file and set the state variable */
		
			/* State LED coding, only for debug purposes */
			HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET); 				/* Green LED off */
			HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_SET); 				/* Yellow LED on */
			HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET); 							/* Red LED on */
			
			/* Get next state, based on the request received */
			if(startAcquisitionEvent == ACQUISITION_OFF_DEBUG_REQUEST) {
				acquisitionState = ACQUISITION_OFF_STATE;
			} else {
				acquisitionState = ACQUISITION_ON_FROM_DEBUG_STATE;
			}
			
			break;
		
		default:
			/* Should be never go there, but anyway reset the state machine */
			/* Put here error managing code */
		
			/* State LED coding, only for debug purposes */
			HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET); 					/* Green LED off */
			HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_SET); 				/* Yellow LED on */
			HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET); 							/* Red LED on */
			
			acquisitionState = ACQUISITION_OFF_STATE;
			break;
	}

	return;
}

extern inline uint8_t getUsbReadyState(void)
{
	return dcuStateBuffer[STATE_USB_READY_INDEX];
}

extern inline uint8_t getAcquisitionState(void)
{
	return dcuStateBuffer[STATE_ACQUISITION_ON_INDEX];
}

extern inline void setUsbPresentState(void)
{
	dcuStateBuffer[STATE_USB_PRESENT_INDEX] = STATE_ON;
	return;
}

extern inline void setUsbReadyState(void)
{
	dcuStateBuffer[STATE_USB_READY_INDEX] = STATE_ON;
	return;
}

extern inline void setAcquisitionState(void)
{
	dcuStateBuffer[STATE_ACQUISITION_ON_INDEX] = STATE_ON;
	return;
}

extern inline void resetUsbPresentState(void)
{
	dcuStateBuffer[STATE_USB_PRESENT_INDEX] = STATE_OFF;
	return;
}

extern inline void resetUsbReadyState(void)
{
	dcuStateBuffer[STATE_USB_READY_INDEX] = STATE_OFF;
	return;
}

extern inline void resetAcquisitionState(void)
{
	dcuStateBuffer[STATE_ACQUISITION_ON_INDEX] = STATE_OFF;
	return;
}

extern void dataBufferInit(void)
{
	for(uint16_t i = 0; i < (BUFFER_BLOCK_LEN - 1); i++) {
		blockBuffer[i] = '5';
	}
	
	blockBuffer[BUFFER_BLOCK_LEN - 1] = '\n';
	return;
}

static inline void USB_OpenFile(void)
{
	FRESULT openResult;
	
	if((getAcquisitionState() == STATE_OFF) && (getUsbReadyState() == STATE_ON)){
		/* Put here getFilename function */
		openResult = f_open(&USBHFile, "DynamisPRC_USB_test.txt", FA_CREATE_ALWAYS | FA_WRITE);
	
		if(openResult == FR_OK) {
			//HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
			setAcquisitionState();				/* Update of the status packet */
			/* Put here the code to manage errors */
		}
	}
	
	return;
}

static inline void USB_CloseFile(void)
{
	FRESULT closeResult;
	
	if((getAcquisitionState() == STATE_ON) && (getUsbReadyState() == STATE_ON)) {
		closeResult = f_close(&USBHFile);
		
		if(closeResult == FR_OK) {
			//HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
			resetAcquisitionState();			/* Update of the status packet */
			/* Put here the code to manage errors */
		}
	}
	
	return;
}
