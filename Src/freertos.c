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
#include "fatfs.h"
#include "sdmmc.h"

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
osThreadId uSD_ControlHandle;
osSemaphoreId uSD_WriteSemaphoreHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void aliveTask(void const * argument);
void uSD_ControlTask(void const * argument);

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
  /* definition and creation of uSD_WriteSemaphore */
  osSemaphoreDef(uSD_WriteSemaphore);
  uSD_WriteSemaphoreHandle = osSemaphoreCreate(osSemaphore(uSD_WriteSemaphore), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
	xSemaphoreTake(uSD_WriteSemaphoreHandle, portMAX_DELAY);				/* Start with the task locked */
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

  /* definition and creation of uSD_Control */
  osThreadDef(uSD_Control, uSD_ControlTask, osPriorityNormal, 0, 512);
  uSD_ControlHandle = osThreadCreate(osThread(uSD_Control), NULL);

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
		vTaskDelay(250 / portTICK_PERIOD_MS);
  }
  /* USER CODE END aliveTask */
}

/* USER CODE BEGIN Header_uSD_ControlTask */
/**
* @brief Function implementing the uSD_Control thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_uSD_ControlTask */
void uSD_ControlTask(void const * argument)
{
  /* USER CODE BEGIN uSD_ControlTask */
	uint8_t SD_ErrorCode = FR_OK;
	
	FIL SD_File;
	uint8_t SDFileName[] = "td.txt";
	
	uint8_t SD_WriteBuffer[20];
	uint32_t queueData = 0;
	
	SD_ErrorCode = SD_Init();
	if(SD_ErrorCode != FR_OK) while(1);
	
	SD_ErrorCode = SD_openFile(SDFileName, &SD_File);
	if(SD_ErrorCode != FR_OK) while(1);
	
  /* Infinite loop */
  for(;;) {
		xSemaphoreTake(uSD_WriteSemaphoreHandle, portMAX_DELAY);		/* Unlock when timer callback is called */
		sprintf(SD_WriteBuffer, "Count -> %8d\n", queueData);
		SD_ErrorCode = SD_writeString(SD_WriteBuffer, 18, &SD_File);
		
		if(SD_ErrorCode != FR_OK) {
			while(1);			
		}
		else {
			SD_ErrorCode = SD_refreshFile(SDFileName, &SD_File);
			if(SD_ErrorCode != FR_OK) while(1);		
			HAL_GPIO_TogglePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin);	
		}

  }
  /* USER CODE END uSD_ControlTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
