/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usb_host.h
  * @version        : v1.0_Cube
  * @brief          : Header for usb_host.c file.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_HOST__H__
#define __USB_HOST__H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"

/* USER CODE BEGIN INCLUDE */
#include "cmsis_os.h"

#define VBUS_ENABLE 										(uint8_t)0
#define VBUS_DISABLE 										(uint8_t)1
#define OPEN_FILE_TENTATIVES_NUMBER 		(uint8_t)8
#define CLOSE_FILE_TENTATIVES_NUMBER 		(uint8_t)8


typedef enum {
  DISCONNECTION_EVENT = 1,
	CONNECTED_EVENT
} usbApplicationTypedef;

extern void USB_InitStart(void);
extern inline void USB_SavingRequest(void);
extern inline void USB_SavingTask(void);
extern inline void USB_OpenFile(void);
extern inline void USB_CloseFile(void);
extern inline void USB_EventHandler(uint8_t USB_Event);
extern inline void USB_OvercurrentEvent(void);
static inline void USB_CloseAndOpenFile(void);
static inline void USB_WriteLen(uint8_t *buffer);
static inline void USB_GetFilename(void);
/* USER CODE END INCLUDE */

/** @addtogroup USBH_OTG_DRIVER
  * @{
  */

/** @defgroup USBH_HOST USBH_HOST
  * @brief Host file for Usb otg low level driver.
  * @{
  */

/** @defgroup USBH_HOST_Exported_Variables USBH_HOST_Exported_Variables
  * @brief Public variables.
  * @{
  */

/**
  * @}
  */

/** Status of the application. */
typedef enum {
  APPLICATION_IDLE = 0,
  APPLICATION_START,
  APPLICATION_READY,
  APPLICATION_DISCONNECT
}ApplicationTypeDef;

/** @defgroup USBH_HOST_Exported_FunctionsPrototype USBH_HOST_Exported_FunctionsPrototype
  * @brief Declaration of public functions for Usb host.
  * @{
  */

/* Exported functions -------------------------------------------------------*/

/** @brief USB Host initialization function. */
void MX_USB_HOST_Init(void);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __USB_HOST__H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
