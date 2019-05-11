#include <stdint.h>
#include "data.h"

uint8_t blockBuffer[BUFFER_BLOCK_LEN];

extern void dataBufferInit(void)
{
	for(uint16_t i = 0; i < (BUFFER_BLOCK_LEN - 1); i++) {
		blockBuffer[i] = '5';
	}
	
	blockBuffer[BUFFER_BLOCK_LEN - 1] = '\n';
	return;
}
