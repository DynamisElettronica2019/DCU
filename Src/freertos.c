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
extern BNO085 myIMU;
extern TIM_HandleTypeDef htim7;
BaseType_t IMUHigherPriorityTaskWoken = pdFALSE;

uint8_t stat = 1; /*only for debug*/
int addr = 0;


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
  xSemaphoreTake(IMUSemHandle, portMAX_DELAY);
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

  /* definition and creation of IMUUnboxTask */
  osThreadDef(IMUUnboxTask, IMUUnboxFunc, osPriorityIdle, 0, 512);
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
  /* Infinite loop */
  for(;;) {
    HAL_GPIO_TogglePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin);
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
	while (stat != 0){
		stat = HAL_I2C_IsDeviceReady(&hi2c4,addr, 10, 1000); /*only for debug*/
		addr++;
		HAL_Delay(50);
	}
	BNO085_init();
	HAL_TIM_Base_Start_IT(&htim7);
  /* Infinite loop */
  for(;;)
  {
		xSemaphoreTake(IMUSemHandle, portMAX_DELAY);	/*tries to take semaphore*/
		BNO085_UpdateSensorReading(&myIMU);						/*should fill myIMU fileds with new data, right?*/
    osDelay(1);
  }
  /* USER CODE END IMUUnboxFunc */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
