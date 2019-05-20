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
#include "gpio.h"
#include "adc.h"
#include "data_conversion.h"
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
extern uint32_t adc1BufferRaw [ADC1_RAW_DATA_LEN];					
extern uint32_t adc2BufferRaw [ADC2_RAW_DATA_LEN];
extern float adcBufferConvertedDebug [ADC_CONVERTED_DEBUG_DATA_LEN];
extern float adcBufferConvertedAux [ADC_CONVERTED_AUX_DATA_LEN];
/* USER CODE END Variables */
osThreadId aliveHandle;
osThreadId adc1ConversionHandle;
osThreadId adc2ConversionHandle;
osThreadId digitalAuxManagerHandle;
osThreadId autogearManagerHandle;
osThreadId USB_OvercurrentManagerHandle;
osMessageQId digitalAuxQueueHandle;
osSemaphoreId adc1SemaphoreHandle;
osSemaphoreId adc2SemaphoreHandle;
osSemaphoreId autogearSemaphoreHandle;
osSemaphoreId USB_OvercurrentSemaphoreHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void aliveTask(void const * argument);
void adc1ConversionTask(void const * argument);
void adc2ConversionTask(void const * argument);
void digitalAuxManagerTask(void const * argument);
void autogearManagerTask(void const * argument);
void USB_OvercurrentManagerTask(void const * argument);

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
  /* definition and creation of adc1Semaphore */
  osSemaphoreDef(adc1Semaphore);
  adc1SemaphoreHandle = osSemaphoreCreate(osSemaphore(adc1Semaphore), 1);

  /* definition and creation of adc2Semaphore */
  osSemaphoreDef(adc2Semaphore);
  adc2SemaphoreHandle = osSemaphoreCreate(osSemaphore(adc2Semaphore), 1);

  /* definition and creation of autogearSemaphore */
  osSemaphoreDef(autogearSemaphore);
  autogearSemaphoreHandle = osSemaphoreCreate(osSemaphore(autogearSemaphore), 1);

  /* definition and creation of USB_OvercurrentSemaphore */
  osSemaphoreDef(USB_OvercurrentSemaphore);
  USB_OvercurrentSemaphoreHandle = osSemaphoreCreate(osSemaphore(USB_OvercurrentSemaphore), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
	xSemaphoreTake(adc1SemaphoreHandle, portMAX_DELAY);									/* Start with the task locked */
	xSemaphoreTake(adc2SemaphoreHandle, portMAX_DELAY); 								/* Start with the task locked */
	xSemaphoreTake(autogearSemaphoreHandle, portMAX_DELAY); 						/* Start with the task locked */
	xSemaphoreTake(USB_OvercurrentSemaphoreHandle, portMAX_DELAY); 			/* Start with the task locked */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of digitalAuxQueue */
  osMessageQDef(digitalAuxQueue, 8, uint8_t);
  digitalAuxQueueHandle = osMessageCreate(osMessageQ(digitalAuxQueue), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of alive */
  osThreadDef(alive, aliveTask, osPriorityLow, 0, 128);
  aliveHandle = osThreadCreate(osThread(alive), NULL);

  /* definition and creation of adc1Conversion */
  osThreadDef(adc1Conversion, adc1ConversionTask, osPriorityLow, 0, 128);
  adc1ConversionHandle = osThreadCreate(osThread(adc1Conversion), NULL);

  /* definition and creation of adc2Conversion */
  osThreadDef(adc2Conversion, adc2ConversionTask, osPriorityLow, 0, 128);
  adc2ConversionHandle = osThreadCreate(osThread(adc2Conversion), NULL);

  /* definition and creation of digitalAuxManager */
  osThreadDef(digitalAuxManager, digitalAuxManagerTask, osPriorityLow, 0, 128);
  digitalAuxManagerHandle = osThreadCreate(osThread(digitalAuxManager), NULL);

  /* definition and creation of autogearManager */
  osThreadDef(autogearManager, autogearManagerTask, osPriorityNormal, 0, 128);
  autogearManagerHandle = osThreadCreate(osThread(autogearManager), NULL);

  /* definition and creation of USB_OvercurrentManager */
  osThreadDef(USB_OvercurrentManager, USB_OvercurrentManagerTask, osPriorityNormal, 0, 128);
  USB_OvercurrentManagerHandle = osThreadCreate(osThread(USB_OvercurrentManager), NULL);

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

/* USER CODE BEGIN Header_adc1ConversionTask */
/**
* @brief Function implementing the adc1Conversion thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_adc1ConversionTask */
void adc1ConversionTask(void const * argument)
{
  /* USER CODE BEGIN adc1ConversionTask */
  /* Infinite loop */
  for(;;) {
		xSemaphoreTake(adc1SemaphoreHandle, portMAX_DELAY); 		/* Unlock when DMA callback is called */
		HAL_ADC_Stop_DMA(&hadc1);																/* Stop ADC1 conversion and wait for timer to restrt it */
		adcBufferConvertedDebug[DCU_TEMP_SENSE_POSITION] = dcuTempSenseConversion(adc1BufferRaw[DCU_TEMP_SENSE_POSITION]);
		adcBufferConvertedDebug[MAIN_CURRENT_SENSE_POSITION] = mainCurrentSenseConversion(adc1BufferRaw[MAIN_CURRENT_SENSE_POSITION]);
		adcBufferConvertedDebug[DCU_CURRENT_SENSE_POSITION] = dcuCurrentSenseConversion(adc1BufferRaw[DCU_CURRENT_SENSE_POSITION]);
		adcBufferConvertedDebug[_5V_DCU_POSITION] = _5vSenseConversion(adc1BufferRaw[_5V_DCU_POSITION]);
		adcBufferConvertedDebug[_12V_POST_DIODES_SENSE_POSITION] = _12vSenseConversion(adc1BufferRaw[_12V_POST_DIODES_SENSE_POSITION]);
		adcBufferConvertedDebug[_3V3_MCU_POSITION] = _3v3SenseConversion(adc1BufferRaw[_3V3_MCU_POSITION]);
		adcBufferConvertedDebug[XBEE_CURRENT_SENSE_POSITION] = xbeeCurrentSenseConversion(adc1BufferRaw[XBEE_CURRENT_SENSE_POSITION]);
		adcBufferConvertedDebug[VBAT_CHANNEL_POSITION] = vbatConversion(adc1BufferRaw[VBAT_CHANNEL_POSITION]);
		/* TO BE IMPLEMENTED */
		/* Put here the CSV buffer conversion functions */
  }
  /* USER CODE END adc1ConversionTask */
}

/* USER CODE BEGIN Header_adc2ConversionTask */
/**
* @brief Function implementing the adc2Conversion thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_adc2ConversionTask */
void adc2ConversionTask(void const * argument)
{
  /* USER CODE BEGIN adc2ConversionTask */
  /* Infinite loop */
  for(;;) {
		xSemaphoreTake(adc2SemaphoreHandle, portMAX_DELAY); 		/* Unlock when DMA callback is called */
		HAL_ADC_Stop_DMA(&hadc2);																/* Stop ADC2 conversion and wait for timer to restrt it */
    adcBufferConvertedAux[ANALOG_AUX_1_POSITION] = analogAux1Conversion(adc2BufferRaw[ANALOG_AUX_1_RAW_POSITION]);
		adcBufferConvertedAux[ANALOG_AUX_2_POSITION] = analogAux2Conversion(adc2BufferRaw[ANALOG_AUX_2_RAW_POSITION]);
		adcBufferConvertedAux[ANALOG_AUX_3_POSITION] = analogAux3Conversion(adc2BufferRaw[ANALOG_AUX_3_RAW_POSITION]);
		/* TO BE IMPLEMENTED */
		/* Put here the CSV buffer conversion functions */
  }
  /* USER CODE END adc2ConversionTask */
}

/* USER CODE BEGIN Header_digitalAuxManagerTask */
/**
* @brief Function implementing the digitalAuxManager thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_digitalAuxManagerTask */
void digitalAuxManagerTask(void const * argument)
{
  /* USER CODE BEGIN digitalAuxManagerTask */
	uint8_t digitalAuxEvent = 0;
	
  /* Infinite loop */
  for(;;) {
		
		/* Wait for an event and get the identifier from the queue */
		if(xQueueReceive(digitalAuxQueueHandle, &digitalAuxEvent, portMAX_DELAY) == pdTRUE) {
			switch(digitalAuxEvent) {
				case DIGITAL_EVENT_AUX_1:
					/* Put here the code to handling digital aux 1 event */
					break;
				
				case DIGITAL_EVENT_AUX_2:
					/* Put here the code to handling digital aux 1 event */
					break;
				
				case DIGITAL_EVENT_AUX_3:
					/* Put here the code to handling digital aux 1 event */
					break;
			}
		}
  }
  /* USER CODE END digitalAuxManagerTask */
}

/* USER CODE BEGIN Header_autogearManagerTask */
/**
* @brief Function implementing the autogearManager thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_autogearManagerTask */
void autogearManagerTask(void const * argument)
{
  /* USER CODE BEGIN autogearManagerTask */
  /* Infinite loop */
  for(;;) {
    xSemaphoreTake(autogearSemaphoreHandle, portMAX_DELAY); 			/* Unlock when interrupt callback is called */
		osDelay(15);																									/* Wait for fail trigger detection */
		
		if(HAL_GPIO_ReadPin(AUTOGEAR_SWTICH_MCU_GPIO_Port, AUTOGEAR_SWTICH_MCU_Pin) == GPIO_PIN_RESET) {
			/* Put here the code to handling autogear event */
		}
  }
  /* USER CODE END autogearManagerTask */
}

/* USER CODE BEGIN Header_USB_OvercurrentManagerTask */
/**
* @brief Function implementing the USB_OvercurrentManager thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_USB_OvercurrentManagerTask */
void USB_OvercurrentManagerTask(void const * argument)
{
  /* USER CODE BEGIN USB_OvercurrentManagerTask */
  /* Infinite loop */
  for(;;) {
		xSemaphoreTake(autogearSemaphoreHandle, portMAX_DELAY); 		/* Unlock when interrupt callback is called */
		osDelay(1);																									/* Wait for fail trigger detection */
		
		if(HAL_GPIO_ReadPin(USB_OVERCURRENT_GPIO_Port, USB_OVERCURRENT_Pin) == GPIO_PIN_RESET) {
			/* Put here the code to handling autogear event */
		}
  }
  /* USER CODE END USB_OvercurrentManagerTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
