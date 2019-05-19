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
#include "usart.h"
#include "data.h"
#include "string.h"
#include "telemetry.h"
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
extern uint8_t dcuStateBuffer[BUFFER_STATE_LEN];
extern uint8_t blockBuffer [BUFFER_BLOCK_LEN];
extern uint8_t telemetryReceivedBuffer [BUFFER_COMMAND_LEN];
extern uint8_t setRtcReceivedBuffer [BUFFER_RTC_SET_LEN];
/* USER CODE END Variables */
osThreadId aliveHandle;
osThreadId SendStatesHandle;
osThreadId SendDataHandle;
osThreadId ReceiveTelemHandle;
osThreadId SendErrorHandle;
osThreadId SendFollowingDataHandle;
osMessageQId ErrorQueueHandle;
osMessageQId Usart1TxModeQueueHandle;
osMessageQId Usart1LockQueueHandle;
osSemaphoreId sendDataSemaphoreHandle;
osSemaphoreId sendStateSemaphoreHandle;
osSemaphoreId receiveCommandSemaphoreHandle;
osSemaphoreId sendFollowingDataSemaphoreHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void aliveTask(void const * argument);
void SendStatesFunc(void const * argument);
void SendDataFunc(void const * argument);
void ReceiveTelemFunc(void const * argument);
void SendErrorFunc(void const * argument);
void SendFollowingDataFunc(void const * argument);

extern void MX_FATFS_Init(void);
extern void MX_USB_HOST_Init(void);
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

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
	xSemaphoreTake(sendStateSemaphoreHandle, portMAX_DELAY); 							/* Start with the task locked */
	xSemaphoreTake(sendDataSemaphoreHandle, portMAX_DELAY); 							/* Start with the task locked */
	xSemaphoreTake(receiveCommandSemaphoreHandle, portMAX_DELAY); 				/* Start with the task locked */
	xSemaphoreTake(sendFollowingDataSemaphoreHandle, portMAX_DELAY); 			/* Start with the task locked */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of ErrorQueue */
  osMessageQDef(ErrorQueue, 10, uint8_t);
  ErrorQueueHandle = osMessageCreate(osMessageQ(ErrorQueue), NULL);

  /* definition and creation of Usart1TxModeQueue */
  osMessageQDef(Usart1TxModeQueue, 1, uint8_t);
  Usart1TxModeQueueHandle = osMessageCreate(osMessageQ(Usart1TxModeQueue), NULL);

  /* definition and creation of Usart1LockQueue */
  osMessageQDef(Usart1LockQueue, 1, uint8_t);
  Usart1LockQueueHandle = osMessageCreate(osMessageQ(Usart1LockQueue), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
	uint8_t usart1LockFlag = UART1_CLEAR_FLAG;
	xQueueSend(Usart1LockQueueHandle, &usart1LockFlag, 0);
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of alive */
  osThreadDef(alive, aliveTask, osPriorityLow, 0, 128);
  aliveHandle = osThreadCreate(osThread(alive), NULL);

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
  /* init code for FATFS */
  MX_FATFS_Init();

  /* init code for USB_HOST */
  MX_USB_HOST_Init();

  /* USER CODE BEGIN aliveTask */
  /* Infinite loop */
  for(;;) {
    HAL_GPIO_TogglePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin);
		vTaskDelay(500 / portTICK_PERIOD_MS);
  }
  /* USER CODE END aliveTask */
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
		xQueueReceive(Usart1LockQueueHandle, &usartLockFlag, portMAX_DELAY);						/* Lock if DMA is in use */
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
	uint8_t strToSend[BUFFER_BLOCK_LEN/2+10];
	uint16_t strToSenLen;
  
	/* Infinite loop */
  for(;;) {
    xSemaphoreTake(sendDataSemaphoreHandle, portMAX_DELAY); 														/* Unlock when timer callback is called */
		strToSenLen = encodeString(blockBuffer, strToSend, BUFFER_BLOCK_LEN); 							/* Get the encoded string */
		xQueueReceive(Usart1LockQueueHandle, &usartLockFlag, portMAX_DELAY);								/* Lock if DMA is in use */
		xQueueSend(Usart1TxModeQueueHandle, (void *)&secondTxModeFlag, (TickType_t)0); 			/* Add flag to queue */
		HAL_UART_Transmit_DMA(&huart1, strToSend, strToSenLen); 														/* Transmit first half of data message */
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
			
			else {																																					/* If message does not start correctly */
				HAL_UART_Receive(&huart1, tempBuffer, 50, 50);
				HAL_UART_Receive_DMA(&huart1, telemetryReceivedBuffer, BUFFER_COMMAND_LEN); 			/* Re enable receiving */
				HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
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

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
