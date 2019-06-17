/**
  ******************************************************************************
  * File Name          : USART.c
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
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
#include "usart.h"

/* USER CODE BEGIN 0 */
#include "cmsis_os.h"
#include "gps.h"
#include "telemetry.h"

uint8_t queueUartTransmitFlag; 								/* Receive from queue and store into this variable */
uint8_t usart1LockFlag = UART1_CLEAR_FLAG;		/* Set item to send to queue */
uint8_t startFound = 0;
BaseType_t UART1_TxHigherPriorityTaskWoken = pdFALSE;
BaseType_t UART1_RxHigherPriorityTaskWoken = pdFALSE;
BaseType_t GPS_xHigherPriorityTaskWoken = pdFALSE;
extern uint8_t GPS_ParsingChar;
extern uint8_t GPS_FirstChar;
extern uint8_t i;
extern uint8_t GPS_RawBuffer [GPS_MAX_LENGTH];
extern uint8_t telemetryReceivedBuffer [BUFFER_COMMAND_LEN];
extern uint8_t telemetryIndBuffer [1];
extern osSemaphoreId sendFollowingDataSemaphoreHandle;
extern osSemaphoreId receiveCommandSemaphoreHandle;
extern osSemaphoreId GPSUnboxSemHandle;
extern osMessageQId Usart1LockQueueHandle;
extern osMessageQId Usart1TxModeQueueHandle;
/* USER CODE END 0 */

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 230400;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_RTS_CTS;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_RXOVERRUNDISABLE_INIT|UART_ADVFEATURE_DMADISABLEONERROR_INIT;
  huart1.AdvancedInit.OverrunDisable = UART_ADVFEATURE_OVERRUN_DISABLE;
  huart1.AdvancedInit.DMADisableonRxError = UART_ADVFEATURE_DMA_DISABLEONRXERROR;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }

}
/* USART2 init function */

void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_RXOVERRUNDISABLE_INIT|UART_ADVFEATURE_DMADISABLEONERROR_INIT;
  huart2.AdvancedInit.OverrunDisable = UART_ADVFEATURE_OVERRUN_DISABLE;
  huart2.AdvancedInit.DMADisableonRxError = UART_ADVFEATURE_DMA_DISABLEONRXERROR;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**USART1 GPIO Configuration    
    PA11     ------> USART1_CTS
    PA12     ------> USART1_RTS
    PB6     ------> USART1_TX
    PB7     ------> USART1_RX 
    */
    GPIO_InitStruct.Pin = UART_XBEE_CTS_Pin|UART_XBEE_RTS_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = UART_XBEE_TX_Pin|UART_XBEE_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* USART1 DMA Init */
    /* USART1_RX Init */
    hdma_usart1_rx.Instance = DMA2_Stream5;
    hdma_usart1_rx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_rx.Init.Mode = DMA_NORMAL;
    hdma_usart1_rx.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_usart1_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart1_rx);

    /* USART1_TX Init */
    hdma_usart1_tx.Instance = DMA2_Stream7;
    hdma_usart1_tx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_tx.Init.Mode = DMA_NORMAL;
    hdma_usart1_tx.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_usart1_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart1_tx);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 11, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();
  
    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**USART2 GPIO Configuration    
    PD5     ------> USART2_TX
    PD6     ------> USART2_RX 
    */
    GPIO_InitStruct.Pin = USART_GPS_TX_Pin|USART_GPS_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART2_IRQn, 9, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();
  
    /**USART1 GPIO Configuration    
    PA11     ------> USART1_CTS
    PA12     ------> USART1_RTS
    PB6     ------> USART1_TX
    PB7     ------> USART1_RX 
    */
    HAL_GPIO_DeInit(GPIOA, UART_XBEE_CTS_Pin|UART_XBEE_RTS_Pin);

    HAL_GPIO_DeInit(GPIOB, UART_XBEE_TX_Pin|UART_XBEE_RX_Pin);

    /* USART1 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();
  
    /**USART2 GPIO Configuration    
    PD5     ------> USART2_TX
    PD6     ------> USART2_RX 
    */
    HAL_GPIO_DeInit(GPIOD, USART_GPS_TX_Pin|USART_GPS_RX_Pin);

    /* USART2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */

