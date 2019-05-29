/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MCU_OSC32_IN_Pin GPIO_PIN_14
#define MCU_OSC32_IN_GPIO_Port GPIOC
#define MCU_OSC32_OUT_Pin GPIO_PIN_15
#define MCU_OSC32_OUT_GPIO_Port GPIOC
#define MCU_OSC_IN_Pin GPIO_PIN_0
#define MCU_OSC_IN_GPIO_Port GPIOH
#define MCU_OSC_OUT_Pin GPIO_PIN_1
#define MCU_OSC_OUT_GPIO_Port GPIOH
#define _12V_POST_DIODES_SENSE_Pin GPIO_PIN_0
#define _12V_POST_DIODES_SENSE_GPIO_Port GPIOC
#define _3V3_REG_SENSE_Pin GPIO_PIN_2
#define _3V3_REG_SENSE_GPIO_Port GPIOC
#define XBEE_CURRENT_SENSE_Pin GPIO_PIN_3
#define XBEE_CURRENT_SENSE_GPIO_Port GPIOC
#define DCU_TEMP_SENSE_Pin GPIO_PIN_0
#define DCU_TEMP_SENSE_GPIO_Port GPIOA
#define MAIN_CURRENT_SENSE_Pin GPIO_PIN_3
#define MAIN_CURRENT_SENSE_GPIO_Port GPIOA
#define DCU_CURRENT_SENSE_Pin GPIO_PIN_4
#define DCU_CURRENT_SENSE_GPIO_Port GPIOA
#define ANALOG_AUX_3_Pin GPIO_PIN_5
#define ANALOG_AUX_3_GPIO_Port GPIOA
#define ANALOG_AUX_2_Pin GPIO_PIN_6
#define ANALOG_AUX_2_GPIO_Port GPIOA
#define ANALOG_AUX_1_Pin GPIO_PIN_0
#define ANALOG_AUX_1_GPIO_Port GPIOB
#define _5V_REG_SENSE_Pin GPIO_PIN_1
#define _5V_REG_SENSE_GPIO_Port GPIOB
#define DIGITAL_AUX_2_Pin GPIO_PIN_13
#define DIGITAL_AUX_2_GPIO_Port GPIOE
#define DIGITAL_AUX_3_Pin GPIO_PIN_14
#define DIGITAL_AUX_3_GPIO_Port GPIOE
#define DIGITAL_AUX_1_Pin GPIO_PIN_15
#define DIGITAL_AUX_1_GPIO_Port GPIOE
#define USB_HS_DM_Pin GPIO_PIN_14
#define USB_HS_DM_GPIO_Port GPIOB
#define USB_HS_DP_Pin GPIO_PIN_15
#define USB_HS_DP_GPIO_Port GPIOB
#define I2C_IMU_SCL_Pin GPIO_PIN_12
#define I2C_IMU_SCL_GPIO_Port GPIOD
#define I2C_IMU_SDA_Pin GPIO_PIN_13
#define I2C_IMU_SDA_GPIO_Port GPIOD
#define ETH_nRST_Pin GPIO_PIN_15
#define ETH_nRST_GPIO_Port GPIOD
#define IMU_nRESET_Pin GPIO_PIN_6
#define IMU_nRESET_GPIO_Port GPIOC
#define uSD_D0_Pin GPIO_PIN_8
#define uSD_D0_GPIO_Port GPIOC
#define uSD_D1_Pin GPIO_PIN_9
#define uSD_D1_GPIO_Port GPIOC
#define AUTOGEAR_SWTICH_MCU_Pin GPIO_PIN_8
#define AUTOGEAR_SWTICH_MCU_GPIO_Port GPIOA
#define AUTOGEAR_SWTICH_MCU_EXTI_IRQn EXTI9_5_IRQn
#define USB_5V_ENABLE_Pin GPIO_PIN_9
#define USB_5V_ENABLE_GPIO_Port GPIOA
#define USB_OVERCURRENT_Pin GPIO_PIN_10
#define USB_OVERCURRENT_GPIO_Port GPIOA
#define USB_OVERCURRENT_EXTI_IRQn EXTI15_10_IRQn
#define UART_XBEE_CTS_Pin GPIO_PIN_11
#define UART_XBEE_CTS_GPIO_Port GPIOA
#define UART_XBEE_RTS_Pin GPIO_PIN_12
#define UART_XBEE_RTS_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define uSD_DETECTED_Pin GPIO_PIN_15
#define uSD_DETECTED_GPIO_Port GPIOA
#define uSD_D2_Pin GPIO_PIN_10
#define uSD_D2_GPIO_Port GPIOC
#define uSD_D3_Pin GPIO_PIN_11
#define uSD_D3_GPIO_Port GPIOC
#define uSD_CK_Pin GPIO_PIN_12
#define uSD_CK_GPIO_Port GPIOC
#define LED_YELLOW_Pin GPIO_PIN_1
#define LED_YELLOW_GPIO_Port GPIOD
#define uSD_CMD_Pin GPIO_PIN_2
#define uSD_CMD_GPIO_Port GPIOD
#define LED_RED_Pin GPIO_PIN_3
#define LED_RED_GPIO_Port GPIOD
#define LED_GREEN_Pin GPIO_PIN_4
#define LED_GREEN_GPIO_Port GPIOD
#define USART_GPS_TX_Pin GPIO_PIN_5
#define USART_GPS_TX_GPIO_Port GPIOD
#define USART_GPS_RX_Pin GPIO_PIN_6
#define USART_GPS_RX_GPIO_Port GPIOD
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define UART_XBEE_TX_Pin GPIO_PIN_6
#define UART_XBEE_TX_GPIO_Port GPIOB
#define UART_XBEE_RX_Pin GPIO_PIN_7
#define UART_XBEE_RX_GPIO_Port GPIOB
#define CAN_RX_Pin GPIO_PIN_8
#define CAN_RX_GPIO_Port GPIOB
#define CAN_TX_Pin GPIO_PIN_9
#define CAN_TX_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
