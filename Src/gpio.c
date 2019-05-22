/**
  ******************************************************************************
  * File Name          : gpio.c
  * Description        : This file provides code for the configuration
  *                      of all used GPIO pins.
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
#include "gpio.h"
/* USER CODE BEGIN 0 */
#include "cmsis_os.h"
/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */
uint8_t digitalAuxEvent = 0;
BaseType_t GPIO_AutogearxHigherPriorityTaskWoken = pdFALSE;
BaseType_t GPIO_OvercurrentxHigherPriorityTaskWoken = pdFALSE;
BaseType_t GPIO_Aux1xHigherPriorityTaskWoken = pdFALSE;
BaseType_t GPIO_Aux2xHigherPriorityTaskWoken = pdFALSE;
BaseType_t GPIO_Aux3xHigherPriorityTaskWoken = pdFALSE;
extern osSemaphoreId autogearSemaphoreHandle;
extern osSemaphoreId USB_OvercurrentSemaphoreHandle;
extern osMessageQId digitalAuxQueueHandle;
/* USER CODE END 1 */

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, ETH_nRST_Pin|LED_GREEN_Pin|LED_RED_Pin|LED_YELLOW_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(IMU_nRST_GPIO_Port, IMU_nRST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USB_5V_ENABLE_GPIO_Port, USB_5V_ENABLE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PEPin PEPin PEPin */
  GPIO_InitStruct.Pin = DIGITAL_AUX_2_Pin|DIGITAL_AUX_3_Pin|DIGITAL_AUX_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PDPin PDPin PDPin PDPin */
  GPIO_InitStruct.Pin = ETH_nRST_Pin|LED_GREEN_Pin|LED_RED_Pin|LED_YELLOW_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = IMU_nRST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(IMU_nRST_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PAPin PAPin */
  GPIO_InitStruct.Pin = AUTOGEAR_SWTICH_MCU_Pin|USB_OVERCURRENT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = USB_5V_ENABLE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_5V_ENABLE_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = uSD_DETECTED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(uSD_DETECTED_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 6, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 6, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 2 */

extern inline void GPIO_UsbOvercurrentISR(void)
{
	if(HAL_GPIO_ReadPin(USB_OVERCURRENT_GPIO_Port, USB_OVERCURRENT_Pin) == GPIO_PIN_RESET) {
		if(USB_OvercurrentSemaphoreHandle != NULL) {																														/* Check on system start if semaphore is already created */
			xSemaphoreGiveFromISR(USB_OvercurrentSemaphoreHandle, &GPIO_OvercurrentxHigherPriorityTaskWoken); 		/* Give semaphore to task when interrupt is called */
			portYIELD_FROM_ISR(GPIO_OvercurrentxHigherPriorityTaskWoken);																					/* Do context-switch if needed */
		}
	}
}


extern inline void GPIO_AutogearSwitchISR(void)
{
	if(HAL_GPIO_ReadPin(AUTOGEAR_SWTICH_MCU_GPIO_Port, AUTOGEAR_SWTICH_MCU_Pin) == GPIO_PIN_RESET) {
		if(autogearSemaphoreHandle != NULL) {																																		/* Check on system start if semaphore is already created */
			xSemaphoreGiveFromISR(autogearSemaphoreHandle, &GPIO_AutogearxHigherPriorityTaskWoken); 							/* Give semaphore to task when interrupt is called */
			portYIELD_FROM_ISR(GPIO_AutogearxHigherPriorityTaskWoken);																						/* Do context-switch if needed */
		}
	}
}

extern inline void GPIO_AuxSamplingFunction(void)
{
	if(HAL_GPIO_ReadPin(DIGITAL_AUX_1_GPIO_Port, DIGITAL_AUX_1_Pin) == GPIO_PIN_SET) {
		digitalAuxEvent = DIGITAL_EVENT_AUX_1;
		xQueueSendFromISR(digitalAuxQueueHandle, &digitalAuxEvent, &GPIO_Aux1xHigherPriorityTaskWoken);					/* Add digital aux event to queue */
	}
	
	if(HAL_GPIO_ReadPin(DIGITAL_AUX_2_GPIO_Port, DIGITAL_AUX_2_Pin) == GPIO_PIN_SET) {
		digitalAuxEvent = DIGITAL_EVENT_AUX_2;
		xQueueSendFromISR(digitalAuxQueueHandle, &digitalAuxEvent, &GPIO_Aux2xHigherPriorityTaskWoken);					/* Add digital aux event to queue */
	}
	
	if(HAL_GPIO_ReadPin(DIGITAL_AUX_3_GPIO_Port, DIGITAL_AUX_3_Pin) == GPIO_PIN_SET) {
		digitalAuxEvent = DIGITAL_EVENT_AUX_3;
		xQueueSendFromISR(digitalAuxQueueHandle, &digitalAuxEvent, &GPIO_Aux3xHigherPriorityTaskWoken);					/* Add digital aux event to queue */
	}
}

extern inline void GPIO_AuxManageEvent(uint8_t event)
{
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

/* USER CODE END 2 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
