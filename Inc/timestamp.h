#ifndef __timestamp_H
#define __timestamp_H

#include <stdint.h>

extern inline uint32_t getDataTimestamp(void);
extern inline void incrementDataTimestamp(void);
extern inline void resetDataTimestamp(void);

#endif
