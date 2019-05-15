#include <stdint.h>
#include "data.h"

uint8_t dcuStateBuffer[BUFFER_STATE_LEN] = "[S;0;0;0;0;0;0;0;0]";
uint8_t blockBuffer [BUFFER_BLOCK_LEN] = "[C;000000;000.0;000.0;000.0;000.0;000;000;000;000;000;000;000;00.0;0;00000;000;000;000.0;000.0;0000.0;0;0;000;0;00000;00000;0.000;00000;0;0;000;000;00.00;00000;00.0;00.00;00000;00.0;00000;00.00;0000;00.00;00000;00000;00000;000;000;000;000;000;000;000;000;000;000;000;000;0000.00;0000.00;000.00;0000.00;0000.00;0000.00;000;000;0;0;0;000;000;00000;00000;00000;00000;00000;00000;000;0000;000;0000;000;0000;000;0000;000;0000000000;0000000;0000000;0000000;000000000000000000000000000000000000000000000000000000000000]";
uint8_t telemetryReceivedBuffer [BUFFER_COMMAND_LEN];
uint8_t setRtcReceivedBuffer [BUFFER_RTC_SET_LEN];