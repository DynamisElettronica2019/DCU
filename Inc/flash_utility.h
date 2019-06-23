#ifndef __flash_utility_H
#define __flash_utility_H

#include "stm32f7xx_hal.h"

#define DCU_IS_CALIBRATED_CODE											(uint32_t)0x00000666
#define FLASH_CALIBRATION_SECTOR_USED  							FLASH_SECTOR_11
#define FLASH_CALIBRATION_ADDRESS_USED 							SECTOR_11_BEGIN_ADDRESS_AXIM
#define SECTOR_11_BEGIN_ADDRESS_AXIM								(uint32_t)0x081C0000		/* AXIM interface */
#define SECTOR_11_END_ADDRESS_AXIM									(uint32_t)0x081FFFFF		/* AXIM interface */
#define SECTOR_11_BEGIN_ADDRESS_ICTM								(uint32_t)0x003C0000		/* ICTM interface */
#define SECTOR_11_END_ADDRESS_ICTM									(uint32_t)0x003FFFFF		/* ICTM interface */

#define DATA_ARE_CALIBRATED_SECTOR_ADDRESS_OFFSET		(uint32_t)0x00000000
#define APPS_FULL_SECTOR_ADDRESS_OFFSET 						(uint32_t)0x00000004
#define APPS_ZERO_SECTOR_ADDRESS_OFFSET 						(uint32_t)0x00000008
#define LINEAR_FL_SECTOR_ADDRESS_OFFSET 						(uint32_t)0x0000000C
#define LINEAR_FR_SECTOR_ADDRESS_OFFSET 						(uint32_t)0x00000010
#define LINEAR_RL_SECTOR_ADDRESS_OFFSET 						(uint32_t)0x00000014
#define LINEAR_RR_SECTOR_ADDRESS_OFFSET 						(uint32_t)0x00000018
#define LOAD_CELL_FL_SECTOR_ADDRESS_OFFSET 					(uint32_t)0x0000001C
#define LOAD_CELL_FR_SECTOR_ADDRESS_OFFSET 					(uint32_t)0x00000020
#define LOAD_CELL_RL_SECTOR_ADDRESS_OFFSET 					(uint32_t)0x00000024
#define LOAD_CELL_RR_SECTOR_ADDRESS_OFFSET 					(uint32_t)0x00000028

typedef struct OffsetHandler {
	uint16_t DATA_LOAD_CELL_FR_CalibrationOffset;
	uint16_t DATA_LOAD_CELL_FL_CalibrationOffset;
	uint16_t DATA_LOAD_CELL_RR_CalibrationOffset;
	uint16_t DATA_LOAD_CELL_RL_CalibrationOffset;
	uint16_t DATA_LINEAR_FR_CalibrationOffset;
	uint16_t DATA_LINEAR_FL_CalibrationOffset;
	uint16_t DATA_LINEAR_RR_CalibrationOffset;
	uint16_t DATA_LINEAR_RL_CalibrationOffset;
	uint16_t DATA_APPS_ZeroCalibrationOffset;
	uint16_t DATA_APPS_FullCalibrationOffset;
} OffsetHandler_t;

extern void Flash_EraseSector(void);
extern void Flash_Write(uint8_t sector_offset, uint32_t data);
extern void Flash_Save_Calibration(OffsetHandler_t *OffsetHandler);
extern void Flash_LoadCalibration(OffsetHandler_t *OffsetHandler);

#endif
