#include <stdio.h>
#include <stdint.h>
#include "data.h"

uint8_t blockBuffer[BUFFER_BLOCK_LEN];
uint8_t dcuStateBuffer[BUFFER_STATE_LEN];

extern void dataBufferInit(void)
{
	for(uint16_t i = 0; i < (BUFFER_BLOCK_LEN - 1); i++) {
		blockBuffer[i] = '5';
	}
	
	blockBuffer[BUFFER_BLOCK_LEN - 1] = '\n';
	return;
}

extern void dcuStateBufferInit(void)
{
	sprintf((char *)dcuStateBuffer, "[S;1;0;0;0;0;0;0;0]");
	return;
}
