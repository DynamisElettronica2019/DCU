/**
  ******************************************************************************
  * File Name          : ADC.h
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __adc_H
#define __adc_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;

/* USER CODE BEGIN Private defines */
#define ADC1_NUMBER_OF_CHANNELS 						(uint32_t)8
#define ADC2_NUMBER_OF_CHANNELS							(uint32_t)3
#define ADC1_RAW_DATA_LEN 									ADC1_NUMBER_OF_CHANNELS
#define ADC2_RAW_DATA_LEN 									ADC2_NUMBER_OF_CHANNELS
#define ADC_CONVERTED_DEBUG_DATA_LEN 				ADC1_NUMBER_OF_CHANNELS
#define ADC_CONVERTED_AUX_DATA_LEN 					ADC2_NUMBER_OF_CHANNELS
#define DCU_TEMP_SENSE_POSITION           	(uint8_t)0
#define MAIN_CURRENT_SENSE_POSITION					(uint8_t)1
#define DCU_CURRENT_SENSE_POSITION					(uint8_t)2
#define _5V_DCU_POSITION										(uint8_t)3
#define _12V_POST_DIODES_SENSE_POSITION			(uint8_t)4
#define _3V3_MCU_POSITION										(uint8_t)5
#define XBEE_CURRENT_SENSE_POSITION					(uint8_t)6
#define VBAT_CHANNEL_POSITION								(uint8_t)7
#define ANALOG_AUX_1_POSITION								(uint8_t)0
#define ANALOG_AUX_2_POSITION								(uint8_t)1
#define ANALOG_AUX_3_POSITION								(uint8_t)2
#define ANALOG_AUX_1_RAW_POSITION						(uint8_t)2
#define ANALOG_AUX_2_RAW_POSITION						(uint8_t)1
#define ANALOG_AUX_3_RAW_POSITION						(uint8_t)0
/* USER CODE END Private defines */

void MX_ADC1_Init(void);
void MX_ADC2_Init(void);

/* USER CODE BEGIN Prototypes */
extern void adcBuffersInit(void);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ adc_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
