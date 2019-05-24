/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file            : usb_host.c
  * @version         : v1.0_Cube
  * @brief           : This file implements the USB Host
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

#include "usb_host.h"
#include "usbh_core.h"
#include "usbh_msc.h"

/* USER CODE BEGIN Includes */
#include "usbh_platform.h"
#include "fatfs.h"
#include "data.h"
#include "timestamp.h"
#include "string_utility.h"
#include "file_headers.h"
/* USER CODE END Includes */

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
uint32_t len;
UINT bytesWritten;
FRESULT openResult;
FRESULT closeResult;
BaseType_t USB_xHigherPriorityTaskWoken = pdFALSE;
extern uint8_t DATA_BlockWriteIndex;
extern uint8_t DATA_BlockReadIndex;
extern uint8_t DATA_BlockBuffer [BUFFER_BLOCKS_NUMBER][BUFFER_BLOCK_LEN];
extern osSemaphoreId saveUsbSemaphoreHandle;
extern osMessageQId usbEventQueueHandle;
extern osMessageQId startAcquisitionEventHandle;
/* USER CODE END PV */

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USB Host core handle declaration */
USBH_HandleTypeDef hUsbHostHS;
ApplicationTypeDef Appli_state = APPLICATION_IDLE;

/*
 * -- Insert your variables declaration here --
 */
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*
 * user callback declaration
 */
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id);

/*
 * -- Insert your external function declaration here --
 */
/* USER CODE BEGIN 1 */

extern void USB_InitStart(void)
{
	USBH_Init(&hUsbHostHS, USBH_UserProcess, HOST_HS);			/* Init Host Library */
	USBH_RegisterClass(&hUsbHostHS, USBH_MSC_CLASS); 				/* Add Supported Class */
	USBH_Start(&hUsbHostHS);																/* Start Host Process */
	MX_DriverVbusHS(VBUS_ENABLE);														/* Enable USB Vbus power */
	/* Put here the code to manage errors */
}

