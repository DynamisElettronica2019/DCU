#include "timestamp.h"
#include "data.h"

uint32_t dataTimestamp = 0;			/* Data timestamp variable, in ms*/


extern inline uint32_t getDataTimestamp(void)
{
	return dataTimestamp;
}

extern inline void incrementDataTimestamp(void)
{
		dataTimestamp = dataTimestamp + 10;
}

extern inline void resetDataTimestamp(void)
{
	dataTimestamp = 0;
}
