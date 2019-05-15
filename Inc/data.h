#ifndef __data_H
#define __data_H

#define ACQUISITION_OFF_STATE 								(uint8_t)'1'
#define ACQUISITION_ON_FROM_SW_STATE					(uint8_t)'2'
#define ACQUISITION_ON_FROM_TELEMETRY_STATE 	(uint8_t)'3'
#define ACQUISITION_ON_FROM_AUTO_STATE 				(uint8_t)'4'
#define ACQUISITION_ON_FROM_DEBUG_STATE 			(uint8_t)'5'
#define ACQUISITION_ON_SW_REQUEST 						(uint8_t)'6'
#define ACQUISITION_ON_TELEMETRY_REQUEST 			(uint8_t)'7'
#define ACQUISITION_ON_AUTO_REQUEST 					(uint8_t)'8'
#define ACQUISITION_ON_DEBUG_REQUEST 					(uint8_t)'9'
#define ACQUISITION_OFF_SW_REQUEST 						(uint8_t)'A'
#define ACQUISITION_OFF_TELEMETRY_REQUEST 		(uint8_t)'B'
#define ACQUISITION_OFF_AUTO_REQUEST 					(uint8_t)'C'
#define ACQUISITION_OFF_DEBUG_REQUEST 				(uint8_t)'D'

#define IDLE_COMMAND													(uint8_t)'I'
#define ACQUISITION_ON_SW_COMMAND							(uint8_t)'S'
#define ACQUISITION_OFF_SW_COMMAND 						(uint8_t)'W'
#define ACQUISITION_ON_TELEMETRY_COMMAND 			(uint8_t)'T'
#define ACQUISITION_OFF_TELEMETRY_COMMAND 		(uint8_t)'M'
#define ACQUISITION_ON_ETH_COMMAND 						(uint8_t)'E'
#define ACQUISITION_OFF_ETH_COMMAND 					(uint8_t)'H'
#define ACQUISITION_ON_DEBUG_COMMAND 					(uint8_t)'D'
#define ACQUISITION_OFF_DEBUG_COMMAND 				(uint8_t)'B'
#define STATE_ON 															(uint8_t)'1'
#define STATE_OFF															(uint8_t)'0'

#define BUFFER_BLOCK_LEN 											(uint16_t)512
#define BUFFER_STATE_LEN 											(uint8_t)19
#define STATE_USB_PRESENT_INDEX 							(uint8_t)3
#define STATE_USB_READY_INDEX 								(uint8_t)5
#define STATE_uSD_PRESENT_INDEX 							(uint8_t)7
#define STATE_uSD_READY_INDEX 								(uint8_t)9
#define STATE_ACQUISITION_ON_INDEX 						(uint8_t)11
#define STATE_TELEMETRY_ON_INDEX  						(uint8_t)13

extern inline void startAcquisitionStateMachine(uint8_t startAcquisitionEvent);
extern inline uint8_t getUsbReadyState(void);
extern inline uint8_t getAcquisitionState(void);
extern inline void setUsbPresentState(void);
extern inline void setUsbReadyState(void);
extern inline void setAcquisitionState(void);
extern inline void resetUsbPresentState(void);
extern inline void resetUsbReadyState(void);
extern inline void resetAcquisitionState(void);
extern void dataBufferInit(void);
static inline void USB_OpenFile(void);
static inline void USB_CloseFile(void);

#endif