extern inline void UART1_TxCallback(void)
{
	xQueueReceiveFromISR(Usart1TxModeQueueHandle, (void *)&queueUartTransmitFlag, &UART1_TxHigherPriorityTaskWoken);
	
	/* Check which flag you have received and unlock the correct semaphore */
	switch(queueUartTransmitFlag) {
		
		/* To send second part of data, unlock the secont part tx task and keep the UART semaphore locked to avoit its use from other tasks */
		case SECOND_HALF_TX_FLAG:
			xSemaphoreGiveFromISR(sendFollowingDataSemaphoreHandle, &UART1_TxHigherPriorityTaskWoken); 			/* Give semaphore to task when DMA is clear */
			portYIELD_FROM_ISR(UART1_TxHigherPriorityTaskWoken); 																						/* Do context-switch if needed */
			break;
		
		/* To return to normal mode, unlock the UART locking semaphore */
		case NORMAL_MODE_TX_FLAG:
			xQueueSendFromISR(Usart1LockQueueHandle, &usart1LockFlag, &UART1_TxHigherPriorityTaskWoken);		/* Send wake up signal to task when DMA is clear */
			portYIELD_FROM_ISR(UART1_TxHigherPriorityTaskWoken); 																						/* Do context-switch if needed */
			break;
		
		/* If the flag is unrecognised, raise eventually an error and return to normal mode */
		default:
			xQueueSendFromISR(Usart1LockQueueHandle, &usart1LockFlag, &UART1_TxHigherPriorityTaskWoken);		/* Send wake up signal to task when DMA is clear */
			portYIELD_FROM_ISR(UART1_TxHigherPriorityTaskWoken); 																						/* Do context-switch if needed */
			break;
	}
}

extern inline void UART1_RxCallback(void)
{
	if(startFound == 1) {
		startFound = 0;
		if(receiveCommandSemaphoreHandle != NULL) {																									/* Check at the beginning if the semaphore has been already created */
			xSemaphoreGiveFromISR(receiveCommandSemaphoreHandle, &UART1_RxHigherPriorityTaskWoken); 	/* Give semaphore to task when DMA is clear */
		}
		HAL_UART_Receive_DMA(&huart1, telemetryIndBuffer, 1);
		portYIELD_FROM_ISR(UART1_RxHigherPriorityTaskWoken); 																			/* Do context-switch if needed */
	}
	else {
		if(telemetryIndBuffer[0] == MESSAGE_INIT_ID) {
			telemetryIndBuffer[0] = ' ';
			HAL_UART_Receive_DMA(&huart1, telemetryReceivedBuffer, BUFFER_COMMAND_LEN);
			startFound = 1;
		}
		else {
			HAL_UART_Receive_DMA(&huart1, telemetryIndBuffer, 1);
		}
	}
	
}

extern inline void GPS_RxCallback(void)
{
	/*if initial string charater has been found*/
	if(GPS_FirstChar == '$') {								
		GPS_RawBuffer[i] = GPS_ParsingChar;		/* The char is saved as first element */
		i++;
		HAL_UART_Receive_IT(&huart2, &GPS_ParsingChar, 1);		/* Start reciving successive chars */
	}
	
	/* If last char of the message has been saved */
	if(GPS_ParsingChar == '\n') {								
		GPS_RawBuffer[i] = '\n';		/* Store the last char*/
		GPS_ParsingChar = '$';			/* Reinit variable */
		i = 0;
		
		if(GPSUnboxSemHandle != NULL) {		/* Check at the beginning if the semaphore has been already created */
			xSemaphoreGiveFromISR(GPSUnboxSemHandle, &GPS_xHigherPriorityTaskWoken);		/* Unlock the unboxing task*/
			portYIELD_FROM_ISR(GPS_xHigherPriorityTaskWoken);
		}
	}
	else {		/* If last char of the message has not arrived yet, keep searching for it */	
	 HAL_UART_Receive_IT(&huart2, &GPS_FirstChar, 1);
	}
}

extern void USART2_Init_38400(void)
{
	huart2.Instance = USART2;
  huart2.Init.BaudRate = 38400;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_RXOVERRUNDISABLE_INIT|UART_ADVFEATURE_DMADISABLEONERROR_INIT;
  huart2.AdvancedInit.OverrunDisable = UART_ADVFEATURE_OVERRUN_DISABLE;
  huart2.AdvancedInit.DMADisableonRxError = UART_ADVFEATURE_DMA_DISABLEONRXERROR;
	HAL_UART_Init(&huart2);
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
