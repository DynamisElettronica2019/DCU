/**
  ******************************************************************************
  * File Name          : CAN.c
  * Description        : This file provides code for the configuration
  *                      of the CAN instances.
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
#include "can.h"

/* USER CODE BEGIN 0 */
#include "cmsis_os.h"
#include "id_can.h"
#include "adc.h"
#include "data.h"
#include "telemetry.h"

uint8_t toSW_AcquisitionState = TO_SW_ACQUISITION_IS_ON;
CAN_FilterTypeDef CAN_FilterConfigHeader;
CAN_RxPacket_t CAN_CurrentFifo0ReceivedPacket;
CAN_RxPacket_t CAN_CurrentFifo1ReceivedPacket;
CAN_TxPacket_t CAN_AutogearPacket;
CAN_TxPacket_t CAN_DebugPacket1Packet;
CAN_TxPacket_t CAN_DebugPacket2Packet;
CAN_TxPacket_t CAN_AcquisitionStatePacket;
uint32_t packetMailbox;
BaseType_t CAN_SendxHigherPriorityTaskWoken = pdFALSE;
BaseType_t CAN_Rx0xHigherPriorityTaskWoken = pdFALSE;
BaseType_t CAN_Rx1xHigherPriorityTaskWoken = pdFALSE;
BaseType_t CAN_Tx0xHigherPriorityTaskWoken = pdFALSE;
BaseType_t CAN_Tx1xHigherPriorityTaskWoken = pdFALSE;
BaseType_t CAN_Tx2xHigherPriorityTaskWoken = pdFALSE;
uint32_t CAN_ReceivedPacketsCounter [NUMBER_OF_ACQUIRED_CHANNELS];
extern float ADC_BufferConvertedDebug [ADC_CONVERTED_DEBUG_DATA_LEN];
extern osSemaphoreId CAN_SendSemaphoreHandle;
extern osSemaphoreId CAN_SendDataSemaphoreCounterHandle;
extern osMessageQId canFifo0QueueHandle;
extern osMessageQId canFifo1QueueHandle;
extern osMessageQId CAN_SendDataQueueHandle;
/* USER CODE END 0 */

CAN_HandleTypeDef hcan1;

