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
UINT bytesWritten;
extern uint8_t acquisitionOn;
extern USBH_HandleTypeDef hUsbHostFS;
extern uint8_t blockBuffer[BUFFER_BLOCK_LEN];
extern uint8_t dcuStateBuffer[BUFFER_STATE_LEN];
/* USER CODE END Variables */
osThreadId aliveHandle;
osThreadId saveUsbHandle;
osThreadId usbManagerHandle;
osThreadId startAcquisitionStateMachineHandle;
osThreadId canFifo0UnpackHandle;
osThreadId canFifo1UnpackHandle;
osMessageQId usbEventQueueHandle;
osMessageQId startAcquisitionEventHandle;
osMessageQId canFifo0QueueHandle;
osMessageQId canFifo1QueueHandle;
osSemaphoreId saveUsbSemaphoreHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void aliveTask(void const * argument);
void saveUsbTask(void const * argument);
void usbManageTask(void const * argument);
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
  /* definition and creation of saveUsbSemaphore */
  osSemaphoreDef(saveUsbSemaphore);
  saveUsbSemaphoreHandle = osSemaphoreCreate(osSemaphore(saveUsbSemaphore), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
	xSemaphoreTake(saveUsbSemaphoreHandle, portMAX_DELAY);				/* Start with the task locked */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
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
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of alive */
  osThreadDef(alive, aliveTask, osPriorityLow, 0, 128);
  aliveHandle = osThreadCreate(osThread(alive), NULL);

  /* definition and creation of saveUsb */
  osThreadDef(saveUsb, saveUsbTask, osPriorityHigh, 0, 512);
  saveUsbHandle = osThreadCreate(osThread(saveUsb), NULL);

  /* definition and creation of usbManager */
  osThreadDef(usbManager, usbManageTask, osPriorityNormal, 0, 512);
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
		vTaskDelay(250 / portTICK_PERIOD_MS);
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
		startAcquisitionStateMachine(startAcquisitionEvent);
		
		/* Start acquisition state machine, called at 100 Hz */
		/* NOTE: Events are coded using chars NOT numbers! */
		if(xQueueReceive(startAcquisitionEventHandle, &startAcquisitionEvent, 10 / portTICK_PERIOD_MS) != pdTRUE) {
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
