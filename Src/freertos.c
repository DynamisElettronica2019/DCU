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

/* USER CODE END Variables */
osThreadId aliveHandle;
osThreadId SendStatesHandle;
osThreadId SendDataHandle;
osThreadId ReceiveTelemHandle;
osThreadId SendErrorHandle;
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
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

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
    osDelay(1);
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
    osDelay(1);
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
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
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
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END SendErrorFunc */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
