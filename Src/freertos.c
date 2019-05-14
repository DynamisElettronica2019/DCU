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
#include "data.h"
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
osThreadId canFifo0UnpackHandle;
osThreadId canFifo1UnpackHandle;
osMessageQId canFifo0QueueHandle;
osMessageQId canFifo1QueueHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void aliveTask(void const * argument);
void canFifo0UnpackTask(void const * argument);
void canFifo1UnpackTask(void const * argument);

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

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */

  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of canFifo0Queue */
  osMessageQDef(canFifo0Queue, 32, CAN_RxPacketTypeDef);
  canFifo0QueueHandle = osMessageCreate(osMessageQ(canFifo0Queue), NULL);

  /* definition and creation of canFifo1Queue */
  osMessageQDef(canFifo1Queue, 32, CAN_RxPacketTypeDef);
  canFifo1QueueHandle = osMessageCreate(osMessageQ(canFifo1Queue), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of alive */
  osThreadDef(alive, aliveTask, osPriorityLow, 0, 128);
  aliveHandle = osThreadCreate(osThread(alive), NULL);

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
  /* init code for FATFS */
  MX_FATFS_Init();

  /* init code for USB_HOST */
  MX_USB_HOST_Init();

  /* USER CODE BEGIN aliveTask */
  /* Infinite loop */
  for(;;) {
    HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
		vTaskDelay(250 / portTICK_PERIOD_MS);
  }
  /* USER CODE END aliveTask */
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
		if(xQueueReceive(canFifo0QueueHandle, &unpackedData, portMAX_DELAY)) {		
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
		if(xQueueReceive(canFifo1QueueHandle, &unpackedData, portMAX_DELAY)) {		
			canDataParser(&unpackedData);
		}
  }
  /* USER CODE END canFifo1UnpackTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
