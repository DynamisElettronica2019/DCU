/**
  ******************************************************************************
  * File Name          : SDMMC.c
  * Description        : This file provides code for the configuration
  *                      of the SDMMC instances.
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
#include "sdmmc.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

SD_HandleTypeDef hsd1;
DMA_HandleTypeDef hdma_sdmmc1_rx;
DMA_HandleTypeDef hdma_sdmmc1_tx;

/* SDMMC1 init function */

void MX_SDMMC1_SD_Init(void)
{

  hsd1.Instance = SDMMC1;
  hsd1.Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING;
  hsd1.Init.ClockBypass = SDMMC_CLOCK_BYPASS_DISABLE;
  hsd1.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;
  hsd1.Init.BusWide = SDMMC_BUS_WIDE_1B;
  hsd1.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_ENABLE;
  hsd1.Init.ClockDiv = 0;

}

void HAL_SD_MspInit(SD_HandleTypeDef* sdHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(sdHandle->Instance==SDMMC1)
  {
  /* USER CODE BEGIN SDMMC1_MspInit 0 */

  /* USER CODE END SDMMC1_MspInit 0 */
    /* SDMMC1 clock enable */
    __HAL_RCC_SDMMC1_CLK_ENABLE();
  
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**SDMMC1 GPIO Configuration    
    PC8     ------> SDMMC1_D0
    PC9     ------> SDMMC1_D1
    PC10     ------> SDMMC1_D2
    PC11     ------> SDMMC1_D3
    PC12     ------> SDMMC1_CK
    PD2     ------> SDMMC1_CMD 
    */
    GPIO_InitStruct.Pin = uSD_D0_Pin|uSD_D1_Pin|uSD_D2_Pin|uSD_D3_Pin 
                          |uSD_CK_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_SDMMC1;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = uSD_CMD_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_SDMMC1;
    HAL_GPIO_Init(uSD_CMD_GPIO_Port, &GPIO_InitStruct);

    /* SDMMC1 DMA Init */
    /* SDMMC1_RX Init */
    hdma_sdmmc1_rx.Instance = DMA2_Stream6;
    hdma_sdmmc1_rx.Init.Channel = DMA_CHANNEL_4;
    hdma_sdmmc1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_sdmmc1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_sdmmc1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_sdmmc1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_sdmmc1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_sdmmc1_rx.Init.Mode = DMA_PFCTRL;
    hdma_sdmmc1_rx.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_sdmmc1_rx.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    hdma_sdmmc1_rx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    hdma_sdmmc1_rx.Init.MemBurst = DMA_MBURST_INC4;
    hdma_sdmmc1_rx.Init.PeriphBurst = DMA_PBURST_INC4;
    if (HAL_DMA_Init(&hdma_sdmmc1_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(sdHandle,hdmarx,hdma_sdmmc1_rx);

    /* SDMMC1_TX Init */
    hdma_sdmmc1_tx.Instance = DMA2_Stream3;
    hdma_sdmmc1_tx.Init.Channel = DMA_CHANNEL_4;
    hdma_sdmmc1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_sdmmc1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_sdmmc1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_sdmmc1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_sdmmc1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_sdmmc1_tx.Init.Mode = DMA_PFCTRL;
    hdma_sdmmc1_tx.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_sdmmc1_tx.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    hdma_sdmmc1_tx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    hdma_sdmmc1_tx.Init.MemBurst = DMA_MBURST_INC4;
    hdma_sdmmc1_tx.Init.PeriphBurst = DMA_PBURST_INC4;
    if (HAL_DMA_Init(&hdma_sdmmc1_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(sdHandle,hdmatx,hdma_sdmmc1_tx);

    /* SDMMC1 interrupt Init */
    HAL_NVIC_SetPriority(SDMMC1_IRQn, 10, 0);
    HAL_NVIC_EnableIRQ(SDMMC1_IRQn);
  /* USER CODE BEGIN SDMMC1_MspInit 1 */

  /* USER CODE END SDMMC1_MspInit 1 */
  }
}

void HAL_SD_MspDeInit(SD_HandleTypeDef* sdHandle)
{

  if(sdHandle->Instance==SDMMC1)
  {
  /* USER CODE BEGIN SDMMC1_MspDeInit 0 */

  /* USER CODE END SDMMC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SDMMC1_CLK_DISABLE();
  
    /**SDMMC1 GPIO Configuration    
    PC8     ------> SDMMC1_D0
    PC9     ------> SDMMC1_D1
    PC10     ------> SDMMC1_D2
    PC11     ------> SDMMC1_D3
    PC12     ------> SDMMC1_CK
    PD2     ------> SDMMC1_CMD 
    */
    HAL_GPIO_DeInit(GPIOC, uSD_D0_Pin|uSD_D1_Pin|uSD_D2_Pin|uSD_D3_Pin 
                          |uSD_CK_Pin);

    HAL_GPIO_DeInit(uSD_CMD_GPIO_Port, uSD_CMD_Pin);

    /* SDMMC1 DMA DeInit */
    HAL_DMA_DeInit(sdHandle->hdmarx);
    HAL_DMA_DeInit(sdHandle->hdmatx);

    /* SDMMC1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(SDMMC1_IRQn);
  /* USER CODE BEGIN SDMMC1_MspDeInit 1 */

  /* USER CODE END SDMMC1_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */

uint8_t SD_Init()
{
	FRESULT SD_Status;
	
	//SD_Status = BSP_SD_Init();
	
	 /* HAL SD initialization */
  SD_Status = (FRESULT)HAL_SD_Init(&hsd1);
  /* Configure SD Bus width (4 bits mode selected) */
  if (SD_Status == MSD_OK)
  {
    /* Enable wide operation */
    if (HAL_SD_ConfigWideBusOperation(&hsd1, SDMMC_BUS_WIDE_4B) != HAL_OK)
    {
      SD_Status = (FRESULT)MSD_ERROR;
			SD_onError(SD_Status);
			return SD_Status;
    }
  }
	
	if(SD_Status != FR_OK)
	{
		SD_onError(SD_Status);
		return SD_Status;
	}	
	SD_Status = f_mount(&SDFatFS, SDPath, 1);
	
	if(SD_Status != FR_OK)
	{		
		if(SD_Status == FR_NO_FILESYSTEM)
		{
			/*uint8_t SDWorkBuffer[_MAX_SS];
			
			SD_Status = f_mkfs((TCHAR const*)SDPath, FM_FAT, 0, SDWorkBuffer, sizeof(SDWorkBuffer));
			if(SD_Status != FR_OK)
			{*/
				SD_onError(SD_Status);	
				return SD_Status;
			//}
		}
	}	
	
	return SD_Status;
}

uint8_t SD_openFile(uint8_t* fileName, FIL* MyFile)
{
	FRESULT SD_Status;
	
	SD_Status = f_open(MyFile, (const TCHAR*)fileName, FA_OPEN_APPEND|FA_WRITE|FA_READ);
	
	if(SD_Status != FR_OK)
	{
		SD_onError(SD_Status);
		return SD_Status;	
	}
	
	return SD_Status;
}

uint8_t SD_refreshFile(uint8_t* fileName, FIL* MyFile)
{
	FRESULT SD_Status;
	
	SD_Status = f_close(MyFile);
	
	if(SD_Status != FR_OK)
	{
		SD_onError(SD_Status);
		return SD_Status;	
	}
	
	SD_Status = f_open(MyFile, (const TCHAR*)fileName, FA_OPEN_APPEND|FA_WRITE|FA_READ);
	
	if(SD_Status != FR_OK)
	{
		SD_onError(SD_Status);
		return SD_Status;	
	}
	
	return SD_Status;
}

uint8_t SD_writeString(uint8_t *writeBuffer, uint32_t bufferLen, FIL* MyFile)
{
	FRESULT SD_Status;
	uint32_t writtenBytes = 0;
	
	SD_Status = f_write(MyFile, writeBuffer, bufferLen, (void*)&writtenBytes);
	
	if(SD_Status != FR_OK)
	{
		SD_onError(SD_Status);
		return SD_Status;	
	}
	else if(writtenBytes != bufferLen)
	{
		SD_onError(SD_Status);
		return SD_Status;	
	}
	
	return SD_Status;
}

void SD_onError(FRESULT code)
{
	HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);			
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
