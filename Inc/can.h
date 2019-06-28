/**
  ******************************************************************************
  * File Name          : CAN.h
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __can_H
#define __can_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern CAN_HandleTypeDef hcan1;

/* USER CODE BEGIN Private defines */
typedef struct
{
	CAN_RxHeaderTypeDef packetHeader;
	uint8_t packetData[8];
} CAN_RxPacket_t;

typedef struct
{
	CAN_TxHeaderTypeDef packetHeader;
	uint8_t packetData[8];
} CAN_TxPacket_t;
/* USER CODE END Private defines */

void MX_CAN1_Init(void);

/* USER CODE BEGIN Prototypes */
extern void CAN_Start(void);
extern inline void CAN_SendPackets(void);
extern inline void CAN_SendDebugPackets(void);
extern inline void CAN_SendAutogearPacket(void);
extern inline void CAN_SW_CalibrationSendAck(uint8_t ackValue);
extern void CAN_PacketCounterReset(void);
static inline void CAN_SendPacketPolling(CAN_TxPacket_t packet);
static void CAN_FilterConfig(void);
static void CAN_PacketInit(void);
static void CAN_SetFiler(uint8_t filterBank, uint16_t filterMask,  uint16_t filterID, uint32_t CAN_FIFO_Number);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ can_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
