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

#include "tim.h"
#include "BNO085.h"

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

uint8_t buffer [6];
	uint8_t received [6];
	uint8_t k = 0;
	uint8_t add = 0;
	
	uint8_t tx = 255;
	uint8_t rx = 255;
	
extern BNO085 myIMU;
/* USER CODE END Variables */
osThreadId aliveHandle;
osThreadId IMUUnboxTaskHandle;
osSemaphoreId IMUSemHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void aliveTask(void const * argument);
void IMUUnboxFunc(void const * argument);

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
  /* definition and creation of IMUSem */
  osSemaphoreDef(IMUSem);
  IMUSemHandle = osSemaphoreCreate(osSemaphore(IMUSem), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
	
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
	xSemaphoreTake(IMUSemHandle, portMAX_DELAY);     /* Start with the task locked */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of alive */
  osThreadDef(alive, aliveTask, osPriorityLow, 0, 128);
  aliveHandle = osThreadCreate(osThread(alive), NULL);

  /* definition and creation of IMUUnboxTask */
  osThreadDef(IMUUnboxTask, IMUUnboxFunc, osPriorityNormal, 0, 2048);
  IMUUnboxTaskHandle = osThreadCreate(osThread(IMUUnboxTask), NULL);

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
	HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
	HAL_TIM_Base_Start_IT(&htim5);
	HAL_TIM_Base_Start_IT(&htim6);
	HAL_TIM_Base_Start_IT(&htim7);
	
  /* Infinite loop */
  for(;;) {
    HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
		osDelay(250);
  }
  /* USER CODE END aliveTask */
}

/* USER CODE BEGIN Header_IMUUnboxFunc */
/**
* @brief Function implementing the IMUUnboxTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_IMUUnboxFunc */
void IMUUnboxFunc(void const * argument)
{
  /* USER CODE BEGIN IMUUnboxFunc */
	
		//BNO085_init();
	
		HAL_GPIO_WritePin(myIMU.reset_GPIOx, myIMU.reset_Pin, GPIO_PIN_SET);
		osDelay(250);
		HAL_GPIO_WritePin(myIMU.reset_GPIOx, myIMU.reset_Pin, GPIO_PIN_RESET);
		osDelay(500);
		HAL_GPIO_WritePin(myIMU.reset_GPIOx, myIMU.reset_Pin, GPIO_PIN_SET);
		osDelay(500);
	
	
	while(1) {
		buffer[0] = 6;
		buffer[1] = 0;
		buffer[2] = CHANNEL_CONTROL;
		buffer[3] = k;
		k++;
		buffer[4] = SHTP_PRODUCT_ID_REQUEST;
		buffer[5] = 0;
		tx = HAL_I2C_Master_Transmit(&hi2c4, 0x96, buffer, 6, 1000);
		rx = HAL_I2C_Master_Receive(&hi2c4, 0x96, received, 6, 1000);
		osDelay(50);
	}
	
	
	
	
//	
//	myIMU->BNO085_Send_Buffer[0]=6;
//	myIMU->BNO085_Send_Buffer[1]=0;
//	myIMU->BNO085_Send_Buffer[2]=CHANNEL_CONTROL;
//	myIMU->BNO085_Send_Buffer[3]=myIMU->sequenceNumber[CHANNEL_CONTROL];
//	myIMU->sequenceNumber[CHANNEL_CONTROL]++;
//	
//	myIMU->BNO085_Send_Buffer[4]= SHTP_PRODUCT_ID_REQUEST;
//	myIMU->BNO085_Send_Buffer[5]= 0;
//	
//	HAL_I2C_Master_Transmit(myIMU->hi2cx, myIMU->address, myIMU->BNO085_Send_Buffer,6, 1000);
	
	
  /* Infinite loop */
//  for(;;) {
////		xSemaphoreTake(IMUSemHandle, portMAX_DELAY);		/* Try to take semaphore */
////		BNO085_UpdateSensorReading();							/* Should fill myIMU struct with new data */
////		osDelay(1);
//  }
  /* USER CODE END IMUUnboxFunc */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