extern inline void USB_SavingRequest(void)
{
	if(saveUsbSemaphoreHandle != NULL) {
		xSemaphoreGiveFromISR(saveUsbSemaphoreHandle, &USB_xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(USB_xHigherPriorityTaskWoken);
	}
}

extern inline void USB_SavingTask(void)
{
	uint32_t timestamp;
	
	if(DATA_GetAcquisitionState() == STATE_ON) {
		timestamp = getDataTimestamp();			/* Get data timestamp private variable */
		incrementDataTimestamp();						/* Incremente data timestamp private variable by 10 ms */
		uint32ToString(timestamp, &DATA_BlockBuffer[DATA_BlockWriteIndex][TIMESTAMP_CSV_INDEX], 7);
		DATA_SwapDataPackePointers();
		f_write(&USBHFile, DATA_BlockBuffer[DATA_BlockReadIndex], BUFFER_BLOCK_LEN, (void *)&bytesWritten);
		
		
		/* Put here the code to manage errors */
	}
}

extern inline void USB_OpenFile(void)
{		
	if((DATA_GetAcquisitionState() == STATE_OFF) && (DATA_GetUsbReadyState() == STATE_ON)){
		/* Put here getFilename function */
		openResult = f_open(&USBHFile, "DynamisPRC_USB_test.csv", FA_CREATE_ALWAYS | FA_WRITE);
	
		if(openResult == FR_OK) {
			USB_WriteLen(fileHeader);
			USB_WriteLen(channelNameHeader);
			HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
			DATA_SetAcquisitionState();				/* Update of the status packet */
			resetDataTimestamp();							/* Reset data timestamp private variable */
			/* Put here the code to manage errors */
		}
	}
}

extern inline void USB_CloseFile(void)
{
	if((DATA_GetAcquisitionState() == STATE_ON) && (DATA_GetUsbReadyState() == STATE_ON)) {
		closeResult = f_close(&USBHFile);
		
		if(closeResult == FR_OK) {
			HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
			DATA_ResetAcquisitionState();			/* Update of the status packet */
			DATA_PacketReset();								/* Reset the data saving buffer */
			/* Put here the code to manage errors */
		}
	}
}

extern inline void USB_EventHandler(uint8_t USB_Event)
{
	uint8_t startAquisitionEvent = ACQUISITION_IDLE_REQUEST;
	BaseType_t startAcquisition_xHigherPriorityTaskWoken = pdFALSE;
	
	switch(USB_Event) {
		case DISCONNECTION_EVENT:
			f_mount(NULL, (TCHAR const *)"", 1);
			startAquisitionEvent = ACQUISITION_OFF_TELEMETRY_REQUEST;			/* Stop acquisition */
			xQueueSendFromISR(startAcquisitionEventHandle, &startAquisitionEvent, &startAcquisition_xHigherPriorityTaskWoken);
			FATFS_UnLinkDriver(USBHPath);
			DATA_ResetUsbReadyState();																		/* Update of the status packet */
			/* Put here the code to manage errors */
			break;

		case CONNECTED_EVENT:
			if(FATFS_LinkDriver(&USBH_Driver, USBHPath) == 0) {
				f_mount(&USBHFatFS, (TCHAR const *)USBHPath, 1);
				DATA_SetUsbReadyState();					/* Update of the status packet */
			}
			
			/* Put here the code to manage errors */
			break;
	
		default:
			break;
	}
}

extern inline void USB_OvercurrentEvent(void)
{
	if(HAL_GPIO_ReadPin(USB_OVERCURRENT_GPIO_Port, USB_OVERCURRENT_Pin) == GPIO_PIN_RESET) {
			/* Put here the code to handling autogear event */
	}
}

static inline void USB_WriteLen(uint8_t *buffer)
{
  len = strlen((char *)buffer);
	f_write(&USBHFile, buffer, len, (void *)&bytesWritten);
}

/* USER CODE END 1 */

/**
  * Init USB host library, add supported class and start the library
  * @retval None
  */
void MX_USB_HOST_Init(void)
{
  /* USER CODE BEGIN USB_HOST_Init_PreTreatment */
  /* Inside USBH_Start could be present a call to USBH_LL_DriverVBUS (phost, TRUE),
	that disable te VBUS on the port: check it out in case of problems */
  /* USER CODE END USB_HOST_Init_PreTreatment */
  
  /* Init host Library, add supported class and start the library. */
  if (USBH_Init(&hUsbHostHS, USBH_UserProcess, HOST_HS) != USBH_OK)
  {
    Error_Handler();
  }
  if (USBH_RegisterClass(&hUsbHostHS, USBH_MSC_CLASS) != USBH_OK)
  {
    Error_Handler();
  }
  if (USBH_Start(&hUsbHostHS) != USBH_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_HOST_Init_PostTreatment */
  
  /* USER CODE END USB_HOST_Init_PostTreatment */
}

/*
 * user callback definition
 */
static void USBH_UserProcess  (USBH_HandleTypeDef *phost, uint8_t id)
{
  /* USER CODE BEGIN CALL_BACK_1 */
	uint8_t temp;
	BaseType_t usbEvent = pdFALSE;
	
	switch(id)
  {
    case HOST_USER_SELECT_CONFIGURATION:
      break;
      
    case HOST_USER_DISCONNECTION:
      Appli_state = APPLICATION_DISCONNECT;
			temp = DISCONNECTION_EVENT;
			xQueueSendFromISR(usbEventQueueHandle, &temp, &usbEvent);
			portYIELD_FROM_ISR(usbEvent); 
			break;
      
    case HOST_USER_CLASS_ACTIVE:
      Appli_state = APPLICATION_READY;
			temp = CONNECTED_EVENT;
			xQueueSendFromISR(usbEventQueueHandle, &temp, &usbEvent);
			portYIELD_FROM_ISR(usbEvent); 
			break;

    case HOST_USER_CONNECTION:
      Appli_state = APPLICATION_START;
      break;

    default:
      break;
  }
  /* USER CODE END CALL_BACK_1 */
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
