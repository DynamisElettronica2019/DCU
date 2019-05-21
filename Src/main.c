/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "cmsis_os.h"
#include "adc.h"
#include "can.h"
#include "dma.h"
#include "fatfs.h"
#include "i2c.h"
#include "rtc.h"
#include "sdmmc.h"
#include "tim.h"
#include "usart.h"
#include "usb_host.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "data.h"
#include "telemetry.h"
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

/* USER CODE BEGIN PV */
uint8_t startAcquisitionCommand = ACQUISITION_IDLE_REQUEST;
uint8_t timer8HzCounter = 1;
BaseType_t GPIO_AutogearHigherPriorityTaskWoken = pdFALSE;
BaseType_t GPIO_OvercurrentHigherPriorityTaskWoken = pdFALSE;
BaseType_t GPIO1_CtrlxHigherPriorityTaskWoken = pdFALSE;
BaseType_t GPIO2_CtrlxHigherPriorityTaskWoken = pdFALSE;
BaseType_t GPIO3_CtrlxHigherPriorityTaskWoken = pdFALSE;
BaseType_t alive_xHigherPriorityTaskWoken = pdFALSE;
BaseType_t USB_xHigherPriorityTaskWoken = pdFALSE;
BaseType_t CAN_xHigherPriorityTaskWoken = pdFALSE;
extern uint8_t telemetryReceivedBuffer [BUFFER_COMMAND_LEN];
extern uint32_t adc1BufferRaw [ADC1_RAW_DATA_LEN];					
extern uint32_t adc2BufferRaw [ADC2_RAW_DATA_LEN];
extern osThreadId aliveHandle;
extern osSemaphoreId saveUsbSemaphoreHandle;
extern osMessageQId startAcquisitionEventHandle;
extern osMessageQId digitalAuxQueueHandle;
extern osSemaphoreId autogearSemaphoreHandle;
extern osSemaphoreId USB_OvercurrentSemaphoreHandle;
extern osSemaphoreId CAN_SendSemaphoreHandle;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_CAN1_Init();
  MX_I2C4_Init();
  MX_RTC_Init();
  MX_SDMMC1_SD_Init();
  MX_TIM5_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
	HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET); 			/* Green LED off as default */
	HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_RESET); 		/* Yellow LED off as default */
	HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET); 					/* Red LED off as default */
	dataPacketReset();																													/* Reset the data saving buffer */
	packetCounterReset();																												/* Reset the CAN packets recevide counter */
	canStart();																																	/* CAN filter config and start */
	adcBuffersInit();																														/* ADC buffer initialization */
	usbInitStart();																															/* USB peripheral config and start */
	MX_FATFS_Init();																														/* FatFS init */
	HAL_Delay(500);																															/* Delay for Vbus stabilization */
	HAL_TIM_Base_Start_IT(&htim5); 																							/* Start timer 5 in interrupt mode */
	HAL_TIM_Base_Start_IT(&htim6); 																							/* Start timer 6 in interrupt mode */
	HAL_TIM_Base_Start_IT(&htim7); 																							/* Start timer 7 in interrupt mode */
  /* USER CODE END 2 */

  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();

  /* Start scheduler */
  osKernelStart();
  
  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 6;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode 
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_USART1
                              |RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_I2C4
                              |RCC_PERIPHCLK_SDMMC1|RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 192;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 2;
  PeriphClkInitStruct.PLLSAI.PLLSAIQ = 2;
  PeriphClkInitStruct.PLLSAI.PLLSAIP = RCC_PLLSAIP_DIV8;
  PeriphClkInitStruct.PLLSAIDivQ = 1;
  PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_2;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  PeriphClkInitStruct.Usart1ClockSelection = RCC_USART1CLKSOURCE_SYSCLK;
  PeriphClkInitStruct.Usart2ClockSelection = RCC_USART2CLKSOURCE_SYSCLK;
  PeriphClkInitStruct.I2c4ClockSelection = RCC_I2C4CLKSOURCE_SYSCLK;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLLSAIP;
  PeriphClkInitStruct.Sdmmc1ClockSelection = RCC_SDMMC1CLKSOURCE_CLK48;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1) {
		usart1RxCallback();
	}
	
	if(huart->Instance == USART2) {
		/* Put here the GPS code */
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1) {
		usart1TxCallback();
	}
	
	if(huart->Instance==USART2) {
		/* Put here the GPS code */
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

	/* USB overcurrent ISR */
	if(HAL_GPIO_ReadPin(USB_OVERCURRENT_GPIO_Port, USB_OVERCURRENT_Pin) == GPIO_PIN_RESET) {
		if(USB_OvercurrentSemaphoreHandle != NULL) {																													/* Check on system start if semaphore is already created */
			xSemaphoreGiveFromISR(USB_OvercurrentSemaphoreHandle, &GPIO_OvercurrentHigherPriorityTaskWoken); 		/* Give semaphore to task when interrupt is called */
			portYIELD_FROM_ISR(GPIO_OvercurrentHigherPriorityTaskWoken);																				/* Do context-switch if needed */
		}
	}
	
	/* Autogear switch ISR */
	if(HAL_GPIO_ReadPin(AUTOGEAR_SWTICH_MCU_GPIO_Port, AUTOGEAR_SWTICH_MCU_Pin) == GPIO_PIN_RESET) {
		if(autogearSemaphoreHandle != NULL) {																																	/* Check on system start if semaphore is already created */
			xSemaphoreGiveFromISR(autogearSemaphoreHandle, &GPIO_AutogearHigherPriorityTaskWoken); 							/* Give semaphore to task when interrupt is called */
			portYIELD_FROM_ISR(GPIO_AutogearHigherPriorityTaskWoken);																						/* Do context-switch if needed */
		}
	}
}

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */
	uint8_t digitalAuxEvent = 0;
	
  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */
	
	/* Timer 5 period elapsed callback: 1 Hz */
	if (htim->Instance == TIM5) {

		/* CAN send at 1 Hz: temperature, currents, voltages and acquisition status */
		if(CAN_SendSemaphoreHandle != NULL) {
			xSemaphoreGiveFromISR(CAN_SendSemaphoreHandle, &CAN_xHigherPriorityTaskWoken);
			portYIELD_FROM_ISR(CAN_xHigherPriorityTaskWoken);
		}
		
		/* Telemetry DCU state packet send at 1 Hz */
    stateSendTimCallback();
		
		/* ADCs management: 1 Hz sampling time */
		HAL_ADC_Start_DMA(&hadc1,adc1BufferRaw, ADC1_NUMBER_OF_CHANNELS);			/* Start ADC 1 in DMA mode */
		HAL_ADC_Start_DMA(&hadc2,adc2BufferRaw, ADC2_NUMBER_OF_CHANNELS ); 		/* Start ADC 2 in DMA mode */
	
		/* Digital aux management: 1 Hz sampling time */
		if(HAL_GPIO_ReadPin(DIGITAL_AUX_1_GPIO_Port, DIGITAL_AUX_1_Pin) == GPIO_PIN_SET) {
			digitalAuxEvent = DIGITAL_EVENT_AUX_1;
			xQueueSendFromISR(digitalAuxQueueHandle, &digitalAuxEvent, &GPIO1_CtrlxHigherPriorityTaskWoken);		/* Add digital aux event to queue */
		}
		
		if(HAL_GPIO_ReadPin(DIGITAL_AUX_2_GPIO_Port, DIGITAL_AUX_2_Pin) == GPIO_PIN_SET) {
			digitalAuxEvent = DIGITAL_EVENT_AUX_2;
			xQueueSendFromISR(digitalAuxQueueHandle, &digitalAuxEvent, &GPIO1_CtrlxHigherPriorityTaskWoken);		/* Add digital aux event to queue */
		}
		
		if(HAL_GPIO_ReadPin(DIGITAL_AUX_3_GPIO_Port, DIGITAL_AUX_3_Pin) == GPIO_PIN_SET) {
			digitalAuxEvent = DIGITAL_EVENT_AUX_3;
			xQueueSendFromISR(digitalAuxQueueHandle, &digitalAuxEvent, &GPIO1_CtrlxHigherPriorityTaskWoken);		/* Add digital aux event to queue */
		}
	}

	/* Timer 6 period elapsed callback: 10 Hz */
	if(htim->Instance == TIM6) {
	}

	/* Timer 7 period elapsed callback: 100 Hz */
	if(htim->Instance == TIM7) {
		
		/* USB saving task */
		if(saveUsbSemaphoreHandle != NULL) {
			xSemaphoreGiveFromISR(saveUsbSemaphoreHandle, &USB_xHigherPriorityTaskWoken);
			portYIELD_FROM_ISR(USB_xHigherPriorityTaskWoken);
		}

    /* Send telemetry data */
    if(timer8HzCounter >= 12) {
      dataSendTimCallback();
      timer8HzCounter = 1;
    } else {
      timer8HzCounter++;
    }
	}
  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
	HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET); 			/* Switch-off green LED */
	HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_RESET); 		/* Switch-off yellow LED */
	HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET); 					/* Switch-off ed LED off */
	
	/* Red LED toogle to report error */
  while(1) {
		HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
		HAL_Delay(100);
	}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET); 			/* Switch-off green LED */
	HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_RESET); 		/* Switch-off yellow LED */
	HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET); 					/* Switch-off ed LED off */
	
	/* Red LED toogle to report erorrs */
	while(1) {
		HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
		HAL_Delay(250);
	}
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
