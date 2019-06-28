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
#include "gpio.h"
#include "tim.h"
#include "adc.h"
#include "can.h"
#include "usart.h"
#include "usb_host.h"
#include "fatfs.h"
#include "rtc.h"
#include "GPS.h"
#include "data.h"
#include "telemetry.h"
#include "timestamp.h"
#include "flash_utility.h"

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
extern OffsetHandler_t OffsetHandler;
extern uint8_t DATA_BlockReadIndex;
extern uint8_t GPS_FirstChar;
extern uint8_t DATA_BlockBuffer [BUFFER_POINTERS_NUMBER][BUFFER_BLOCK_LEN];
extern uint8_t DATA_StateBuffer [BUFFER_STATE_LEN];
extern uint8_t telemetryReceivedBuffer [BUFFER_COMMAND_LEN];
extern uint8_t telemetryIndBuffer [1];
extern uint8_t GPS_RawBuffer [GPS_MAX_LENGTH];
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
osThreadId sendDebugDataHandle;
osThreadId CAN_SendManagerHandle;
osThreadId automaticStartAcquisitionMonitoringHandle;
osThreadId GPSUnboxingTaskHandle;
osThreadId updateTimestampHandle;
osMessageQId digitalAuxQueueHandle;
osMessageQId ErrorQueueHandle;
osMessageQId Usart1TxModeQueueHandle;
osMessageQId Usart1LockQueueHandle;
osMessageQId usbEventQueueHandle;
osMessageQId startAcquisitionEventHandle;
osMessageQId canFifo0QueueHandle;
osMessageQId canFifo1QueueHandle;
osMessageQId CAN_SendDataQueueHandle;
osSemaphoreId adc1SemaphoreHandle;
osSemaphoreId adc2SemaphoreHandle;
osSemaphoreId autogearSemaphoreHandle;
osSemaphoreId USB_OvercurrentSemaphoreHandle;
osSemaphoreId sendDataSemaphoreHandle;
osSemaphoreId sendStateSemaphoreHandle;
osSemaphoreId receiveCommandSemaphoreHandle;
osSemaphoreId sendFollowingDataSemaphoreHandle;
osSemaphoreId saveUsbSemaphoreHandle;
osSemaphoreId CAN_SendSemaphoreHandle;
osSemaphoreId automaticStartAcquisitionSemaphoreHandle;
osSemaphoreId GPSUnboxSemHandle;
osSemaphoreId updateTimestampSemaphoreHandle;
osSemaphoreId CAN_SendDataSemaphoreCounterHandle;

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
void sendDebugDataTask(void const * argument);
void CAN_SendManagerTask(void const * argument);
void automaticStartAcquisitionMonitoringTask(void const * argument);
void GPSUnboxingFunc(void const * argument);
void updateTimestampTask(void const * argument);

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

  /* definition and creation of CAN_SendSemaphore */
  osSemaphoreDef(CAN_SendSemaphore);
  CAN_SendSemaphoreHandle = osSemaphoreCreate(osSemaphore(CAN_SendSemaphore), 1);

  /* definition and creation of automaticStartAcquisitionSemaphore */
  osSemaphoreDef(automaticStartAcquisitionSemaphore);
  automaticStartAcquisitionSemaphoreHandle = osSemaphoreCreate(osSemaphore(automaticStartAcquisitionSemaphore), 1);

  /* definition and creation of GPSUnboxSem */
  osSemaphoreDef(GPSUnboxSem);
  GPSUnboxSemHandle = osSemaphoreCreate(osSemaphore(GPSUnboxSem), 1);

  /* definition and creation of updateTimestampSemaphore */
  osSemaphoreDef(updateTimestampSemaphore);
  updateTimestampSemaphoreHandle = osSemaphoreCreate(osSemaphore(updateTimestampSemaphore), 1);

  /* definition and creation of CAN_SendDataSemaphoreCounter */
  osSemaphoreDef(CAN_SendDataSemaphoreCounter);
  CAN_SendDataSemaphoreCounterHandle = osSemaphoreCreate(osSemaphore(CAN_SendDataSemaphoreCounter), 3);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  xSemaphoreTake(saveUsbSemaphoreHandle, portMAX_DELAY);                			/* Start with the task locked */
	xSemaphoreTake(sendStateSemaphoreHandle, portMAX_DELAY); 										/* Start with the task locked */
	xSemaphoreTake(sendDataSemaphoreHandle, portMAX_DELAY); 										/* Start with the task locked */
	xSemaphoreTake(receiveCommandSemaphoreHandle, portMAX_DELAY); 							/* Start with the task locked */
	xSemaphoreTake(sendFollowingDataSemaphoreHandle, portMAX_DELAY); 						/* Start with the task locked */
  xSemaphoreTake(adc1SemaphoreHandle, portMAX_DELAY);                  			 	/* Start with the task locked */
  xSemaphoreTake(adc2SemaphoreHandle, portMAX_DELAY);                   			/* Start with the task locked */
  xSemaphoreTake(autogearSemaphoreHandle, portMAX_DELAY);               			/* Start with the task locked */
  xSemaphoreTake(USB_OvercurrentSemaphoreHandle, portMAX_DELAY);        			/* Start with the task locked */
  xSemaphoreTake(CAN_SendSemaphoreHandle, portMAX_DELAY);               			/* Start with the task locked */
  xSemaphoreTake(automaticStartAcquisitionSemaphoreHandle, portMAX_DELAY);    /* Start with the task locked */
  xSemaphoreTake(GPSUnboxSemHandle, portMAX_DELAY);                           /* Start with the task locked */
	xSemaphoreTake(updateTimestampSemaphoreHandle, portMAX_DELAY);              /* Start with the task locked */
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
  osMessageQDef(canFifo0Queue, 32, CAN_RxPacket_t);
  canFifo0QueueHandle = osMessageCreate(osMessageQ(canFifo0Queue), NULL);

  /* definition and creation of canFifo1Queue */
  osMessageQDef(canFifo1Queue, 32, CAN_RxPacket_t);
  canFifo1QueueHandle = osMessageCreate(osMessageQ(canFifo1Queue), NULL);

  /* definition and creation of CAN_SendDataQueue */
  osMessageQDef(CAN_SendDataQueue, 8, CAN_TxPacket_t);
  CAN_SendDataQueueHandle = osMessageCreate(osMessageQ(CAN_SendDataQueue), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
	
	/* Start the transmit process */
	uint8_t usart1LockFlag = UART1_CLEAR_FLAG;
	xQueueSend(Usart1LockQueueHandle, &usart1LockFlag, 0);
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of alive */
  osThreadDef(alive, aliveTask, osPriorityIdle, 0, 128);
  aliveHandle = osThreadCreate(osThread(alive), NULL);

  /* definition and creation of adc1Conversion */
  osThreadDef(adc1Conversion, adc1ConversionTask, osPriorityLow, 0, 128);
  adc1ConversionHandle = osThreadCreate(osThread(adc1Conversion), NULL);

  /* definition and creation of adc2Conversion */
  osThreadDef(adc2Conversion, adc2ConversionTask, osPriorityIdle, 0, 128);
  adc2ConversionHandle = osThreadCreate(osThread(adc2Conversion), NULL);

  /* definition and creation of digitalAuxManager */
  osThreadDef(digitalAuxManager, digitalAuxManagerTask, osPriorityIdle, 0, 128);
  digitalAuxManagerHandle = osThreadCreate(osThread(digitalAuxManager), NULL);

  /* definition and creation of autogearManager */
  osThreadDef(autogearManager, autogearManagerTask, osPriorityLow, 0, 128);
  autogearManagerHandle = osThreadCreate(osThread(autogearManager), NULL);

  /* definition and creation of USB_OvercurrentManager */
  osThreadDef(USB_OvercurrentManager, USB_OvercurrentManagerTask, osPriorityLow, 0, 128);
  USB_OvercurrentManagerHandle = osThreadCreate(osThread(USB_OvercurrentManager), NULL);

  /* definition and creation of SendStates */
  osThreadDef(SendStates, SendStatesFunc, osPriorityBelowNormal, 0, 128);
  SendStatesHandle = osThreadCreate(osThread(SendStates), NULL);

  /* definition and creation of SendData */
  osThreadDef(SendData, SendDataFunc, osPriorityBelowNormal, 0, 128);
  SendDataHandle = osThreadCreate(osThread(SendData), NULL);

  /* definition and creation of ReceiveTelem */
  osThreadDef(ReceiveTelem, ReceiveTelemFunc, osPriorityNormal, 0, 128);
  ReceiveTelemHandle = osThreadCreate(osThread(ReceiveTelem), NULL);

  /* definition and creation of SendError */
  osThreadDef(SendError, SendErrorFunc, osPriorityNormal, 0, 128);
  SendErrorHandle = osThreadCreate(osThread(SendError), NULL);

  /* definition and creation of SendFollowingData */
  osThreadDef(SendFollowingData, SendFollowingDataFunc, osPriorityBelowNormal, 0, 128);
  SendFollowingDataHandle = osThreadCreate(osThread(SendFollowingData), NULL);

  /* definition and creation of saveUsb */
  osThreadDef(saveUsb, saveUsbTask, osPriorityRealtime, 0, 512);
  saveUsbHandle = osThreadCreate(osThread(saveUsb), NULL);

  /* definition and creation of usbManager */
  osThreadDef(usbManager, usbManagerTask, osPriorityLow, 0, 512);
  usbManagerHandle = osThreadCreate(osThread(usbManager), NULL);

  /* definition and creation of startAcquisitionStateMachine */
  osThreadDef(startAcquisitionStateMachine, startAcquisitionStateMachineTask, osPriorityNormal, 0, 128);
  startAcquisitionStateMachineHandle = osThreadCreate(osThread(startAcquisitionStateMachine), NULL);

  /* definition and creation of canFifo0Unpack */
  osThreadDef(canFifo0Unpack, canFifo0UnpackTask, osPriorityHigh, 0, 512);
  canFifo0UnpackHandle = osThreadCreate(osThread(canFifo0Unpack), NULL);

  /* definition and creation of canFifo1Unpack */
  osThreadDef(canFifo1Unpack, canFifo1UnpackTask, osPriorityHigh, 0, 512);
  canFifo1UnpackHandle = osThreadCreate(osThread(canFifo1Unpack), NULL);

  /* definition and creation of sendDebugData */
  osThreadDef(sendDebugData, sendDebugDataTask, osPriorityBelowNormal, 0, 512);
  sendDebugDataHandle = osThreadCreate(osThread(sendDebugData), NULL);

  /* definition and creation of CAN_SendManager */
  osThreadDef(CAN_SendManager, CAN_SendManagerTask, osPriorityNormal, 0, 512);
  CAN_SendManagerHandle = osThreadCreate(osThread(CAN_SendManager), NULL);

  /* definition and creation of automaticStartAcquisitionMonitoring */
  osThreadDef(automaticStartAcquisitionMonitoring, automaticStartAcquisitionMonitoringTask, osPriorityBelowNormal, 0, 128);
  automaticStartAcquisitionMonitoringHandle = osThreadCreate(osThread(automaticStartAcquisitionMonitoring), NULL);

  /* definition and creation of GPSUnboxingTask */
  osThreadDef(GPSUnboxingTask, GPSUnboxingFunc, osPriorityNormal, 0, 512);
  GPSUnboxingTaskHandle = osThreadCreate(osThread(GPSUnboxingTask), NULL);

  /* definition and creation of updateTimestamp */
  osThreadDef(updateTimestamp, updateTimestampTask, osPriorityLow, 0, 128);
  updateTimestampHandle = osThreadCreate(osThread(updateTimestamp), NULL);

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
	USB_InitStart();		/* USB peripheral config and start */
	MX_FATFS_Init();		/* FatFS init */
	
	HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET); 			/* Green LED off as default */
	HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_RESET); 		/* Yellow LED off as default */
	HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET); 					/* Red LED off as default */
	HAL_TIM_Base_Start_IT(&htim5); 		/* Start timer 5 (1 Hz) in interrupt mode */
	HAL_TIM_Base_Start_IT(&htim6); 		/* Start timer 6 (10 Hz) in interrupt mode */
  
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
	ADC_BuffersInit();		/* ADC buffer initialization */
	
  /* Infinite loop */
  for(;;) {
		xSemaphoreTake(adc1SemaphoreHandle, portMAX_DELAY); 		/* Unlock when DMA callback is called */
		ADC_ReadDataDebug();																		/* Debug channels parsing and conversion */
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
		ADC_ReadDataAux();																			/* Aux channels parsing and conversion */
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
			GPIO_AuxManageEvent(digitalAuxEvent); 		/* Digital aux event manager*/
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
    xSemaphoreTake(autogearSemaphoreHandle, portMAX_DELAY); 		/* Unlock when interrupt callback is called */
		osDelay(15);																								/* Wait to reject false trigger detection */
		CAN_SendAutogearPacket();																		/* Send autogear packet event to GCU*/
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
		xSemaphoreTake(USB_OvercurrentSemaphoreHandle, portMAX_DELAY); 		/* Unlock when interrupt callback is called */
		osDelay(1);																												/* Wait to reject false trigger detection */
		USB_OvercurrentEvent();																						/* Handle the overcurrent event */
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
	uint8_t normalTxModeFlag = NORMAL_MODE_TX_FLAG;  		/* Setting flag identifier to put later into the queue */
  uint8_t usartLockFlag;
	uint8_t strToSend[BUFFER_STATE_LEN/2+5];
	uint16_t strToSenLen;
	
	DATA_ResetStateBuffer();		/* Reset state buffer: all off */
	DATA_SetTelemetryState();		/* Start telemetry as default */
	
	/* Infinite loop */
  for(;;) {
    xSemaphoreTake(sendStateSemaphoreHandle, portMAX_DELAY);																				/* Unlock when timer callback is called */
		strToSenLen = encodeString(DATA_StateBuffer, strToSend, BUFFER_STATE_LEN, STATE_MESSAGE_ID); 		/* Get the encoded string */		
		xQueueReceive(Usart1LockQueueHandle, &usartLockFlag, portMAX_DELAY);														/* Lock if DMA is in use */
		xQueueSend(Usart1TxModeQueueHandle, (void *)&normalTxModeFlag, (TickType_t)0); 									/* Add flag to queue */
		HAL_UART_Transmit_DMA(&huart1, strToSend, strToSenLen); 																				/* Transmit state message */
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
	uint8_t secondTxModeFlag = SECOND_HALF_TX_FLAG; 		/* Setting flag identifier to put later into the queue */
	uint8_t strToSend[BUFFER_BLOCK_LEN/2+10];
	uint16_t strToSenLen;
  
	/* Infinite loop */
  for(;;) {
    xSemaphoreTake(sendDataSemaphoreHandle, portMAX_DELAY); 	/* Unlock when timer callback is called */

		if(DATA_GetTelemetryState() == STATE_ON) {
      strToSenLen = encodeString(DATA_BlockBuffer[DATA_BlockReadIndex], strToSend, HALF_DATA_INDEX, DATA_MESSAGE_ID);  /* Get the encoded string */
      xQueueReceive(Usart1LockQueueHandle, &usartLockFlag, portMAX_DELAY);              /* Lock if DMA is in use */
      xQueueSend(Usart1TxModeQueueHandle, (void *)&secondTxModeFlag, (TickType_t)0);    /* Add flag to queue */
      HAL_UART_Transmit_DMA(&huart1, strToSend, strToSenLen - 1);                       /* Transmit first half of data message */
		}
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
	if(HAL_UART_Receive_IT(&huart1, telemetryIndBuffer, 1) != HAL_OK) {
		uint8_t startRxErrorLetter = START_RX_ERR_ID;
		xQueueSend(ErrorQueueHandle, (void *)&startRxErrorLetter, (TickType_t)0); 					/* Add error to queue */
	}
	/* Infinite loop */
  for(;;) {
    xSemaphoreTake(receiveCommandSemaphoreHandle, portMAX_DELAY); 		/* Unlock when uart rx from telemetry is completed */
		TELEMETRY_Receive();
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
	uint8_t errorMsg[ERROR_MSG_LEN] = ERROR_MSG; 																					/* Set standard error message to be edited later */
	uint8_t normalTxModeFlag = NORMAL_MODE_TX_FLAG; 																			/* Setting flag identifier to put later into the queue */
  uint8_t usartLockFlag;
	
	/* Infinite loop */
  for(;;) {
    xQueueReceive(ErrorQueueHandle, &queueLetter, portMAX_DELAY); 											/* Wait for error, get the identifier char from the queue */
    errorMsg[ERROR_MSG_IDENTIFIER_POS] = queueLetter; 																	/* Set error identifier into the message */
		xQueueReceive(Usart1LockQueueHandle, &usartLockFlag, portMAX_DELAY);								/* Lock if DMA is in use */
		xQueueSend(Usart1TxModeQueueHandle, (void *) &normalTxModeFlag, (TickType_t)0); 		/* Add flag to queue */
		HAL_UART_Transmit_DMA(&huart1, errorMsg, ERROR_MSG_LEN); 														/* Transmit error message */
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
	uint8_t normalTxModeFlag = NORMAL_MODE_TX_FLAG;			/* Setting flag identifier to put later into the queue */
  uint8_t strToSend[BUFFER_BLOCK_LEN/2+10];
	uint16_t strToSenLen;
	
	/* Infinite loop */
  for(;;) {
    xSemaphoreTake(sendFollowingDataSemaphoreHandle, portMAX_DELAY); 									/* Unlock when first part tx is completed, unlocked from tx complete callback */
		xQueueSend(Usart1TxModeQueueHandle, (void *)&normalTxModeFlag, (TickType_t)0); 		/* Add flag to queue */
		strToSenLen = encodeString(DATA_BlockBuffer[DATA_BlockReadIndex] + HALF_DATA_INDEX, strToSend, HALF_DATA_INDEX, SECOND_DATA_MESSAGE_ID); 		/* Get the encoded string */
		HAL_UART_Transmit_DMA(&huart1, strToSend + 1, strToSenLen); 											/* Transmit first half of data message */
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
		USB_SavingTask();																					/* Saving task */
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
	uint8_t USB_Event;
	
  /* Infinite loop */
  for(;;) {
		xQueueReceive(usbEventQueueHandle, &USB_Event, portMAX_DELAY);		/* Wait for and event */
		USB_EventHandler(USB_Event); 																			/* Manage USB insertion evets */														
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
		/* NOTE: Events are coded using chars, NOT numbers! */
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
	CAN_RxPacket_t CAN_UnpackedData_FIFO0;
	
	Flash_LoadCalibration(&OffsetHandler);
	DATA_PacketReset();					/* Reset the data saving buffer */
	CAN_PacketCounterReset();		/* Reset the CAN packets recevide counter */
	CAN_Start();								/* CAN filter config and start */
	
  /* Infinite loop */
  for(;;) {		
		if(xQueueReceive(canFifo0QueueHandle, &CAN_UnpackedData_FIFO0, portMAX_DELAY) == pdTRUE) {		
			DATA_CanParser_FIFO0(&CAN_UnpackedData_FIFO0);  	/* Data parsing and conversion */
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
	CAN_RxPacket_t CAN_UnpackedData_FIFO1;
	
  /* Infinite loop */
  for(;;) {
		if(xQueueReceive(canFifo1QueueHandle, &CAN_UnpackedData_FIFO1, portMAX_DELAY) == pdTRUE) {		
			DATA_CanParser_FIFO1(&CAN_UnpackedData_FIFO1);  	/* Data parsing and convertion */
		}
  }
  /* USER CODE END canFifo1UnpackTask */
}

/* USER CODE BEGIN Header_sendDebugDataTask */
/**
* @brief Function implementing the sendDebugData thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_sendDebugDataTask */
void sendDebugDataTask(void const * argument)
{
  /* USER CODE BEGIN sendDebugDataTask */
  /* Infinite loop */
  for(;;) {
		xSemaphoreTake(CAN_SendSemaphoreHandle, portMAX_DELAY);			/* Unlock when timer callback is called */
		CAN_SendDebugPackets();
  }
  /* USER CODE END sendDebugDataTask */
}

/* USER CODE BEGIN Header_CAN_SendManagerTask */
/**
* @brief Function implementing the CAN_SendManager thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_CAN_SendManagerTask */
void CAN_SendManagerTask(void const * argument)
{
  /* USER CODE BEGIN CAN_SendManagerTask */
	uint8_t errorLetter = CAN_TX_ERROR;
	uint32_t packetMailbox;
	CAN_TxPacket_t CAN_TxPacket;
	HAL_StatusTypeDef CAN_SendError;
	
  /* Infinite loop */
  for(;;) {
		xQueueReceive(CAN_SendDataQueueHandle, &CAN_TxPacket, portMAX_DELAY);			/* Wait to transmit a packet */
		xSemaphoreTake(CAN_SendDataSemaphoreCounterHandle, portMAX_DELAY);				/* Decrement CAN counting semapgore */
		CAN_SendError = HAL_CAN_AddTxMessage(&hcan1, &CAN_TxPacket.packetHeader, CAN_TxPacket.packetData, &packetMailbox);		/* Send CAN message */
		
		if(CAN_SendError != HAL_OK) {
			xQueueSend(ErrorQueueHandle, (void *)&errorLetter, (TickType_t)0); 			/* Add error to queue */
		}
  }
  /* USER CODE END CAN_SendManagerTask */
}

/* USER CODE BEGIN Header_automaticStartAcquisitionMonitoringTask */
/**
* @brief Function implementing the automaticStartAcquisitionMonitoring thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_automaticStartAcquisitionMonitoringTask */
void automaticStartAcquisitionMonitoringTask(void const * argument)
{
  /* USER CODE BEGIN automaticStartAcquisitionMonitoringTask */
  /* Infinite loop */
  for(;;) {
		xSemaphoreTake(automaticStartAcquisitionSemaphoreHandle, portMAX_DELAY);		/* Unlock when timer callback is called */
		DATA_AutomaticStartAcquisitionManager(); 																		/* Check if start/stop automatic start acquisition */
  }
  /* USER CODE END automaticStartAcquisitionMonitoringTask */
}

/* USER CODE BEGIN Header_GPSUnboxingFunc */
/**
* @brief Function implementing the GPSUnboxingTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_GPSUnboxingFunc */
void GPSUnboxingFunc(void const * argument)
{
  /* USER CODE BEGIN GPSUnboxingFunc */
	GPS_Init();																					/* GPS init */
	HAL_UART_Receive_IT(&huart2, &GPS_FirstChar, 1);		/* Start UART RX */
  
	/* Infinite loop */
  for(;;) {
		xSemaphoreTake(GPSUnboxSemHandle, portMAX_DELAY);
		GPS_DataConversion(GPS_RawBuffer);
  }
  /* USER CODE END GPSUnboxingFunc */
}

/* USER CODE BEGIN Header_updateTimestampTask */
/**
* @brief Function implementing the updateTimestamp thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_updateTimestampTask */
void updateTimestampTask(void const * argument)
{
  /* USER CODE BEGIN updateTimestampFromRtcTask */
  resetActualTimestamp();			/* Reset the timestamp struct */
	rtcPeripheralInit();				/* RTC peripheral init only */
	//resetRtcTime();						/* RTC time values reset */
	//resetRtcDate();						/* RTC date values reset */
	
	/* Infinite loop */
  for(;;) {
    xSemaphoreTake(updateTimestampSemaphoreHandle, portMAX_DELAY); 			/* Unlock when timer callback is called */
		
		/* You must call GetData after GetTime to unlock the data */
		setTimestampTimeFormRtc(); 																					/* Set time values in the timestamp struct, from RTC */
		setTimestampDateFormRtc(); 																					/* Set date values in the timestamp struct, from RTC */
		setTimestampTimeFormGps();																					/* Set time values in the timestamp struct, from GPS */
		setTimestampDateFormGps();																					/* Set date values in the timestamp struct, from GPS*/
  }
  /* USER CODE END updateTimestampFromRtcTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
