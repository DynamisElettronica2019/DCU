#ifndef __data_H
#define __data_H

#include "can.h"

extern inline void canDataParser(CAN_RxPacketTypeDef *unpackedData);
extern void packetCounterReset(void);

#endif
