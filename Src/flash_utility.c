#include "flash_utility.h"
#include <stdint.h>


extern void Flash_EraseSector(void)
{
	HAL_FLASH_Unlock();
	FLASH_Erase_Sector(FLASH_CALIBRATION_SECTOR_USED, FLASH_VOLTAGE_RANGE_3);
	HAL_FLASH_Lock();
}

extern void Flash_Write(uint8_t sector_offset, uint16_t data)
{
	uint32_t adress;
	
	adress = FLASH_CALIBRATION_ADDRESS_USED + sector_offset;
	HAL_FLASH_Unlock();
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, adress, data);
	HAL_FLASH_Lock();
}

extern void Flash_Save_Calibration(OffsetHandler_t *OffsetHandler)
{
	Flash_EraseSector();
	HAL_FLASH_Unlock();
	Flash_Write(DATA_ARE_CALIBRATED_SECTOR_ADDRESS_OFFSET, (uint16_t) 666);
	Flash_Write(APPS_FULL_SECTOR_ADDRESS_OFFSET, OffsetHandler->DATA_APPS_FullCalibrationOffset);
	Flash_Write(APPS_ZERO_SECTOR_ADDRESS_OFFSET, OffsetHandler->DATA_APPS_ZeroCalibrationOffset);
	Flash_Write(LINEAR_FL_SECTOR_ADDRESS_OFFSET, OffsetHandler->DATA_LINEAR_FL_CalibrationOffset);
	Flash_Write(LINEAR_FR_SECTOR_ADDRESS_OFFSET, OffsetHandler->DATA_LINEAR_FR_CalibrationOffset);
	Flash_Write(LINEAR_RL_SECTOR_ADDRESS_OFFSET, OffsetHandler->DATA_LINEAR_RL_CalibrationOffset);
	Flash_Write(LINEAR_RR_SECTOR_ADDRESS_OFFSET, OffsetHandler->DATA_LINEAR_RR_CalibrationOffset);
	Flash_Write(LOAD_CELL_FL_SECTOR_ADDRESS_OFFSET, OffsetHandler->DATA_LOAD_CELL_FL_CalibrationOffset);
	Flash_Write(LOAD_CELL_FR_SECTOR_ADDRESS_OFFSET, OffsetHandler->DATA_LOAD_CELL_FR_CalibrationOffset);
	Flash_Write(LOAD_CELL_RL_SECTOR_ADDRESS_OFFSET, OffsetHandler->DATA_LOAD_CELL_RL_CalibrationOffset);
	Flash_Write(LOAD_CELL_RR_SECTOR_ADDRESS_OFFSET, OffsetHandler->DATA_LOAD_CELL_RR_CalibrationOffset);
	HAL_FLASH_Lock();
}

extern void Flash_LoadCalibration(OffsetHandler_t *OffsetHandler)
{
	if (*((uint32_t *)FLASH_CALIBRATION_ADDRESS_USED + DATA_ARE_CALIBRATED_SECTOR_ADDRESS_OFFSET) == 666){
		OffsetHandler->DATA_APPS_FullCalibrationOffset = *((uint32_t *)FLASH_CALIBRATION_ADDRESS_USED + APPS_FULL_SECTOR_ADDRESS_OFFSET);
		OffsetHandler->DATA_APPS_ZeroCalibrationOffset = *((uint32_t *)FLASH_CALIBRATION_ADDRESS_USED + APPS_ZERO_SECTOR_ADDRESS_OFFSET);
		OffsetHandler->DATA_LINEAR_FL_CalibrationOffset = *((uint32_t *)FLASH_CALIBRATION_ADDRESS_USED + LINEAR_FL_SECTOR_ADDRESS_OFFSET);
		OffsetHandler->DATA_LINEAR_FR_CalibrationOffset = *((uint32_t *)FLASH_CALIBRATION_ADDRESS_USED + LINEAR_FR_SECTOR_ADDRESS_OFFSET);
		OffsetHandler->DATA_LINEAR_RL_CalibrationOffset = *((uint32_t *)FLASH_CALIBRATION_ADDRESS_USED + LINEAR_RL_SECTOR_ADDRESS_OFFSET);
		OffsetHandler->DATA_LINEAR_RR_CalibrationOffset = *((uint32_t *)FLASH_CALIBRATION_ADDRESS_USED + LINEAR_RR_SECTOR_ADDRESS_OFFSET);
		OffsetHandler->DATA_LOAD_CELL_FL_CalibrationOffset = *((uint32_t *)FLASH_CALIBRATION_ADDRESS_USED + LOAD_CELL_FL_SECTOR_ADDRESS_OFFSET);
		OffsetHandler->DATA_LOAD_CELL_FR_CalibrationOffset = *((uint32_t *)FLASH_CALIBRATION_ADDRESS_USED + LOAD_CELL_FR_SECTOR_ADDRESS_OFFSET);
		OffsetHandler->DATA_LOAD_CELL_RL_CalibrationOffset = *((uint32_t *)FLASH_CALIBRATION_ADDRESS_USED + LOAD_CELL_RL_SECTOR_ADDRESS_OFFSET);
		OffsetHandler->DATA_LOAD_CELL_RR_CalibrationOffset = *((uint32_t *)FLASH_CALIBRATION_SECTOR_USED + LOAD_CELL_RL_SECTOR_ADDRESS_OFFSET);
		}
	else {
		OffsetHandler->DATA_APPS_FullCalibrationOffset = 1;
		OffsetHandler->DATA_APPS_ZeroCalibrationOffset = 0;
		OffsetHandler->DATA_LINEAR_FL_CalibrationOffset = 0;
		OffsetHandler->DATA_LINEAR_FR_CalibrationOffset = 0;
		OffsetHandler->DATA_LINEAR_RL_CalibrationOffset = 0;
		OffsetHandler->DATA_LINEAR_RR_CalibrationOffset = 0;
		OffsetHandler->DATA_LOAD_CELL_FL_CalibrationOffset = 2047;
		OffsetHandler->DATA_LOAD_CELL_FR_CalibrationOffset = 2047;
		OffsetHandler->DATA_LOAD_CELL_RL_CalibrationOffset = 2047;
		OffsetHandler->DATA_LOAD_CELL_RR_CalibrationOffset = 2047;
		
	}
}
