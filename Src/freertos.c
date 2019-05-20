/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "can.h"
#include "usb_host.h"
#include "fatfs.h"
#include "usart.h"
#include "gpio.h"
#include "adc.h"
#include "data.h"
#include "data_conversion.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
UINT bytesWritten;
BaseType_t startAcquisition_xHigherPriorityTaskWoken = pdFALSE;
extern uint8_t acquisitionOn;
extern uint8_t blockBuffer[BUFFER_BLOCK_LEN];
extern uint8_t dcuStateBuffer[BUFFER_STATE_LEN];
extern uint8_t telemetryReceivedBuffer [BUFFER_COMMAND_LEN];
extern uint8_t setRtcReceivedBuffer [BUFFER_RTC_SET_LEN];
extern uint32_t adc1BufferRaw [ADC1_RAW_DATA_LEN];					
extern uint32_t adc2BufferRaw [ADC2_RAW_DATA_LEN];
extern float adcBufferConvertedDebug [ADC_CONVERTED_DEBUG_DATA_LEN];
extern float adcBufferConvertedAux [ADC_CONVERTED_AUX_DATA_LEN];
extern USBH_HandleTypeDef hUsbHostFS;

uint8_t state;
/* USER CODE END Variables */
osThreadId aliveHandle;
osThreadId adc1ConversionHandle;
osThreadId adc2ConversionHandle;
osThreadId digitalAuxManagerHandle;
osThreadId autogearManagerHandle;
osThreadId USB_OvercurrentManagerHandle;
osThreadId SendStatesHandle;
osThreadId SendDataHandle;
osThreadId ReceiveTelemHandle;
osThreadId SendErrorHandle;
osThreadId SendFollowingDataHandle;
osThreadId saveUsbHandle;
osThreadId usbManagerHandle;
osThreadId startAcquisitionStateMachineHandle;
osThreadId canFifo0UnpackHandle;
osThreadId canFifo1UnpackHandle;
osMessageQId digitalAuxQueueHandle;
osMessageQId ErrorQueueHandle;
osMessageQId Usart1TxModeQueueHandle;
osMessageQId Usart1LockQueueHandle;
osMessageQId usbEventQueueHandle;
osMessageQId startAcquisitionEventHandle;
osMessageQId canFifo0QueueHandle;
osMessageQId canFifo1QueueHandle;
osSemaphoreId adc1SemaphoreHandle;
osSemaphoreId adc2SemaphoreHandle;
osSemaphoreId autogearSemaphoreHandle;
osSemaphoreId USB_OvercurrentSemaphoreHandle;
osSemaphoreId sendDataSemaphoreHandle;
osSemaphoreId sendStateSemaphoreHandle;
osSemaphoreId receiveCommandSemaphoreHandle;
osSemaphoreId sendFollowingDataSemaphoreHandle;
osSemaphoreId saveUsbSemaphoreHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void aliveTask(void const * argument);
void adc1ConversionTask(void const * argument);
void adc2ConversionTask(void const * argument);
void digitalAuxManagerTask(void const * argument);
void autogearManagerTask(void const * argument);
void USB_OvercurrentManagerTask(void const * argument);
void SendStatesFunc(void const * argument);
void SendDataFunc(void const * argument);
void ReceiveTelemFunc(void const * argument);
void SendErrorFunc(void const * argument);
void SendFollowingDataFunc(void const * argument);
void saveUsbTask(void const * argument);
void usbManagerTask(void const * argument);
void startAcquisitionStateMachineTask(void const * argument);
void canFifo0UnpackTask(void const * argument);
void canFifo1UnpackTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* definition and creation of adc1Semaphore */
  osSemaphoreDef(adc1Semaphore);
  adc1SemaphoreHandle = osSemaphoreCreate(osSemaphore(adc1Semaphore), 1);

  /* definition and creation of adc2Semaphore */
  osSemaphoreDef(adc2Semaphore);
  adc2SemaphoreHandle = osSemaphoreCreate(osSemaphore(adc2Semaphore), 1);

  /* definition and creation of autogearSemaphore */
  osSemaphoreDef(autogearSemaphore);
  autogearSemaphoreHandle = osSemaphoreCreate(osSemaphore(autogearSemaphore), 1);

  /* definition and creation of USB_OvercurrentSemaphore */
  osSemaphoreDef(USB_OvercurrentSemaphore);
  USB_OvercurrentSemaphoreHandle = osSemaphoreCreate(osSemaphore(USB_OvercurrentSemaphore), 1);

  /* definition and creation of sendDataSemaphore */
  osSemaphoreDef(sendDataSemaphore);
  sendDataSemaphoreHandle = osSemaphoreCreate(osSemaphore(sendDataSemaphore), 1);

  /* definition and creation of sendStateSemaphore */
  osSemaphoreDef(sendStateSemaphore);
  sendStateSemaphoreHandle = osSemaphoreCreate(osSemaphore(sendStateSemaphore), 1);

  /* definition and creation of receiveCommandSemaphore */
  osSemaphoreDef(receiveCommandSemaphore);
  receiveCommandSemaphoreHandle = osSemaphoreCreate(osSemaphore(receiveCommandSemaphore), 1);

  /* definition and creation of sendFollowingDataSemaphore */
  osSemaphoreDef(sendFollowingDataSemaphore);
  sendFollowingDataSemaphoreHandle = osSemaphoreCreate(osSemaphore(sendFollowingDataSemaphore), 1);

  /* definition and creation of saveUsbSemaphore */
  osSemaphoreDef(saveUsbSemaphore);
  saveUsbSemaphoreHandle = osSemaphoreCreate(osSemaphore(saveUsbSemaphore), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
	xSemaphoreTake(saveUsbSemaphoreHandle, portMAX_DELAY);								/* Start with the task locked */
	xSemaphoreTake(sendStateSemaphoreHandle, portMAX_DELAY); 							/* Start with the task locked */
	xSemaphoreTake(sendDataSemaphoreHandle, portMAX_DELAY); 							/* Start with the task locked */
	xSemaphoreTake(receiveCommandSemaphoreHandle, portMAX_DELAY); 				/* Start with the task locked */
	xSemaphoreTake(sendFollowingDataSemaphoreHandle, portMAX_DELAY); 			/* Start with the task locked */
	xSemaphoreTake(adc1SemaphoreHandle, portMAX_DELAY);										/* Start with the task locked */
	xSemaphoreTake(adc2SemaphoreHandle, portMAX_DELAY); 									/* Start with the task locked */
	xSemaphoreTake(autogearSemaphoreHandle, portMAX_DELAY); 							/* Start with the task locked */
	xSemaphoreTake(USB_OvercurrentSemaphoreHandle, portMAX_DELAY); 				/* Start with the task locked */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of digitalAuxQueue */
  osMessageQDef(digitalAuxQueue, 8, uint8_t);
  digitalAuxQueueHandle = osMessageCreate(osMessageQ(digitalAuxQueue), NULL);

  /* definition and creation of ErrorQueue */
  osMessageQDef(ErrorQueue, 8, uint8_t);
  ErrorQueueHandle = osMessageCreate(osMessageQ(ErrorQueue), NULL);

  /* definition and creation of Usart1TxModeQueue */
  osMessageQDef(Usart1TxModeQueue, 1, uint8_t);
  Usart1TxModeQueueHandle = osMessageCreate(osMessageQ(Usart1TxModeQueue), NULL);

  /* definition and creation of Usart1LockQueue */
  osMessageQDef(Usart1LockQueue, 1, uint8_t);
  Usart1LockQueueHandle = osMessageCreate(osMessageQ(Usart1LockQueue), NULL);

  /* definition and creation of usbEventQueue */
  osMessageQDef(usbEventQueue, 8, uint8_t);
  usbEventQueueHandle = osMessageCreate(osMessageQ(usbEventQueue), NULL);

  /* definition and creation of startAcquisitionEvent */
  osMessageQDef(startAcquisitionEvent, 8, uint8_t);
  startAcquisitionEventHandle = osMessageCreate(osMessageQ(startAcquisitionEvent), NULL);

  /* definition and creation of canFifo0Queue */
  osMessageQDef(canFifo0Queue, 32, CAN_RxPacketTypeDef);
  canFifo0QueueHandle = osMessageCreate(osMessageQ(canFifo0Queue), NULL);

  /* definition and creation of canFifo1Queue */
  osMessageQDef(canFifo1Queue, 32, CAN_RxPacketTypeDef);
  canFifo1QueueHandle = osMessageCreate(osMessageQ(canFifo1Queue), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
	uint8_t usart1LockFlag = UART1_CLEAR_FLAG;
	xQueueSend(Usart1LockQueueHandle, &usart1LockFlag, 0);
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of alive */
  osThreadDef(alive, aliveTask, osPriorityLow, 0, 128);
  aliveHandle = osThreadCreate(osThread(alive), NULL);

  /* definition and creation of adc1Conversion */
  osThreadDef(adc1Conversion, adc1ConversionTask, osPriorityLow, 0, 128);
  adc1ConversionHandle = osThreadCreate(osThread(adc1Conversion), NULL);

  /* definition and creation of adc2Conversion */
  osThreadDef(adc2Conversion, adc2ConversionTask, osPriorityLow, 0, 128);
  adc2ConversionHandle = osThreadCreate(osThread(adc2Conversion), NULL);

  /* definition and creation of digitalAuxManager */
  osThreadDef(digitalAuxManager, digitalAuxManagerTask, osPriorityLow, 0, 128);
  digitalAuxManagerHandle = osThreadCreate(osThread(digitalAuxManager), NULL);

  /* definition and creation of autogearManager */
  osThreadDef(autogearManager, autogearManagerTask, osPriorityBelowNormal, 0, 128);
  autogearManagerHandle = osThreadCreate(osThread(autogearManager), NULL);

  /* definition and creation of USB_OvercurrentManager */
  osThreadDef(USB_OvercurrentManager, USB_OvercurrentManagerTask, osPriorityBelowNormal, 0, 128);
  USB_OvercurrentManagerHandle = osThreadCreate(osThread(USB_OvercurrentManager), NULL);

  /* definition and creation of SendStates */
  osThreadDef(SendStates, SendStatesFunc, osPriorityNormal, 0, 128);
  SendStatesHandle = osThreadCreate(osThread(SendStates), NULL);

  /* definition and creation of SendData */
  osThreadDef(SendData, SendDataFunc, osPriorityNormal, 0, 128);
  SendDataHandle = osThreadCreate(osThread(SendData), NULL);

  /* definition and creation of ReceiveTelem */
  osThreadDef(ReceiveTelem, ReceiveTelemFunc, osPriorityAboveNormal, 0, 128);
  ReceiveTelemHandle = osThreadCreate(osThread(ReceiveTelem), NULL);

  /* definition and creation of SendError */
  osThreadDef(SendError, SendErrorFunc, osPriorityAboveNormal, 0, 128);
  SendErrorHandle = osThreadCreate(osThread(SendError), NULL);

  /* definition and creation of SendFollowingData */
  osThreadDef(SendFollowingData, SendFollowingDataFunc, osPriorityNormal, 0, 128);
  SendFollowingDataHandle = osThreadCreate(osThread(SendFollowingData), NULL);

  /* definition and creation of saveUsb */
  osThreadDef(saveUsb, saveUsbTask, osPriorityAboveNormal, 0, 512);
  saveUsbHandle = osThreadCreate(osThread(saveUsb), NULL);

  /* definition and creation of usbManager */
  osThreadDef(usbManager, usbManagerTask, osPriorityBelowNormal, 0, 512);
  usbManagerHandle = osThreadCreate(osThread(usbManager), NULL);

  /* definition and creation of startAcquisitionStateMachine */
  osThreadDef(startAcquisitionStateMachine, startAcquisitionStateMachineTask, osPriorityBelowNormal, 0, 128);
  startAcquisitionStateMachineHandle = osThreadCreate(osThread(startAcquisitionStateMachine), NULL);

  /* definition and creation of canFifo0Unpack */
  osThreadDef(canFifo0Unpack, canFifo0UnpackTask, osPriorityHigh, 0, 512);
  canFifo0UnpackHandle = osThreadCreate(osThread(canFifo0Unpack), NULL);

  /* definition and creation of canFifo1Unpack */
  osThreadDef(canFifo1Unpack, canFifo1UnpackTask, osPriorityHigh, 0, 512);
  canFifo1UnpackHandle = osThreadCreate(osThread(canFifo1Unpack), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_aliveTask */
/**
  * @brief  Function implementing the alive thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_aliveTask */
void aliveTask(void const * argument)
{

  /* USER CODE BEGIN aliveTask */
  /* Infinite loop */
  for(;;) {
    HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
		osDelay(250);
  }
  /* USER CODE END aliveTask */
}

/* USER CODE BEGIN Header_adc1ConversionTask */
/**
* @brief Function implementing the adc1Conversion thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_adc1ConversionTask */
void adc1ConversionTask(void const * argument)
{
  /* USER CODE BEGIN adc1ConversionTask */
  /* Infinite loop */
  for(;;) {
		xSemaphoreTake(adc1SemaphoreHandle, portMAX_DELAY); 		/* Unlock when DMA callback is called */
		HAL_ADC_Stop_DMA(&hadc1);																/* Stop ADC1 conversion and wait for timer to restrt it */
		adcBufferConvertedDebug[DCU_TEMP_SENSE_POSITION] = dcuTempSenseConversion(adc1BufferRaw[DCU_TEMP_SENSE_POSITION]);
		adcBufferConvertedDebug[MAIN_CURRENT_SENSE_POSITION] = mainCurrentSenseConversion(adc1BufferRaw[MAIN_CURRENT_SENSE_POSITION]);
		adcBufferConvertedDebug[DCU_CURRENT_SENSE_POSITION] = dcuCurrentSenseConversion(adc1BufferRaw[DCU_CURRENT_SENSE_POSITION]);
		adcBufferConvertedDebug[_5V_DCU_POSITION] = _5vSenseConversion(adc1BufferRaw[_5V_DCU_POSITION]);
		adcBufferConvertedDebug[_12V_POST_DIODES_SENSE_POSITION] = _12vSenseConversion(adc1BufferRaw[_12V_POST_DIODES_SENSE_POSITION]);
		adcBufferConvertedDebug[_3V3_MCU_POSITION] = _3v3SenseConversion(adc1BufferRaw[_3V3_MCU_POSITION]);
		adcBufferConvertedDebug[XBEE_CURRENT_SENSE_POSITION] = xbeeCurrentSenseConversion(adc1BufferRaw[XBEE_CURRENT_SENSE_POSITION]);
		adcBufferConvertedDebug[VBAT_CHANNEL_POSITION] = vbatConversion(adc1BufferRaw[VBAT_CHANNEL_POSITION]);
		/* TO BE IMPLEMENTED */
		/* Put here the CSV buffer conversion functions */
  }
  /* USER CODE END adc1ConversionTask */
}

/* USER CODE BEGIN Header_adc2ConversionTask */
/**
* @brief Function implementing the adc2Conversion thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_adc2ConversionTask */
void adc2ConversionTask(void const * argument)
{
  /* USER CODE BEGIN adc2ConversionTask */
  /* Infinite loop */
  for(;;) {
		xSemaphoreTake(adc2SemaphoreHandle, portMAX_DELAY); 		/* Unlock when DMA callback is called */
		HAL_ADC_Stop_DMA(&hadc2);																/* Stop ADC2 conversion and wait for timer to restrt it */
    adcBufferConvertedAux[ANALOG_AUX_1_POSITION] = analogAux1Conversion(adc2BufferRaw[ANALOG_AUX_1_RAW_POSITION]);
		adcBufferConvertedAux[ANALOG_AUX_2_POSITION] = analogAux2Conversion(adc2BufferRaw[ANALOG_AUX_2_RAW_POSITION]);
		adcBufferConvertedAux[ANALOG_AUX_3_POSITION] = analogAux3Conversion(adc2BufferRaw[ANALOG_AUX_3_RAW_POSITION]);
		/* TO BE IMPLEMENTED */
		/* Put here the CSV buffer conversion functions */
  }
  /* USER CODE END adc2ConversionTask */
}

/* USER CODE BEGIN Header_digitalAuxManagerTask */
/**
* @brief Function implementing the digitalAuxManager thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_digitalAuxManagerTask */
void digitalAuxManagerTask(void const * argument)
{
  /* USER CODE BEGIN digitalAuxManagerTask */
	uint8_t digitalAuxEvent = 0;
	
  /* Infinite loop */
  for(;;) {
		
		/* Wait for an event and get the identifier from the queue */
		if(xQueueReceive(digitalAuxQueueHandle, &digitalAuxEvent, portMAX_DELAY) == pdTRUE) {
			switch(digitalAuxEvent) {
				case DIGITAL_EVENT_AUX_1:
					/* Put here the code to handling digital aux 1 event */
					break;
				
				case DIGITAL_EVENT_AUX_2:
					/* Put here the code to handling digital aux 1 event */
					break;
				
				case DIGITAL_EVENT_AUX_3:
					/* Put here the code to handling digital aux 1 event */
					break;
			}
		}
  }
  /* USER CODE END digitalAuxManagerTask */
}

/* USER CODE BEGIN Header_autogearManagerTask */
/**
* @brief Function implementing the autogearManager thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_autogearManagerTask */
void autogearManagerTask(void const * argument)
{
  /* USER CODE BEGIN autogearManagerTask */
  /* Infinite loop */
  for(;;) {
    xSemaphoreTake(autogearSemaphoreHandle, portMAX_DELAY); 			/* Unlock when interrupt callback is called */
		osDelay(15);																									/* Wait for fail trigger detection */
		
		if(HAL_GPIO_ReadPin(AUTOGEAR_SWTICH_MCU_GPIO_Port, AUTOGEAR_SWTICH_MCU_Pin) == GPIO_PIN_RESET) {
			/* Put here the code to handling autogear event */
		}
  }
  /* USER CODE END autogearManagerTask */
}

/* USER CODE BEGIN Header_USB_OvercurrentManagerTask */
/**
* @brief Function implementing the USB_OvercurrentManager thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_USB_OvercurrentManagerTask */
void USB_OvercurrentManagerTask(void const * argument)
{
  /* USER CODE BEGIN USB_OvercurrentManagerTask */
  /* Infinite loop */
  for(;;) {
		xSemaphoreTake(autogearSemaphoreHandle, portMAX_DELAY); 		/* Unlock when interrupt callback is called */
		osDelay(1);																									/* Wait for fail trigger detection */
		
		if(HAL_GPIO_ReadPin(USB_OVERCURRENT_GPIO_Port, USB_OVERCURRENT_Pin) == GPIO_PIN_RESET) {
			/* Put here the code to handling autogear event */
		}
  }
  /* USER CODE END USB_OvercurrentManagerTask */
}

/* USER CODE BEGIN Header_SendStatesFunc */
/**
* @brief Function implementing the SendStates thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_SendStatesFunc */
void SendStatesFunc(void const * argument)
{
  /* USER CODE BEGIN SendStatesFunc */
	uint8_t normalTxModeFlag = NORMAL_MODE_TX_FLAG;  																			/* Setting flag identifier to put later into the queue */
  uint8_t usartLockFlag;
	/* Infinite loop */
  for(;;) {
    xSemaphoreTake(sendStateSemaphoreHandle, portMAX_DELAY); 														/* Unlock when timer callback is called */																			
		xQueueReceive(Usart1LockQueueHandle, &usartLockFlag, portMAX_DELAY);						    /* Lock if DMA is in use */
		xQueueSend(Usart1TxModeQueueHandle, (void *)&normalTxModeFlag, (TickType_t)0); 			/* Add flag to queue */
		HAL_UART_Transmit_DMA(&huart1, dcuStateBuffer, BUFFER_STATE_LEN); 									/* Transmit state message */
  }
  /* USER CODE END SendStatesFunc */
}

/* USER CODE BEGIN Header_SendDataFunc */
/**
* @brief Function implementing the SendData thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_SendDataFunc */
void SendDataFunc(void const * argument)
{
  /* USER CODE BEGIN SendDataFunc */
	uint8_t usartLockFlag;
	uint8_t secondTxModeFlag = SECOND_HALF_TX_FLAG; 																			/* Setting flag identifier to put later into the queue */
  
	/* Infinite loop */
  for(;;) {
    xSemaphoreTake(sendDataSemaphoreHandle, portMAX_DELAY); 														/* Unlock when timer callback is called */
		xQueueReceive(Usart1LockQueueHandle, &usartLockFlag, portMAX_DELAY);								/* Lock if DMA is in use */
		xQueueSend(Usart1TxModeQueueHandle, (void *)&secondTxModeFlag, (TickType_t)0); 			/* Add flag to queue */
		HAL_UART_Transmit_DMA(&huart1, blockBuffer, HALF_DATA_INDEX); 											/* Transmit first half of data message */
  }
  /* USER CODE END SendDataFunc */
}

/* USER CODE BEGIN Header_ReceiveTelemFunc */
/**
* @brief Function implementing the ReceiveTelem thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ReceiveTelemFunc */
void ReceiveTelemFunc(void const * argument)
{
  /* USER CODE BEGIN ReceiveTelemFunc */
	uint8_t errorLetter = CMD_READ_ERR_ID; 																									/* Define which char is the identifier of the error */
	uint8_t commandAckMsg [COMMAND_ACK_MSG_LEN] = ACK_MSG;
	uint8_t usartLockFlag;
	uint8_t tempBuffer[50];
	_Bool setRtcComing = 0;
	uint8_t temp;
	
	HAL_UART_Receive(&huart1, tempBuffer, 50, 50);
	HAL_UART_Receive_DMA(&huart1, telemetryReceivedBuffer, BUFFER_COMMAND_LEN);
  
	/* Infinite loop */
  for(;;) {
    xSemaphoreTake(receiveCommandSemaphoreHandle, portMAX_DELAY); 												/* Unlock when uart rx from telemetry is completed */
		
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
								temp = '6';
								xQueueSend(startAcquisitionEventHandle, (void *)&temp, (TickType_t)0); 			/* Add flag to queue */
							
								xQueueReceive(Usart1LockQueueHandle, &usartLockFlag, portMAX_DELAY);			/* Lock if DMA is in use */
								commandAckMsg[COMMAND_ACK_IDENTIFIER_POS] = START_ACQ_ID; 								/* Set the correct identifier */
								HAL_UART_Transmit_DMA(&huart1, commandAckMsg, COMMAND_ACK_MSG_LEN); 			/* Transmit ack message */
								break;	

							case STOP_ACQ_ID:
								/* Put here the code necessary for stopping acquisition */
								temp = 'A';
								xQueueSend(startAcquisitionEventHandle, &temp, 0);
							
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
			
			else {																																					/* If message does not start correctly */
				HAL_UART_Receive(&huart1, tempBuffer, 50, 50);
				HAL_UART_Receive_DMA(&huart1, telemetryReceivedBuffer, BUFFER_COMMAND_LEN); 			/* Re enable receiving */
				xQueueSend(ErrorQueueHandle, ( void * ) &errorLetter, ( TickType_t ) 0 ); 				/* Add error to queue */				
			}
		}
  }
  /* USER CODE END ReceiveTelemFunc */
}

/* USER CODE BEGIN Header_SendErrorFunc */
/**
* @brief Function implementing the SendError thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_SendErrorFunc */
void SendErrorFunc(void const * argument)
{
  /* USER CODE BEGIN SendErrorFunc */
	uint8_t queueLetter;
	uint8_t errorMsg[ERROR_MSG_LEN] = ERROR_MSG; 																						/* Set standard error message to be edited later */
	uint8_t normalTxModeFlag = NORMAL_MODE_TX_FLAG; 																				/* Setting flag identifier to put later into the queue */
  uint8_t usartLockFlag;
	
	/* Infinite loop */
  for(;;) {
    xQueueReceive(ErrorQueueHandle, &queueLetter, portMAX_DELAY); 												/* Wait for error, get the identifier char from the queue */
    errorMsg[ERROR_MSG_IDENTIFIER_POS] = queueLetter; 																		/* Set error identifier into the message */
		xQueueReceive(Usart1LockQueueHandle, &usartLockFlag, portMAX_DELAY);									/* Lock if DMA is in use */
		xQueueSend(Usart1TxModeQueueHandle, (void *) &normalTxModeFlag, (TickType_t)0); 			/* Add flag to queue */
		HAL_UART_Transmit_DMA(&huart1, errorMsg, ERROR_MSG_LEN); 															/* Transmit error message */
  }
  /* USER CODE END SendErrorFunc */
}

/* USER CODE BEGIN Header_SendFollowingDataFunc */
/**
* @brief Function implementing the SendFollowingData thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_SendFollowingDataFunc */
void SendFollowingDataFunc(void const * argument)
{
  /* USER CODE BEGIN SendFollowingDataFunc */
	uint8_t normalTxModeFlag = NORMAL_MODE_TX_FLAG; 																				/* Setting flag identifier to put later into the queue */
  
	/* Infinite loop */
  for(;;) {
    xSemaphoreTake(sendFollowingDataSemaphoreHandle, portMAX_DELAY); 											/* Unlock when first part tx is completed, unlocked from tx complete callback */
		xQueueSend(Usart1TxModeQueueHandle, (void *)&normalTxModeFlag, (TickType_t)0); 				/* Add flag to queue */
		HAL_UART_Transmit_DMA(&huart1, blockBuffer + HALF_DATA_INDEX, HALF_DATA_INDEX); 			/* Transmit second half of data message */
  }
  /* USER CODE END SendFollowingDataFunc */
}

/* USER CODE BEGIN Header_saveUsbTask */
/**
* @brief Function implementing the saveUsb thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_saveUsbTask */
void saveUsbTask(void const * argument)
{
  /* USER CODE BEGIN saveUsbTask */
  /* Infinite loop */
  for(;;) {
		xSemaphoreTake(saveUsbSemaphoreHandle, portMAX_DELAY);		/* Unlock when timer callback is called */

		/* Saving task code */
		if(getAcquisitionState() == STATE_ON) {
			f_write(&USBHFile, blockBuffer, BUFFER_BLOCK_LEN, (void *)&bytesWritten);
			/* Put here the code to manage errors */
		}
  }
  /* USER CODE END saveUsbTask */
}

/* USER CODE BEGIN Header_usbManagerTask */
/**
* @brief Function implementing the usbManager thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_usbManagerTask */
void usbManagerTask(void const * argument)
{
  /* USER CODE BEGIN usbManageTask */
	osEvent usbEvent;
	
  /* Infinite loop */
  for(;;) {
		usbEvent = osMessageGet(usbEventQueueHandle, osWaitForever);
		
		if(usbEvent.status == osEventMessage) {
			switch(usbEvent.value.v) {
				case DISCONNECTION_EVENT:
					f_mount(NULL, (TCHAR const *)"", 1);
					FATFS_UnLinkDriver(USBHPath);
					resetUsbReadyState();					/* Update of the status packet */
					resetUsbPresentState();				/* Update of the status packet */
					/* Put here the code to manage errors */
					break;

				case CONNECTED_EVENT:
					if(FATFS_LinkDriver(&USBH_Driver, USBHPath) == 0) {
						f_mount(&USBHFatFS, (TCHAR const *)USBHPath, 1);
						setUsbReadyState();					/* Update of the status packet */
						setUsbReadyState();					/* Update of the status packet */
					}
					
					/* Put here the code to manage errors */
					break;
	    
				default:
					break;
			}
		}
	}
  /* USER CODE END usbManageTask */
}

/* USER CODE BEGIN Header_startAcquisitionStateMachineTask */
/**
* @brief Function implementing the startAcquisitionStateMachine thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_startAcquisitionStateMachineTask */
void startAcquisitionStateMachineTask(void const * argument)
{
	/* USER CODE BEGIN startAcquisitionStateMachineTask */
	uint8_t startAcquisitionEvent = ACQUISITION_IDLE_REQUEST;

	/* Infinite loop */
	for(;;) {
		/* Start acquisition state machine, called at 100 Hz */
		/* NOTE: Events are coded using chars NOT numbers! */
		startAcquisitionStateMachine(startAcquisitionEvent);
	
		if(xQueueReceive(startAcquisitionEventHandle, &startAcquisitionEvent, 100 / portTICK_PERIOD_MS) != pdTRUE) {
			startAcquisitionEvent = ACQUISITION_IDLE_REQUEST;
		}
	}
	/* USER CODE END startAcquisitionStateMachineTask */
}

/* USER CODE BEGIN Header_canFifo0UnpackTask */
/**
* @brief Function implementing the canFifo0Unpack thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_canFifo0UnpackTask */
void canFifo0UnpackTask(void const * argument)
{
  /* USER CODE BEGIN canFifo0UnpackTask */
	CAN_RxPacketTypeDef unpackedData;
	
  /* Infinite loop */
  for(;;) {		
		if(xQueueReceive(canFifo0QueueHandle, &unpackedData, portMAX_DELAY) == pdTRUE) {		
			canDataParser(&unpackedData);
		}
  }
  /* USER CODE END canFifo0UnpackTask */
}

/* USER CODE BEGIN Header_canFifo1UnpackTask */
/**
* @brief Function implementing the canFifo1Unpack thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_canFifo1UnpackTask */
void canFifo1UnpackTask(void const * argument)
{
  /* USER CODE BEGIN canFifo1UnpackTask */
	CAN_RxPacketTypeDef unpackedData;
	
  /* Infinite loop */
  for(;;) {
		if(xQueueReceive(canFifo1QueueHandle, &unpackedData, portMAX_DELAY) == pdTRUE) {		
			canDataParser(&unpackedData);
		}
  }
  /* USER CODE END canFifo1UnpackTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
