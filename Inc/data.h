#ifndef __data_H
#define __data_H

#define ACQUISITION_ON_SW_COMMAND						(uint8_t)'S'
#define ACQUISITION_OFF_SW_COMMAND 					(uint8_t)'W'
#define ACQUISITION_ON_TELEMETRY_COMMAND 		(uint8_t)'T'
#define ACQUISITION_OFF_TELEMETRY_COMMAND 	(uint8_t)'M'
#define ACQUISITION_ON_ETH_COMMAND 					(uint8_t)'E'
#define ACQUISITION_OFF_ETH_COMMAND 				(uint8_t)'H'
#define ACQUISITION_ON_DEBUG_COMMAND 				(uint8_t)'D'
#define ACQUISITION_OFF_DEBUG_COMMAND 			(uint8_t)'B'

#define BUFFER_BLOCK_LEN 						(uint16_t)512
#define BUFFER_STATE_LEN 						(uint8_t)19
#define STATE_USB_PRESENT_INDEX 		(uint8_t)3
#define STATE_USB_READY_INDEX 			(uint8_t)5
#define STATE_uSD_PRESENT_INDEX 		(uint8_t)7
#define STATE_uSD_READY_INDEX 			(uint8_t)9
#define STATE_ACQUISITION_ON 				(uint8_t)11
#define STATE_TELEMETRY_ON 					(uint8_t)13
#define STATE_ON 										(uint8_t)1
#define STATE_OFF										(uint8_t)0

extern void dataBufferInit(void);
extern void dcuStateBufferInit(void);

#endif
