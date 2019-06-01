/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
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

#include "fatfs.h"

uint8_t retSD;    /* Return value for SD */
char SDPath[4];   /* SD logical drive path */
FATFS SDFatFS;    /* File system object for SD logical drive */
FIL SDFile;       /* File object for SD */
uint8_t retUSBH;    /* Return value for USBH */
char USBHPath[4];   /* USBH logical drive path */
FATFS USBHFatFS;    /* File system object for USBH logical drive */
FIL USBHFile;       /* File object for USBH */

/* USER CODE BEGIN Variables */
//#include "timestamp.h"

//DWORD FATFS_Year;
//DWORD FATFS_Month;
//DWORD FATFS_Day;
//DWORD FATFS_Hours;
//DWORD FATFS_Minutes;
//DWORD FATFS_Seconds;
//DWORD FATFS_Timestamp;
//extern actualTimestamp_t actualTimestamp;
/* USER CODE END Variables */    

void MX_FATFS_Init(void) 
{
  /*## FatFS: Link the SD driver ###########################*/
  //retSD = FATFS_LinkDriver(&SD_Driver, SDPath);
  /*## FatFS: Link the USBH driver ###########################*/
  retUSBH = FATFS_LinkDriver(&USBH_Driver, USBHPath);

  /* USER CODE BEGIN Init */
  /* additional user code for init */
	if(retUSBH == FR_OK) {
		f_mount(&USBHFatFS, (TCHAR const *)USBHPath, 1);
	}
  /* USER CODE END Init */
}

/**
  * @brief  Gets Time from RTC 
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void)
{
  /* USER CODE BEGIN get_fattime */
	
  /* Shall return any valid time even if the system does not support RTC (return 0) */
	return 0;
	
	/* Currnet local time shall be returned as bit-fields packed into a DWORD value. The bit fields are as follows:
  [bit 31:25] Year origin from the 1980 (0..127)
  [bit 24:21] Month (1..12)
  [bit 20:16] Day of the month(1..31)
  [bit 15:11] Hour (0..23)
  [bit 10:5] Minute (0..59)
  [bit 4:0] Second / 2 (0..29) */
  
	/*if(actualTimestamp.GPS_TimestampReady == GPS_TIMESTAMP_READY) {
		FATFS_Year = (DWORD)actualTimestamp.yearFromGps;
		FATFS_Month = (DWORD)actualTimestamp.monthFromGps;
		FATFS_Day = (DWORD)actualTimestamp.dateNumberFromGps;
		FATFS_Hours = (DWORD)actualTimestamp.hoursFromGps;
		FATFS_Minutes = (DWORD)actualTimestamp.minutesFromGps;
		FATFS_Seconds = (DWORD)actualTimestamp.secondsFromGps;
	}
	else {
		FATFS_Year = (DWORD)actualTimestamp.yearFromRtc;
		FATFS_Month = (DWORD)actualTimestamp.monthFromRtc;
		FATFS_Day = (DWORD)actualTimestamp.dateNumberFromRtc;
		FATFS_Hours = (DWORD)actualTimestamp.hoursFromRtc;
		FATFS_Minutes = (DWORD)actualTimestamp.minutesFromRtc;
		FATFS_Seconds = (DWORD)actualTimestamp.secondsFromRtc;
	}
	
  FATFS_Year = (((FATFS_Year + 2000) - 1980)) << 25;
	FATFS_Month = FATFS_Month << 21;
	FATFS_Day = FATFS_Day << 16;
	FATFS_Hours = FATFS_Hours << 11;
	FATFS_Minutes = FATFS_Minutes << 5;
	FATFS_Seconds = FATFS_Seconds / 2;
	FATFS_Timestamp = 0;
	FATFS_Timestamp = FATFS_Year;
	FATFS_Timestamp = FATFS_Timestamp | FATFS_Month;
	FATFS_Timestamp = FATFS_Timestamp | FATFS_Day;
	FATFS_Timestamp = FATFS_Timestamp | FATFS_Hours;
	FATFS_Timestamp = FATFS_Timestamp | FATFS_Minutes;
	FATFS_Timestamp = FATFS_Timestamp | FATFS_Seconds;
	return FATFS_Timestamp;*/
  /* USER CODE END get_fattime */  
}

/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
