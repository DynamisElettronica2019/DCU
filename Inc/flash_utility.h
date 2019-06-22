#ifndef __flash_utility_H
#define __flash_utility_H

#include "stm32f7xx_hal.h"

#define FLASH_CALIBRATION_SECTOR_USED  			FLASH_SECTOR_11
#define FLASH_CALIBRATION_ADDRESS_USED 			SECTOR_11_BEGIN_ADDRESS_ICTM
#define SECTOR_11_BEGIN_ADDRESS_AXIM				(uint32_t)0x081C0000		/* AXIM interface */
#define SECTOR_11_END_ADDRESS_AXIM					(uint32_t)0x081FFFFF		/* AXIM interface */
#define SECTOR_11_BEGIN_ADDRESS_ICTM				(uint32_t)0x003C0000		/* ICTM interface */
#define SECTOR_11_END_ADDRESS_ICTM					(uint32_t)0x003FFFFF		/* ICTM interface */
#define APPS_FULL_SECTOR_ADDRESS_OFFSET 		(uint32_t)0
#define APPS_ZERO_SECTOR_ADDRESS_OFFSET 		(uint32_t)2
#define LINEAR_FL_SECTOR_ADDRESS_OFFSET 		(uint32_t)4
#define LINEAR_FR_SECTOR_ADDRESS_OFFSET 		(uint32_t)6
#define LINEAR_RL_SECTOR_ADDRESS_OFFSET 		(uint32_t)8
#define LINEAR_RR_SECTOR_ADDRESS_OFFSET 		(uint32_t)10
#define LOAD_CELL_FL_SECTOR_ADDRESS_OFFSET 	(uint32_t)12
#define LOAD_CELL_FR_SECTOR_ADDRESS_OFFSET 	(uint32_t)14
#define LOAD_CELL_RL_SECTOR_ADDRESS_OFFSET 	(uint32_t)16
#define LOAD_CELL_RR_SECTOR_ADDRESS_OFFSET 	(uint32_t)18

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
extern void Flash_Write(uint8_t sector_offset, uint16_t data);
extern void Flash_Save_Calibration(OffsetHandler_t *OffsetHandler);
extern void Flash_LoadCalibration(OffsetHandler_t *OffsetHandler);

#endif
