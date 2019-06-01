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
#include "tim.h"
#include "fatfs.h"
#include "data.h"
#include "telemetry.h"
#include "timestamp.h"
#include "string_utility.h"
#include "file_headers.h"
/* USER CODE END Includes */

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
uint8_t USB_ISR_Event;
uint8_t USB_Year;
uint8_t USB_Month;
uint8_t USB_Day;
uint8_t USB_Hours;
uint8_t USB_Minutes;
uint8_t USB_Seconds;
uint8_t DATA_openAttempsNumber = 0;
uint8_t DATA_closeAttempsNumber = 0;
uint32_t len;
UINT bytesWritten;
FRESULT openResult;
FRESULT closeResult;
FRESULT writeResult;
BaseType_t USB_xHigherPriorityTaskWoken = pdFALSE;
BaseType_t USB_EventHigherPriorityTaskWoken = pdFALSE;
TCHAR USB_Filename[25];
extern uint8_t DATA_BlockWriteIndex;
extern uint8_t DATA_BlockReadIndex;
extern uint8_t DATA_BlockBuffer [BUFFER_POINTERS_NUMBER][BUFFER_BLOCK_LEN];
extern actualTimestamp_t actualTimestamp;
extern osSemaphoreId saveUsbSemaphoreHandle;
extern osMessageQId usbEventQueueHandle;
extern osMessageQId ErrorQueueHandle;
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
	USBH_Init(&hUsbHostHS, USBH_UserProcess, HOST_HS);		/* Init Host Library */
	USBH_RegisterClass(&hUsbHostHS, USBH_MSC_CLASS); 			/* Add Supported Class */
	USBH_Start(&hUsbHostHS);															/* Start Host Process */
	MX_DriverVbusHS(VBUS_ENABLE);													/* Enable USB Vbus power */
	HAL_Delay(100);																				/* Delay for Vbus stabilization */
}

