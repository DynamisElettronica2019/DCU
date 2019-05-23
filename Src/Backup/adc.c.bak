/**
  ******************************************************************************
  * File Name          : ADC.c
  * Description        : This file provides code for the configuration
  *                      of the ADC instances.
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

/* Includes ------------------------------------------------------------------*/
#include "adc.h"

/* USER CODE BEGIN 0 */
#include "cmsis_os.h"
#include "data.h"
#include "data_conversion.h"
#include "string_utility.h"

BaseType_t ADC1_xHigherPriorityTaskWoken = pdFALSE;
BaseType_t ADC2_xHigherPriorityTaskWoken = pdFALSE;
uint32_t ADC1_BufferRaw [ADC1_RAW_DATA_LEN];					
uint32_t ADC2_BufferRaw [ADC2_RAW_DATA_LEN];
float _5vTemp = 0.0f;
float _12vTemp = 0.0f;
float _3v3Temp = 0.0f;
float ADC_BufferConvertedDebug [ADC_CONVERTED_DEBUG_DATA_LEN];
float ADC_BufferConvertedAux [ADC_CONVERTED_AUX_DATA_LEN];
extern uint8_t DATA_BlockBuffer [BUFFER_BLOCK_LEN];
extern osSemaphoreId adc1SemaphoreHandle;
extern osSemaphoreId adc2SemaphoreHandle;
/* USER CODE END 0 */

ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;
DMA_HandleTypeDef hdma_adc1;
DMA_HandleTypeDef hdma_adc2;