/* CAN1 init function */
void MX_CAN1_Init(void)
{

  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 3;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_2TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_13TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_4TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspInit 0 */

  /* USER CODE END CAN1_MspInit 0 */
    /* CAN1 clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();
  
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**CAN1 GPIO Configuration    
    PB8     ------> CAN1_RX
    PB9     ------> CAN1_TX 
    */
    GPIO_InitStruct.Pin = CAN_RX_Pin|CAN_TX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* CAN1 interrupt Init */
    HAL_NVIC_SetPriority(CAN1_TX_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);
    HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
    HAL_NVIC_SetPriority(CAN1_RX1_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);
    HAL_NVIC_SetPriority(CAN1_SCE_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(CAN1_SCE_IRQn);
  /* USER CODE BEGIN CAN1_MspInit 1 */

  /* USER CODE END CAN1_MspInit 1 */
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* canHandle)
{

  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspDeInit 0 */

  /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN1_CLK_DISABLE();
  
    /**CAN1 GPIO Configuration    
    PB8     ------> CAN1_RX
    PB9     ------> CAN1_TX 
    */
    HAL_GPIO_DeInit(GPIOB, CAN_RX_Pin|CAN_TX_Pin);

    /* CAN1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(CAN1_TX_IRQn);
    HAL_NVIC_DisableIRQ(CAN1_RX0_IRQn);
    HAL_NVIC_DisableIRQ(CAN1_RX1_IRQn);
    HAL_NVIC_DisableIRQ(CAN1_SCE_IRQn);
  /* USER CODE BEGIN CAN1_MspDeInit 1 */

  /* USER CODE END CAN1_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */

extern void CAN_Start(void)
{
	CAN_PacketInit();
	CAN_FilterConfig();
	HAL_CAN_ActivateNotification(&hcan1, CAN_IT_TX_MAILBOX_EMPTY);
	HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
  HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO1_MSG_PENDING);
	HAL_CAN_Start(&hcan1);
}

extern inline void CAN_SendPackets(void)
{
	xSemaphoreGiveFromISR(CAN_SendSemaphoreHandle, &CAN_SendxHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(CAN_SendxHigherPriorityTaskWoken);
}

extern inline void CAN_SendDebugPackets(void)
{
	if(DATA_GetAcquisitionState() == STATE_ON) {
		toSW_AcquisitionState = TO_SW_ACQUISITION_IS_ON;
	}
	else {
		toSW_AcquisitionState = TO_SW_ACQUISITION_IS_OFF;
	}
	
	/* DCU_DEBUG_1_ID */
	CAN_DebugPacket1Packet.packetData[0] = (uint8_t)(((uint16_t)(ADC_BufferConvertedDebug[DCU_TEMP_SENSE_POSITION]) >> 8) & 0x00FF);
	CAN_DebugPacket1Packet.packetData[1] = (uint8_t)((uint16_t)(ADC_BufferConvertedDebug[DCU_TEMP_SENSE_POSITION]) & 0x00FF);
	CAN_DebugPacket1Packet.packetData[2] = (uint8_t)(((uint16_t)(ADC_BufferConvertedDebug[MAIN_CURRENT_SENSE_POSITION]) >> 8) & 0x00FF);
	CAN_DebugPacket1Packet.packetData[3] = (uint8_t)((uint16_t)(ADC_BufferConvertedDebug[MAIN_CURRENT_SENSE_POSITION]) & 0x00FF);
	CAN_DebugPacket1Packet.packetData[4] = (uint8_t)(((uint16_t)(ADC_BufferConvertedDebug[XBEE_CURRENT_SENSE_POSITION]) >> 8) & 0x00FF);
	CAN_DebugPacket1Packet.packetData[5] = (uint8_t)((uint16_t)(ADC_BufferConvertedDebug[XBEE_CURRENT_SENSE_POSITION]) & 0x00FF);
	CAN_DebugPacket1Packet.packetData[6] = (uint8_t)(((uint16_t)(ADC_BufferConvertedDebug[DCU_CURRENT_SENSE_POSITION]) >> 8) & 0x00FF);
	CAN_DebugPacket1Packet.packetData[7] = (uint8_t)((uint16_t)(ADC_BufferConvertedDebug[DCU_CURRENT_SENSE_POSITION]) & 0x00FF);
	HAL_CAN_AddTxMessage(&hcan1, &CAN_DebugPacket1Packet.packetHeader, CAN_DebugPacket1Packet.packetData, &packetMailbox);
	//xQueueSend(CAN_SendDataQueueHandle, (void *)&CAN_DebugPacket1Packet, 10/portTICK_PERIOD_MS);
	
	/* DCU_DEBUG_2_ID */
	CAN_DebugPacket2Packet.packetData[0] = (uint8_t)(((uint16_t)(ADC_BufferConvertedDebug[_12V_POST_DIODES_SENSE_POSITION]) >> 8) & 0x00FF);
	CAN_DebugPacket2Packet.packetData[1] = (uint8_t)((uint16_t)(ADC_BufferConvertedDebug[_12V_POST_DIODES_SENSE_POSITION]) & 0x00FF);
	CAN_DebugPacket2Packet.packetData[2] = (uint8_t)(((uint16_t)(ADC_BufferConvertedDebug[_5V_DCU_POSITION]) >> 8) & 0x00FF);
	CAN_DebugPacket2Packet.packetData[3] = (uint8_t)((uint16_t)(ADC_BufferConvertedDebug[_5V_DCU_POSITION]) & 0x00FF);
	CAN_DebugPacket2Packet.packetData[4] = (uint8_t)(((uint16_t)(ADC_BufferConvertedDebug[_3V3_MCU_POSITION]) >> 8) & 0x00FF);
	CAN_DebugPacket2Packet.packetData[5] = (uint8_t)((uint16_t)(ADC_BufferConvertedDebug[_3V3_MCU_POSITION]) & 0x00FF);
	HAL_CAN_AddTxMessage(&hcan1, &CAN_DebugPacket2Packet.packetHeader, CAN_DebugPacket2Packet.packetData, &packetMailbox);
	//xQueueSend(CAN_SendDataQueueHandle, (void *)&CAN_DebugPacket2Packet, 10/portTICK_PERIOD_MS);
	
	/* DCU_ACQUISITION_SW_ID */
	CAN_AcquisitionStatePacket.packetData[3] = toSW_AcquisitionState;
	HAL_CAN_AddTxMessage(&hcan1, &CAN_AcquisitionStatePacket.packetHeader, CAN_AcquisitionStatePacket.packetData, &packetMailbox);
	//xQueueSend(CAN_SendDataQueueHandle, (void *)&CAN_AcquisitionStatePacket, 10/portTICK_PERIOD_MS);
}

extern inline void CAN_SendAutogearPacket(void)
{
	if(HAL_GPIO_ReadPin(AUTOGEAR_SWTICH_MCU_GPIO_Port, AUTOGEAR_SWTICH_MCU_Pin) == GPIO_PIN_RESET) {
		//xQueueSend(CAN_SendDataQueueHandle, (void *)&CAN_AutogearPacket, 10/portTICK_PERIOD_MS);		/* Add CAN message to queue */
		//HAL_CAN_AddTxMessage(&hcan1, &CAN_AutogearPacket.packetHeader, CAN_AutogearPacket.packetData, &packetMailbox);
	}
}

extern inline void CAN_SW_CalibrationSendAck(uint16_t ackValue)
{
	CAN_AcquisitionStatePacket.packetData[1] = 2;
	CAN_AcquisitionStatePacket.packetData[3] = ackValue;
	HAL_CAN_AddTxMessage(&hcan1, &CAN_AcquisitionStatePacket.packetHeader, CAN_AcquisitionStatePacket.packetData, &packetMailbox);
	CAN_AcquisitionStatePacket.packetData[1] = 1;
	HAL_CAN_AddTxMessage(&hcan1, &CAN_AcquisitionStatePacket.packetHeader, CAN_AcquisitionStatePacket.packetData, &packetMailbox);
}

extern void CAN_PacketCounterReset(void)
{
	for(uint16_t i = 0; i < NUMBER_OF_ACQUIRED_CHANNELS; i++) {
		CAN_ReceivedPacketsCounter[i] = 0;
	}
}

static void CAN_FilterConfig(void)
{
	CAN_FilterConfigHeader.FilterBank = 0;
  CAN_FilterConfigHeader.FilterMode = CAN_FILTERMODE_IDMASK;
  CAN_FilterConfigHeader.FilterScale = CAN_FILTERSCALE_32BIT;
	CAN_FilterConfigHeader.FilterIdHigh = (0x0000 << 5);
  CAN_FilterConfigHeader.FilterIdLow = 0x0000;
	CAN_FilterConfigHeader.FilterMaskIdHigh = (0x0001 << 5);
  CAN_FilterConfigHeader.FilterMaskIdLow = 0x0000;
	CAN_FilterConfigHeader.FilterFIFOAssignment = CAN_RX_FIFO0;
  CAN_FilterConfigHeader.FilterActivation = ENABLE;	
  CAN_FilterConfigHeader.SlaveStartFilterBank = 14;
	HAL_CAN_ConfigFilter(&hcan1, &CAN_FilterConfigHeader);
	
  CAN_FilterConfigHeader.FilterBank = 1;
  CAN_FilterConfigHeader.FilterMode = CAN_FILTERMODE_IDMASK;
  CAN_FilterConfigHeader.FilterScale = CAN_FILTERSCALE_32BIT;
	CAN_FilterConfigHeader.FilterIdHigh = (0x0001 << 5);
  CAN_FilterConfigHeader.FilterIdLow = 0x0000;
	CAN_FilterConfigHeader.FilterMaskIdHigh = (0x0001 << 5);
  CAN_FilterConfigHeader.FilterMaskIdLow = 0x0000;
	CAN_FilterConfigHeader.FilterFIFOAssignment = CAN_RX_FIFO1;
  CAN_FilterConfigHeader.FilterActivation = ENABLE;	
  CAN_FilterConfigHeader.SlaveStartFilterBank = 14;
	HAL_CAN_ConfigFilter(&hcan1, &CAN_FilterConfigHeader);
}

static void CAN_PacketInit(void)
{
	/* DCU_DEBUG_1_ID */
	CAN_DebugPacket1Packet.packetHeader.StdId = DCU_DEBUG_1_ID;
	CAN_DebugPacket1Packet.packetHeader.RTR = CAN_RTR_DATA;
	CAN_DebugPacket1Packet.packetHeader.IDE = CAN_ID_STD;
	CAN_DebugPacket1Packet.packetHeader.DLC = 8;
	CAN_DebugPacket1Packet.packetHeader.TransmitGlobalTime = DISABLE;
	
	/* DCU_DEBUG_2_ID */
	CAN_DebugPacket2Packet.packetHeader.StdId = DCU_DEBUG_2_ID;
	CAN_DebugPacket2Packet.packetHeader.RTR = CAN_RTR_DATA;
	CAN_DebugPacket2Packet.packetHeader.IDE = CAN_ID_STD;
	CAN_DebugPacket2Packet.packetHeader.DLC = 6;
	CAN_DebugPacket2Packet.packetHeader.TransmitGlobalTime = DISABLE;
	
	/* DCU_ACQUISITION_SW_ID */
	CAN_AcquisitionStatePacket.packetHeader.StdId = DCU_ACQUISITION_SW_ID;
	CAN_AcquisitionStatePacket.packetHeader.RTR = CAN_RTR_DATA;
	CAN_AcquisitionStatePacket.packetHeader.IDE = CAN_ID_STD;
	CAN_AcquisitionStatePacket.packetHeader.DLC = 4;
	CAN_AcquisitionStatePacket.packetHeader.TransmitGlobalTime = DISABLE;
	CAN_AcquisitionStatePacket.packetData[0] = 0;
	CAN_AcquisitionStatePacket.packetData[1] = 1;
	CAN_AcquisitionStatePacket.packetData[2] = 0;
	
	 /* DCU_AUTOGEARSHIFT_GCU_ID*/
	CAN_AutogearPacket.packetData[0] = 0;
	CAN_AutogearPacket.packetData[1] = 1;
	CAN_AutogearPacket.packetHeader.StdId = DCU_AUTOGEARSHIFT_GCU_ID;
	CAN_AutogearPacket.packetHeader.RTR = CAN_RTR_DATA;
	CAN_AutogearPacket.packetHeader.IDE = CAN_ID_STD;
	CAN_AutogearPacket.packetHeader.DLC = 2;
	CAN_AutogearPacket.packetHeader.TransmitGlobalTime = DISABLE;
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &(CAN_CurrentFifo0ReceivedPacket.packetHeader), CAN_CurrentFifo0ReceivedPacket.packetData);
	xQueueSendFromISR(canFifo0QueueHandle, &CAN_CurrentFifo0ReceivedPacket, &CAN_Rx0xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(CAN_Rx0xHigherPriorityTaskWoken);
}

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO1, &(CAN_CurrentFifo1ReceivedPacket.packetHeader), CAN_CurrentFifo1ReceivedPacket.packetData); 
	xQueueSendFromISR(canFifo1QueueHandle, &CAN_CurrentFifo1ReceivedPacket, &CAN_Rx1xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(CAN_Rx1xHigherPriorityTaskWoken);
}

void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{
	xSemaphoreGiveFromISR(CAN_SendDataSemaphoreCounterHandle, &CAN_Tx0xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(CAN_Tx0xHigherPriorityTaskWoken);
}

void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan)
{
	xSemaphoreGiveFromISR(CAN_SendDataSemaphoreCounterHandle, &CAN_Tx1xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(CAN_Tx1xHigherPriorityTaskWoken);
}

void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan)
{
	xSemaphoreGiveFromISR(CAN_SendDataSemaphoreCounterHandle, &CAN_Tx2xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(CAN_Tx2xHigherPriorityTaskWoken);
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
