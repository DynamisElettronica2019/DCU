#ifndef __telemetry_H
#define __telemetry_H

#include "data.h"

#define BUFFER_COMMAND_LEN          	(uint8_t)4
#define BUFFER_RTC_SET_LEN          	(uint8_t)20
#define ERROR_MSG_LEN               	(uint8_t)4
#define COMMAND_ACK_MSG_LEN         	(uint8_t)4
#define ERROR_MSG_IDENTIFIER_POS    	(uint8_t)2
#define COMMAND_ACK_IDENTIFIER_POS  	(uint8_t)2
#define HALF_DATA_INDEX  							(BUFFER_BLOCK_LEN / 2)

#define ACK_MSG                     	"W3-g"
#define ERROR_MSG                   	"W4-g"
#define SECOND_HALF_TX_FLAG         	(uint8_t)'S'
#define NORMAL_MODE_TX_FLAG         	(uint8_t)'N'
#define MESSAGE_INIT_ID             	(uint8_t)'['
#define MESSAGE_END_ID              	(uint8_t)']'
#define SET_TELEM_ID              		(uint8_t)'A'
#define RESET_TELEM_ID              	(uint8_t)'M'
#define START_ACQ_ID                	(uint8_t)'B'
#define STOP_ACQ_ID                 	(uint8_t)'V'
#define GIVE_RTC_TIME_ID            	(uint8_t)'J'
#define GIVE_RTC_DATE_ID            	(uint8_t)'H'
#define SET_RTC_ID                  	(uint8_t)'R'
#define CMD_READ_ERR_ID          			(uint8_t)'B'
#define UART1_CLEAR_FLAG							(uint8_t)'C'

/* Encoding defines */
#define ENCODE_0                     0x0
#define ENCODE_1                     0x1
#define ENCODE_2                     0x2
#define ENCODE_3                     0x3
#define ENCODE_4                     0x4
#define ENCODE_5                     0xC
#define ENCODE_6                     0xD
#define ENCODE_7                     0xF
#define ENCODE_8                     0x8
#define ENCODE_9                     0x9
#define ENCODE_SEMICOLON             0xA
#define ENCODE_COLON                 0xB
#define ENCODE_OPEN_SQUARE           0x5
#define ENCODE_CLOSE_SQUARE          0x6
#define ENCODE_BLANK                 0x7
#define ENCODE_MINUS                 0xE

#define DATA_MESSAGE_ID							 (uint8_t)'1'
#define SECOND_DATA_MESSAGE_ID			 (uint8_t)'N'
#define STATE_MESSAGE_ID						 (uint8_t)'2'
#define COMMAND_MESSAGE_ID					 (uint8_t)'3'
#define ERROR_MESSAGE_ID						 (uint8_t)'4'

extern inline void TELEMETRY_StateSendTimCallback(void);
extern inline void TELEMETRY_DataSendTimCallback(void);
extern inline void TELEMETRY_Receive(void);
uint16_t encodeString(uint8_t* string, uint8_t* outString, uint16_t len, uint8_t messageType);
uint8_t setUpperNibble(uint8_t editByte, uint8_t nibble);
uint8_t setLowerNibble(uint8_t editByte, uint8_t nibble);

#endif
