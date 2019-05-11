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
#include "usb_host.h"
#include "fatfs.h"
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
uint8_t linkResult;
UINT bytesWritten;
FRESULT mountResult;
FRESULT writeResult;
FRESULT openResult;
FRESULT closeResult;
extern USBH_HandleTypeDef hUsbHostFS;
extern uint8_t blockBuffer[BUFFER_BLOCK_LEN];
/* USER CODE END Variables */
osThreadId aliveHandle;
osThreadId saveUsbHandle;
osThreadId usbManagerHandle;
osMessageQId usbEventQueueHandle;
osSemaphoreId saveUsbSemaphoreHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void aliveTask(void const * argument);
void saveUsbTask(void const * argument);
void usbManageTask(void const * argument);

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
  /* definition and creation of saveUsbSemaphore */
  osSemaphoreDef(saveUsbSemaphore);
  saveUsbSemaphoreHandle = osSemaphoreCreate(osSemaphore(saveUsbSemaphore), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */
	xSemaphoreTake(saveUsbHandle, portMAX_DELAY);			/* Start with the USB saving task locked */
  
	/* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of usbEventQueue */
  osMessageQDef(usbEventQueue, 8, uint8_t);
  usbEventQueueHandle = osMessageCreate(osMessageQ(usbEventQueue), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of alive */
  osThreadDef(alive, aliveTask, osPriorityLow, 0, 128);
  aliveHandle = osThreadCreate(osThread(alive), NULL);

  /* definition and creation of saveUsb */
  osThreadDef(saveUsb, saveUsbTask, osPriorityHigh, 0, 512);
  saveUsbHandle = osThreadCreate(osThread(saveUsb), NULL);

  /* definition and creation of usbManager */
  osThreadDef(usbManager, usbManageTask, osPriorityLow, 0, 128);
  usbManagerHandle = osThreadCreate(osThread(usbManager), NULL);

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
    HAL_GPIO_TogglePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin);
		vTaskDelay(500 / portTICK_PERIOD_MS);
  }
  /* USER CODE END aliveTask */
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
	dataBufferInit();
	
  /* Infinite loop */
  for(;;) {
		xSemaphoreTake(saveUsbHandle, portMAX_DELAY);		/* Unlock when timer callback is called */
		
		/* Put here the saving code */
		
		/* Testing code */
		openResult = f_open(&USBHFile, "DynamisPRC_Polimi.txt", FA_CREATE_ALWAYS | FA_WRITE);
		HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
		writeResult = f_write(&USBHFile, blockBuffer, BUFFER_BLOCK_LEN, (void *)&bytesWritten);
		closeResult = f_close(&USBHFile);
		HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
  }
  /* USER CODE END saveUsbTask */
}

/* USER CODE BEGIN Header_usbManageTask */
/**
* @brief Function implementing the usbManager thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_usbManageTask */
void usbManageTask(void const * argument)
{
  /* USER CODE BEGIN usbManageTask */
	osEvent usbEvent;
	
  /* Infinite loop */
  for(;;) {
		usbEvent = osMessageGet(usbEventQueueHandle, osWaitForever);
		
		if(usbEvent.status == osEventMessage) {
			switch(usbEvent.value.v) {
				case DISCONNECTION_EVENT:
					mountResult = f_mount(NULL, (TCHAR const *)"", 1);
					linkResult = FATFS_UnLinkDriver(USBHPath);
					/* Update here the status packet */
					break;

				case CONNECTED_EVENT:
					if(FATFS_LinkDriver(&USBH_Driver, USBHPath) == 0) {
						mountResult = f_mount(&USBHFatFS, (TCHAR const *)USBHPath, 1);
					}
					/* Update here the status packet */
					
					break;
	    
				default:
					break;
			}
		}
	}
  /* USER CODE END usbManageTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
