#include <stdio.h>
#include <stdint.h>
#include "data.h"

uint8_t blockBuffer[BUFFER_BLOCK_LEN];
uint8_t dcuStateBuffer[BUFFER_STATE_LEN] = "[S;0;0;0;0;0;0;0;0]";

extern void dataBufferInit(void)
{
	for(uint16_t i = 0; i < (BUFFER_BLOCK_LEN - 1); i++) {
		blockBuffer[i] = '5';
	}
	
	blockBuffer[BUFFER_BLOCK_LEN - 1] = '\n';
	return;
}

extern inline uint8_t getUsbReadyState(void)
{
	return dcuStateBuffer[STATE_USB_READY_INDEX];
}

extern inline uint8_t getAcquisitionState(void)
{
	return dcuStateBuffer[STATE_ACQUISITION_ON_INDEX];
}

extern inline void setUsbPresentState(void)
{
	dcuStateBuffer[STATE_USB_PRESENT_INDEX] = STATE_ON;
	return;
}

extern inline void setUsbReadyState(void)
{
	dcuStateBuffer[STATE_USB_READY_INDEX] = STATE_ON;
	return;
}

extern inline void setAcquisitionState(void)
{
	dcuStateBuffer[STATE_ACQUISITION_ON_INDEX] = STATE_ON;
	return;
}

extern inline void resetUsbPresentState(void)
{
	dcuStateBuffer[STATE_USB_PRESENT_INDEX] = STATE_OFF;
	return;
}

extern inline void resetUsbReadyState(void)
{
	dcuStateBuffer[STATE_USB_READY_INDEX] = STATE_OFF;
	return;
}

extern inline void resetAcquisitionState(void)
{
	dcuStateBuffer[STATE_ACQUISITION_ON_INDEX] = STATE_OFF;
	return;
}