extern inline void USB_SavingRequest(void)
{
	xSemaphoreGiveFromISR(saveUsbSemaphoreHandle, &USB_xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(USB_xHigherPriorityTaskWoken);
}

extern inline void USB_SavingTask(void)
{
	uint32_t timestamp;
	uint8_t errorLetter = USB_WRITE_FILE_ERROR;
	
	if(DATA_GetAcquisitionState() == STATE_ON) {
		timestamp = getDataTimestamp();			/* Get data timestamp private variable */
		uint32ToString(timestamp, &DATA_BlockBuffer[DATA_BlockWriteIndex][TIMESTAMP_CSV_INDEX], 7);
		DATA_SwapDataPackePointers();
		writeResult = f_write(&USBHFile, DATA_BlockBuffer[DATA_BlockReadIndex], BUFFER_BLOCK_LEN, (void *)&bytesWritten);
		
		if(writeResult != FR_OK) {
			xQueueSend(ErrorQueueHandle, (void *)&errorLetter, (TickType_t)0); 		/* Add error to queue */
		}
		
		if((timestamp % CLOSE_FILE_INTERVAL) == 0) {
			USB_CloseAndOpenFile();
		}
	}
}

extern inline void USB_OpenFile(void)
{
	uint8_t errorLetter = USB_OPEN_FILE_ERROR;
	
	if((DATA_GetAcquisitionState() == STATE_OFF) && (DATA_GetUsbReadyState() == STATE_ON)){
		if(DATA_openAttempsNumber <= OPEN_FILE_TENTATIVES_NUMBER) {
			USB_GetFilename(); 		/* Set filename based on actual date and time */
			openResult = f_open(&USBHFile, USB_Filename, FA_CREATE_ALWAYS | FA_WRITE);
			DATA_openAttempsNumber++;
			
			if(openResult == FR_OK) {
				USB_WriteLen(fileHeader);
				USB_WriteLen(channelNameHeader);
				DATA_SetAcquisitionState();				/* Update of the status packet */
				resetDataTimestamp();							/* Reset data timestamp private variable */
				HAL_TIM_Base_Start_IT(&htim7); 		/* Start timer 7 (100 Hz) in interrupt mode */
				DATA_openAttempsNumber = 0;
				HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
			}
		}
		
		else {
			xQueueSend(ErrorQueueHandle, (void *)&errorLetter, (TickType_t)0); 		/* Add error to queue */
		}
	}
}

extern inline void USB_CloseFile(void)
{
	uint8_t errorLetter = USB_CLOSE_FILE_ERROR;
	
	if((DATA_GetAcquisitionState() == STATE_ON) && (DATA_GetUsbReadyState() == STATE_ON)) {
		if(DATA_closeAttempsNumber < CLOSE_FILE_TENTATIVES_NUMBER) {
			closeResult = f_close(&USBHFile);
			DATA_closeAttempsNumber++;
			
			if(closeResult == FR_OK) {
				HAL_TIM_Base_Stop_IT(&htim7); 		/* Stop timer 7 (100 Hz) */
				DATA_ResetAcquisitionState();			/* Update of the status packet */
				DATA_PacketReset();								/* Reset the data saving buffer */
				DATA_closeAttempsNumber = 0;
				HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
			}
		}
		
		else {
			xQueueSend(ErrorQueueHandle, (void *)&errorLetter, (TickType_t)0); 		/* Add error to queue */
		}
	}
}

extern inline void USB_EventHandler(uint8_t USB_Event)
{	
	switch(USB_Event) {
		case DISCONNECTION_EVENT:
			f_mount(NULL, (TCHAR const *)"", 1);
			DATA_ResetAcquisitionStateMachine();
			FATFS_UnLinkDriver(USBHPath);
			DATA_ResetUsbReadyState();			/* Update of the status packet */
		case CONNECTED_EVENT:
			if(FATFS_LinkDriver(&USBH_Driver, USBHPath) == 0) {
				f_mount(&USBHFatFS, (TCHAR const *)USBHPath, 1);
				DATA_SetUsbReadyState();		/* Update of the status packet */
			}
			break;
			
		default:
			break;
	}
}

extern inline void USB_OvercurrentEvent(void)
{
	uint8_t errorLetter = USB_OVERCURRENT_ERROR;
	
	if(HAL_GPIO_ReadPin(USB_OVERCURRENT_GPIO_Port, USB_OVERCURRENT_Pin) == GPIO_PIN_RESET) {
		MX_DriverVbusHS(VBUS_DISABLE);		/* Disable USB Vbus power */
		HAL_TIM_Base_Stop_IT(&htim7); 		/* Stop timer 7 (100 Hz) */
		DATA_ResetAcquisitionState();			/* Update of the status packet */
		DATA_PacketReset();								/* Reset the data saving buffer */
		DATA_ResetUsbReadyState();
		DATA_ResetAcquisitionState();
		HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
		xQueueSend(ErrorQueueHandle, (void *)&errorLetter, (TickType_t)0); 		/* Add error to queue */
	}
}

static inline void USB_CloseAndOpenFile(void)
{
	uint8_t errorLetter;
	
	if(f_close(&USBHFile) != FR_OK) {
		HAL_TIM_Base_Stop_IT(&htim7); 		/* Stop timer 7 (100 Hz) */
		DATA_ResetAcquisitionState();			/* Update of the status packet */
		DATA_PacketReset();								/* Reset the data saving buffer */
		DATA_ResetUsbReadyState();
		errorLetter = USB_CLOSE_FILE_ERROR;
		HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
		xQueueSend(ErrorQueueHandle, (void *)&errorLetter, (TickType_t)0); 		/* Add error to queue */
	}
	else {
		if(f_open(&USBHFile, USB_Filename, FA_OPEN_APPEND | FA_WRITE) != FR_OK) {
			HAL_TIM_Base_Stop_IT(&htim7); 		/* Stop timer 7 (100 Hz) */
			DATA_ResetAcquisitionState();			/* Update of the status packet */
			DATA_PacketReset();								/* Reset the data saving buffer */
			DATA_ResetUsbReadyState();
			errorLetter = USB_OPEN_FILE_ERROR;
			HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
			xQueueSend(ErrorQueueHandle, (void *)&errorLetter, (TickType_t)0); 		/* Add error to queue */
		}
	}
}

static inline void USB_WriteLen(uint8_t *buffer)
{
	uint8_t errorLetter = USB_WRITE_FILE_ERROR;
	
  len = strlen((char *)buffer);
	writeResult = f_write(&USBHFile, buffer, len, (void *)&bytesWritten);
	
	if(writeResult != FR_OK) {
		xQueueSend(ErrorQueueHandle, (void *)&errorLetter, (TickType_t)0); 		/* Add error to queue */
	}
}

static inline void USB_GetFilename(void)
{
	if(actualTimestamp.GPS_TimestampReady == GPS_TIMESTAMP_READY) {
		USB_Year = actualTimestamp.yearFromGps;
		USB_Month = actualTimestamp.monthFromGps;
		USB_Day = actualTimestamp.dateNumberFromGps;
		USB_Hours = actualTimestamp.hoursFromGps;
		USB_Minutes = actualTimestamp.minutesFromGps;
		USB_Seconds = actualTimestamp.secondsFromGps;
	}
	else {
		USB_Year = actualTimestamp.yearFromRtc;
		USB_Month = actualTimestamp.monthFromRtc;
		USB_Day = actualTimestamp.dateNumberFromRtc;
		USB_Hours = actualTimestamp.hoursFromRtc;
		USB_Minutes = actualTimestamp.minutesFromRtc;
		USB_Seconds = actualTimestamp.secondsFromRtc;
	}
	
  sprintf(USB_Filename, "%d_%d_%d-%d_%d_%d.csv", USB_Year, USB_Month, USB_Day, USB_Hours, USB_Minutes, USB_Seconds);
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
	switch(id) {
    case HOST_USER_SELECT_CONFIGURATION:
      break;
      
    case HOST_USER_DISCONNECTION:
      Appli_state = APPLICATION_DISCONNECT;
			USB_ISR_Event = DISCONNECTION_EVENT;
			DATA_ResetAcquisitionStateMachine();
			DATA_ResetUsbReadyState();		/* Update of the status packet */
			break;
      
    case HOST_USER_CLASS_ACTIVE:
      Appli_state = APPLICATION_READY;
			USB_ISR_Event = CONNECTED_EVENT;
			DATA_SetUsbReadyState();			/* Update of the status packet */
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
