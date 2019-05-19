#ifndef __telemetry_H
#define __telemetry_H

extern inline void usart1TxCallback(void);
extern inline void usart1RxCallback(void);
extern inline void stateSendTimCallback(void);
extern inline void dataSendTimCallback(void);

uint16_t encodeString(uint8_t* string, uint8_t* outString, uint16_t len);
uint8_t setUpperNibble(uint8_t editByte, uint8_t nibble);
uint8_t setLowerNibble(uint8_t editByte, uint8_t nibble);

#endif
