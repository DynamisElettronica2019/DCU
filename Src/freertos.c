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
#include "user_defines.h"
#include "usart.h"
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
/*
* Buffer declaration
*/
uint8_t dcu_State_Packet[BUFFER_STATE_LEN] = "[S;1;0;0;0;0;0;0;0]";
uint8_t block_Buffer[BUFFER_BLOCK_LEN] = "[C;000000;000.0;000.0;000.0;000.0;000;000;000;000;000;000;000;00.0;0;00000;000;000;000.0;000.0;0000.0;0;0;000;0;00000;00000;0.000;00000;0;0;000;000;00.00;00000;00.0;00.00;00000;00.0;00000;00.00;0000;00.00;00000;00000;00000;000;000;000;000;000;000;000;000;000;000;000;000;0000.00;0000.00;000.00;0000.00;0000.00;0000.00;000;000;0;0;0;000;000;00000;00000;00000;00000;00000;00000;000;0000;000;0000;000;0000;000;0000;000;0000000000;0000000;0000000;0000000;000000000000000000000000000000000000000000000000000000000000]";
uint8_t telemRxBuffer[BUFFER_COMMAND_LEN];
uint8_t setRtcRxBuffer[BUFFER_RTC_SET_LEN];
/* USER CODE END Variables */
osThreadId aliveHandle;
osThreadId SendStatesHandle;
osThreadId SendDataHandle;
osThreadId ReceiveTelemHandle;
osThreadId SendErrorHandle;
osMessageQId ErrorQueueHandle;
osMessageQId Usart1TxModeQueueHandle;
osSemaphoreId sendDataSemaphoreHandle;
osSemaphoreId sendStateSemaphoreHandle;
osSemaphoreId receiveCommandSemaphoreHandle;
osSemaphoreId uart1SemHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void aliveTask(void const * argument);
void SendStatesFunc(void const * argument);
void SendDataFunc(void const * argument);
void ReceiveTelemFunc(void const * argument);
void SendErrorFunc(void const * argument);

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

  /* definition and creation of uart1Sem */
  osSemaphoreDef(uart1Sem);
  uart1SemHandle = osSemaphoreCreate(osSemaphore(uart1Sem), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
	xSemaphoreTake(sendStateSemaphoreHandle, portMAX_DELAY); //Start locked
	xSemaphoreTake(sendDataSemaphoreHandle, portMAX_DELAY); //Start locked
	xSemaphoreTake(receiveCommandSemaphoreHandle, portMAX_DELAY); //Start locked
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

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of alive */
  osThreadDef(alive, aliveTask, osPriorityLow, 0, 128);
  aliveHandle = osThreadCreate(osThread(alive), NULL);

  /* definition and creation of SendStates */
  osThreadDef(SendStates, SendStatesFunc, osPriorityIdle, 0, 128);
  SendStatesHandle = osThreadCreate(osThread(SendStates), NULL);

  /* definition and creation of SendData */
  osThreadDef(SendData, SendDataFunc, osPriorityIdle, 0, 128);
  SendDataHandle = osThreadCreate(osThread(SendData), NULL);

  /* definition and creation of ReceiveTelem */
  osThreadDef(ReceiveTelem, ReceiveTelemFunc, osPriorityIdle, 0, 128);
  ReceiveTelemHandle = osThreadCreate(osThread(ReceiveTelem), NULL);

  /* definition and creation of SendError */
  osThreadDef(SendError, SendErrorFunc, osPriorityIdle, 0, 128);
  SendErrorHandle = osThreadCreate(osThread(SendError), NULL);

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
  for(;;)
  {
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
  /* Infinite loop */
  for(;;)
  {
    xSemaphoreTake(sendStateSemaphoreHandle, portMAX_DELAY); //Unlock when timer callback is called
		xSemaphoreTake(uart1SemHandle, portMAX_DELAY); //Lock if DMA is in use
		HAL_UART_Transmit_DMA(&huart1, dcu_State_Packet, BUFFER_STATE_LEN); //Transmit state message
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
  /* Infinite loop */
  for(;;)
  {
    xSemaphoreTake(sendDataSemaphoreHandle, portMAX_DELAY); //Unlock when timer callback is called
		xSemaphoreTake(uart1SemHandle, portMAX_DELAY); //Lock if DMA is in use
		HAL_UART_Transmit_DMA(&huart1, block_Buffer, BUFFER_BLOCK_LEN); //Transmit data message
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
	uint8_t errorLetter = CMD_READ_ERR_ID; // You have to define which char is the identifier of the error, and add it to the queue for waking up the error sender task
	uint8_t commandAckMsg[COMMAND_ACK_MSG_LEN] = ACK_MSG;
	_Bool setRtcComing = 0;
  /* Infinite loop */
  for(;;)
  {
    xSemaphoreTake(receiveCommandSemaphoreHandle, portMAX_DELAY); //Unlock when uart rx from telemetry is completed
		if(setRtcComing == 1) { // If you are waiting for set rtc parameter
			if(setRtcRxBuffer[BUFFER_RTC_SET_LEN - 1] == MESSAGE_END_ID) { // Check if message ends correctly
				
				// User code to set rtc
				
				xSemaphoreTake(uart1SemHandle, portMAX_DELAY); //Lock if DMA is in use
				commandAckMsg[COMMAND_ACK_IDENTIFIER_POS] = SET_RTC_ID; // Set the correct identifier
				HAL_UART_Transmit_DMA(&huart1, commandAckMsg, COMMAND_ACK_MSG_LEN); //Transmit ack message
				HAL_UART_Receive_DMA(&huart1, telemRxBuffer, BUFFER_COMMAND_LEN); // Re enable receiving
			}
			else { // If message does not end correctly
				xQueueSend(ErrorQueueHandle, ( void * ) &errorLetter, ( TickType_t ) 0 ); // Add error to queue
			}
		}
		else { // If you are waiting for standard message
			if(telemRxBuffer[0] == MESSAGE_INIT_ID) { // If message starts correctly with [
				if(telemRxBuffer[BUFFER_COMMAND_LEN-1] == MESSAGE_END_ID) { // If message ends correctly as standard message
						switch (telemRxBuffer[BUFFER_COMMAND_LEN-2]) {
						case RESET_TELEM_ID: // Reset telemetry
							/*
							* Here put user code necessary for resetting telemetry
							*/
							xSemaphoreTake(uart1SemHandle, portMAX_DELAY); //Lock if DMA is in use
							commandAckMsg[COMMAND_ACK_IDENTIFIER_POS] = RESET_TELEM_ID; // Set the correct identifier
							HAL_UART_Transmit_DMA(&huart1, commandAckMsg, COMMAND_ACK_MSG_LEN); //Transmit ack message
							break;
						case START_ACQ_ID: // Start Acquisition
							/*
							* Here put user code necessary for starting acquisition
							*/
							xSemaphoreTake(uart1SemHandle, portMAX_DELAY); //Lock if DMA is in use
							commandAckMsg[COMMAND_ACK_IDENTIFIER_POS] = START_ACQ_ID; // Set the correct identifier
							HAL_UART_Transmit_DMA(&huart1, commandAckMsg, COMMAND_ACK_MSG_LEN); //Transmit ack message
							break;	
						case STOP_ACQ_ID: // Stop Acquisition
							/*
							* Here put user code necessary for stopping acquisition
							*/
							xSemaphoreTake(uart1SemHandle, portMAX_DELAY); //Lock if DMA is in use
							commandAckMsg[COMMAND_ACK_IDENTIFIER_POS] = STOP_ACQ_ID; // Set the correct identifier
							HAL_UART_Transmit_DMA(&huart1, commandAckMsg, COMMAND_ACK_MSG_LEN); //Transmit ack message
							break;
						case GIVE_RTC_TIME_ID: // Give RTC Time
							/*
							* Here put user code necessary for giving rtc time
							*/
							xSemaphoreTake(uart1SemHandle, portMAX_DELAY); //Lock if DMA is in use
							commandAckMsg[COMMAND_ACK_IDENTIFIER_POS] = GIVE_RTC_TIME_ID; // Set the correct identifier
							HAL_UART_Transmit_DMA(&huart1, commandAckMsg, COMMAND_ACK_MSG_LEN); //Transmit ack message
							break;
						case GIVE_RTC_DATE_ID: // Give RTC Data
							/*
							* Here put user code necessary for Giving rtc data
							*/
							xSemaphoreTake(uart1SemHandle, portMAX_DELAY); //Lock if DMA is in use
							commandAckMsg[COMMAND_ACK_IDENTIFIER_POS] = GIVE_RTC_DATE_ID; // Set the correct identifier
							HAL_UART_Transmit_DMA(&huart1, commandAckMsg, COMMAND_ACK_MSG_LEN); //Transmit ack message
							break;
						default: // Message not recognised
						{
							xQueueSend(ErrorQueueHandle, ( void * ) &errorLetter, ( TickType_t ) 0 ); // Add error to queue
						}
						break;
						}
						HAL_UART_Receive_DMA(&huart1, telemRxBuffer, BUFFER_COMMAND_LEN); // Re enable receiving
				}
				else { // If message does not end here
					if(telemRxBuffer[BUFFER_COMMAND_LEN-2] == SET_RTC_ID) { // If the message type is set rtc, wait for the parameter
						setRtcComing = 1;
						HAL_UART_Receive_DMA(&huart1, setRtcRxBuffer, BUFFER_RTC_SET_LEN); // Re enable receiving, wait for parameter
					}
					else { // Not set rtc type -> error
						HAL_UART_Receive_DMA(&huart1, telemRxBuffer, BUFFER_COMMAND_LEN); // Re enable receiving
						xQueueSend(ErrorQueueHandle, ( void * ) &errorLetter, ( TickType_t ) 0 ); // Add error to queue
					}
				}
			}
			else { // If message does not start correctly
				HAL_UART_Receive_DMA(&huart1, telemRxBuffer, BUFFER_COMMAND_LEN); // Re enable receiving
				xQueueSend(ErrorQueueHandle, ( void * ) &errorLetter, ( TickType_t ) 0 ); // Add error to queue
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
	uint8_t errorMsg[ERROR_MSG_LEN] = ERROR_MSG;
  /* Infinite loop */
  for(;;)
  {
    xQueueReceive(ErrorQueueHandle, &queueLetter, portMAX_DELAY); // Wait for error, get the identifier char from the queue
    errorMsg[ERROR_MSG_IDENTIFIER_POS] = queueLetter; // Set error identifier into the message
		xSemaphoreTake(uart1SemHandle, portMAX_DELAY); //Lock if DMA is in use
		HAL_UART_Transmit_DMA(&huart1, errorMsg, ERROR_MSG_LEN); //Transmit error message
  }
  /* USER CODE END SendErrorFunc */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