/* ADC1 init function */
void MX_ADC1_Init(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 8;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = ADC_REGULAR_RANK_3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_9;
  sConfig.Rank = ADC_REGULAR_RANK_4;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_10;
  sConfig.Rank = ADC_REGULAR_RANK_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_12;
  sConfig.Rank = ADC_REGULAR_RANK_6;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_13;
  sConfig.Rank = ADC_REGULAR_RANK_7;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_VBAT;
  sConfig.Rank = ADC_REGULAR_RANK_8;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

}
/* ADC2 init function */
void MX_ADC2_Init(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
  */
  hadc2.Instance = ADC2;
  hadc2.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc2.Init.Resolution = ADC_RESOLUTION_12B;
  hadc2.Init.ScanConvMode = ENABLE;
  hadc2.Init.ContinuousConvMode = DISABLE;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.NbrOfConversion = 3;
  hadc2.Init.DMAContinuousRequests = ENABLE;
  hadc2.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  if (HAL_ADC_Init(&hadc2) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_6;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = ADC_REGULAR_RANK_3;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspInit 0 */

  /* USER CODE END ADC1_MspInit 0 */
    /* ADC1 clock enable */
    __HAL_RCC_ADC1_CLK_ENABLE();
  
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**ADC1 GPIO Configuration    
    PC0     ------> ADC1_IN10
    PC2     ------> ADC1_IN12
    PC3     ------> ADC1_IN13
    PA0/WKUP     ------> ADC1_IN0
    PA3     ------> ADC1_IN3
    PA4     ------> ADC1_IN4
    PB1     ------> ADC1_IN9 
    */
    GPIO_InitStruct.Pin = _12V_POST_DIODES_SENSE_Pin|_3V3_REG_SENSE_Pin|XBEE_CURRENT_SENSE_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = DCU_TEMP_SENSE_Pin|MAIN_CURRENT_SENSE_Pin|DCU_CURRENT_SENSE_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = _5V_REG_SENSE_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(_5V_REG_SENSE_GPIO_Port, &GPIO_InitStruct);

    /* ADC1 DMA Init */
    /* ADC1 Init */
    hdma_adc1.Instance = DMA2_Stream0;
    hdma_adc1.Init.Channel = DMA_CHANNEL_0;
    hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_adc1.Init.Mode = DMA_CIRCULAR;
    hdma_adc1.Init.Priority = DMA_PRIORITY_LOW;
    hdma_adc1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_adc1) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(adcHandle,DMA_Handle,hdma_adc1);

    /* ADC1 interrupt Init */
    HAL_NVIC_SetPriority(ADC_IRQn, 11, 0);
    HAL_NVIC_EnableIRQ(ADC_IRQn);
  /* USER CODE BEGIN ADC1_MspInit 1 */

  /* USER CODE END ADC1_MspInit 1 */
  }
  else if(adcHandle->Instance==ADC2)
  {
  /* USER CODE BEGIN ADC2_MspInit 0 */

  /* USER CODE END ADC2_MspInit 0 */
    /* ADC2 clock enable */
    __HAL_RCC_ADC2_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**ADC2 GPIO Configuration    
    PA5     ------> ADC2_IN5
    PA6     ------> ADC2_IN6
    PB0     ------> ADC2_IN8 
    */
    GPIO_InitStruct.Pin = ANALOG_AUX_3_Pin|ANALOG_AUX_2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = ANALOG_AUX_1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(ANALOG_AUX_1_GPIO_Port, &GPIO_InitStruct);

    /* ADC2 DMA Init */
    /* ADC2 Init */
    hdma_adc2.Instance = DMA2_Stream2;
    hdma_adc2.Init.Channel = DMA_CHANNEL_1;
    hdma_adc2.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc2.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc2.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc2.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_adc2.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_adc2.Init.Mode = DMA_CIRCULAR;
    hdma_adc2.Init.Priority = DMA_PRIORITY_LOW;
    hdma_adc2.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_adc2) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(adcHandle,DMA_Handle,hdma_adc2);

    /* ADC2 interrupt Init */
    HAL_NVIC_SetPriority(ADC_IRQn, 11, 0);
    HAL_NVIC_EnableIRQ(ADC_IRQn);
  /* USER CODE BEGIN ADC2_MspInit 1 */

  /* USER CODE END ADC2_MspInit 1 */
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspDeInit 0 */

  /* USER CODE END ADC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC1_CLK_DISABLE();
  
    /**ADC1 GPIO Configuration    
    PC0     ------> ADC1_IN10
    PC2     ------> ADC1_IN12
    PC3     ------> ADC1_IN13
    PA0/WKUP     ------> ADC1_IN0
    PA3     ------> ADC1_IN3
    PA4     ------> ADC1_IN4
    PB1     ------> ADC1_IN9 
    */
    HAL_GPIO_DeInit(GPIOC, _12V_POST_DIODES_SENSE_Pin|_3V3_REG_SENSE_Pin|XBEE_CURRENT_SENSE_Pin);

    HAL_GPIO_DeInit(GPIOA, DCU_TEMP_SENSE_Pin|MAIN_CURRENT_SENSE_Pin|DCU_CURRENT_SENSE_Pin);

    HAL_GPIO_DeInit(_5V_REG_SENSE_GPIO_Port, _5V_REG_SENSE_Pin);

    /* ADC1 DMA DeInit */
    HAL_DMA_DeInit(adcHandle->DMA_Handle);

    /* ADC1 interrupt Deinit */
  /* USER CODE BEGIN ADC1:ADC_IRQn disable */
    /**
    * Uncomment the line below to disable the "ADC_IRQn" interrupt
    * Be aware, disabling shared interrupt may affect other IPs
    */
    /* HAL_NVIC_DisableIRQ(ADC_IRQn); */
  /* USER CODE END ADC1:ADC_IRQn disable */

  /* USER CODE BEGIN ADC1_MspDeInit 1 */

  /* USER CODE END ADC1_MspDeInit 1 */
  }
  else if(adcHandle->Instance==ADC2)
  {
  /* USER CODE BEGIN ADC2_MspDeInit 0 */

  /* USER CODE END ADC2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC2_CLK_DISABLE();
  
    /**ADC2 GPIO Configuration    
    PA5     ------> ADC2_IN5
    PA6     ------> ADC2_IN6
    PB0     ------> ADC2_IN8 
    */
    HAL_GPIO_DeInit(GPIOA, ANALOG_AUX_3_Pin|ANALOG_AUX_2_Pin);

    HAL_GPIO_DeInit(ANALOG_AUX_1_GPIO_Port, ANALOG_AUX_1_Pin);

    /* ADC2 DMA DeInit */
    HAL_DMA_DeInit(adcHandle->DMA_Handle);

    /* ADC2 interrupt Deinit */
  /* USER CODE BEGIN ADC2:ADC_IRQn disable */
    /**
    * Uncomment the line below to disable the "ADC_IRQn" interrupt
    * Be aware, disabling shared interrupt may affect other IPs
    */
    /* HAL_NVIC_DisableIRQ(ADC_IRQn); */
  /* USER CODE END ADC2:ADC_IRQn disable */

  /* USER CODE BEGIN ADC2_MspDeInit 1 */

  /* USER CODE END ADC2_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */

extern inline void ADC_SamplingFunction(void)
{
	HAL_ADC_Start_DMA(&hadc1,ADC1_BufferRaw, ADC1_NUMBER_OF_CHANNELS);			/* Start ADC 1 in DMA mode */
	HAL_ADC_Start_DMA(&hadc2,ADC2_BufferRaw, ADC2_NUMBER_OF_CHANNELS); 			/* Start ADC 2 in DMA mode */
}

extern inline void ADC_ReadDataDebug(void)
{
	HAL_ADC_Stop_DMA(&hadc1);			/* Stop ADC1 conversion and wait for timer to restrt it */
	ADC_BufferConvertedDebug[DCU_TEMP_SENSE_POSITION] = DCU_TempSenseConversion(ADC1_BufferRaw[DCU_TEMP_SENSE_POSITION]);
	ADC_BufferConvertedDebug[MAIN_CURRENT_SENSE_POSITION] = DCU_MainCurrentSenseConversion(ADC1_BufferRaw[MAIN_CURRENT_SENSE_POSITION]);
	ADC_BufferConvertedDebug[DCU_CURRENT_SENSE_POSITION] = DCU_CurrentSenseConversion(ADC1_BufferRaw[DCU_CURRENT_SENSE_POSITION]);
	ADC_BufferConvertedDebug[_5V_DCU_POSITION] = DCU_5vSenseConversion(ADC1_BufferRaw[_5V_DCU_POSITION]);
	ADC_BufferConvertedDebug[_12V_POST_DIODES_SENSE_POSITION] = DCU_12vSenseConversion(ADC1_BufferRaw[_12V_POST_DIODES_SENSE_POSITION]);
	ADC_BufferConvertedDebug[_3V3_MCU_POSITION] = DCU_3v3SenseConversion(ADC1_BufferRaw[_3V3_MCU_POSITION]);
	ADC_BufferConvertedDebug[XBEE_CURRENT_SENSE_POSITION] = DCU_XbeeCurrentSenseConversion(ADC1_BufferRaw[XBEE_CURRENT_SENSE_POSITION]);
	ADC_BufferConvertedDebug[VBAT_CHANNEL_POSITION] = DCU_VbatConversion(ADC1_BufferRaw[VBAT_CHANNEL_POSITION]);
	
	if(DATA_GetAcquisitionState() == STATE_ON) {
		_5vTemp = ADC_BufferConvertedDebug[_5V_DCU_POSITION] * 100.0f;
		_12vTemp = ADC_BufferConvertedDebug[_12V_POST_DIODES_SENSE_POSITION] * 100.0f;
		_3v3Temp = ADC_BufferConvertedDebug[_3V3_MCU_POSITION] * 100.0f;
		intToStringUnsigned(ADC_BufferConvertedDebug[DCU_TEMP_SENSE_POSITION], &DATA_BlockBuffer[DCU_TEMP_CSV_INDEX], 2);
		intToStringUnsigned(ADC_BufferConvertedDebug[MAIN_CURRENT_SENSE_POSITION], &DATA_BlockBuffer[DCU_CURRENT_CSV_INDEX], 4);
		intToStringUnsigned(ADC_BufferConvertedDebug[DCU_CURRENT_SENSE_POSITION], &DATA_BlockBuffer[DUC_3V3_CURRENT_CSV_INDEX], 3);
		decimalToStringUnsigned((uint16_t)_5vTemp, &DATA_BlockBuffer[DCU_5V_VOLTAGE_CSV_INDEX], 1, 2);
		decimalToStringUnsigned((uint16_t)_12vTemp, &DATA_BlockBuffer[DCU_12V_VOLTAGE_CSV_INDEX], 2, 2);
		decimalToStringUnsigned((uint16_t)_3v3Temp, &DATA_BlockBuffer[DCU_3V3_VOLTAGE_CSV_INDEX], 1, 2);
		intToStringUnsigned(ADC_BufferConvertedDebug[XBEE_CURRENT_SENSE_POSITION], &DATA_BlockBuffer[XBEE_CURRENT_CSV_INDEX], 3);
	}
}

extern inline void ADC_ReadDataAux(void)
{
	HAL_ADC_Stop_DMA(&hadc2);			/* Stop ADC2 conversion and wait for timer to restrt it */
	ADC_BufferConvertedAux[ANALOG_AUX_1_POSITION] = analogAux1Conversion(ADC2_BufferRaw[ANALOG_AUX_1_RAW_POSITION]);
	ADC_BufferConvertedAux[ANALOG_AUX_2_POSITION] = analogAux2Conversion(ADC2_BufferRaw[ANALOG_AUX_2_RAW_POSITION]);
	ADC_BufferConvertedAux[ANALOG_AUX_3_POSITION] = analogAux3Conversion(ADC2_BufferRaw[ANALOG_AUX_3_RAW_POSITION]);
}

extern void ADC_BuffersInit(void) {
	for (uint8_t i = 0; i < ADC1_NUMBER_OF_CHANNELS; i++) {
		ADC2_BufferRaw [i] = 0;
		ADC_BufferConvertedDebug [i] = 0.0;
	}
	
	for (uint8_t i = 0; i < ADC2_NUMBER_OF_CHANNELS; i++) {
		ADC2_BufferRaw [i] = 0;
		ADC_BufferConvertedAux [i] = 0.0;
	}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
	if(hadc->Instance == ADC1) {
		if(adc1SemaphoreHandle != NULL) {																									/* Check on system start if semaphore is already created */
			HAL_ADC_Stop_DMA(hadc);																													/* Stop ADC1 conversion */
			xSemaphoreGiveFromISR(adc1SemaphoreHandle, &ADC1_xHigherPriorityTaskWoken); 		/* Give semaphore to task when DMA is clear */
			portYIELD_FROM_ISR(ADC1_xHigherPriorityTaskWoken);															/* Do context-switch if needed */
		}
	}
	
	if(hadc->Instance == ADC2) {
		if(adc2SemaphoreHandle != NULL) {																									/* Check on system start if semaphore is already created */
			HAL_ADC_Stop_DMA(hadc);																													/* Stop ADC2 conversion */
			xSemaphoreGiveFromISR(adc2SemaphoreHandle, &ADC2_xHigherPriorityTaskWoken); 		/* Give semaphore to task when DMA is clear */
			portYIELD_FROM_ISR(ADC2_xHigherPriorityTaskWoken);															/* Do context-switch if needed */
		}
	}
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
