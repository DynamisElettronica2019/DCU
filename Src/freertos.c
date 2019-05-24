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

#include "GPS.h"
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

extern BaseType_t xGPSHigherPriorityTaskWoken;
extern uint8_t GPSRawBuffer[GPS_MAX_LENGTH];
extern uint8_t GPSFirstChar;

/* USER CODE END Variables */
osThreadId aliveHandle;
osThreadId GPSUnboxingTaskHandle;
osSemaphoreId GPSUnboxSemHandle;
osSemaphoreId GPSSetSemHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void aliveTask(void const * argument);
void GPSUnboxingFunc(void const * argument);

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
  /* definition and creation of GPSUnboxSem */
  osSemaphoreDef(GPSUnboxSem);
  GPSUnboxSemHandle = osSemaphoreCreate(osSemaphore(GPSUnboxSem), 1);

  /* definition and creation of GPSSetSem */
  osSemaphoreDef(GPSSetSem);
  GPSSetSemHandle = osSemaphoreCreate(osSemaphore(GPSSetSem), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  xSemaphoreTake(GPSUnboxSemHandle, 0);
	xSemaphoreTake(GPSSetSemHandle, 0);
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

  /* definition and creation of GPSUnboxingTask */
  osThreadDef(GPSUnboxingTask, GPSUnboxingFunc, osPriorityNormal, 0, 512);
  GPSUnboxingTaskHandle = osThreadCreate(osThread(GPSUnboxingTask), NULL);

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
		extern uint8_t hour, minuts, second, day, month, year, fix;

  /* Infinite loop */
  for(;;)
  {
		HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
		fix = GPS_is_fix_valid();
		GPS_get_date(&day, &month, &year);
		GPS_get_time(&hour, &minuts, &second);
    osDelay(250);
  }
  /* USER CODE END aliveTask */
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
		HAL_UART_Receive_DMA(&huart2,&GPSFirstChar, 1);		/*first time program is executed, uart reception is started by this command*/
  /* Infinite loop */
  for(;;)
  {
		xSemaphoreTake(GPSUnboxSemHandle, portMAX_DELAY);	/*if buffer has been filled entirly*/
		GPS_data_conversion(GPSRawBuffer);											/*call conversion function*/
  }
  /* USER CODE END GPSUnboxingFunc */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
