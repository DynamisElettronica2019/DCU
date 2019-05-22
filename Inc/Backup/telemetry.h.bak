#ifndef __telemetry_H
#define __telemetry_H

#define BUFFER_COMMAND_LEN          													(uint8_t)4
#define BUFFER_RTC_SET_LEN          													(uint8_t)20
#define ERROR_MSG_LEN               													(uint8_t)4
#define COMMAND_ACK_MSG_LEN         													(uint8_t)4
#define ERROR_MSG_IDENTIFIER_POS    													(uint8_t)2
#define COMMAND_ACK_IDENTIFIER_POS  													(uint8_t)2
#define HALF_DATA_INDEX  																			(uint16_t)256

#define ACK_MSG                     													"[A-]"
#define ERROR_MSG                   													"[E-]"
#define SECOND_HALF_TX_FLAG         													(uint8_t)'S'
#define NORMAL_MODE_TX_FLAG         													(uint8_t)'N'
#define MESSAGE_INIT_ID             													(uint8_t)'['
#define MESSAGE_END_ID              													(uint8_t)']'
#define RESET_TELEM_ID              													(uint8_t)'M'
#define START_ACQ_ID                													(uint8_t)'B'
#define STOP_ACQ_ID                 													(uint8_t)'V'
#define GIVE_RTC_TIME_ID            													(uint8_t)'J'
#define GIVE_RTC_DATE_ID            													(uint8_t)'H'
#define SET_RTC_ID                  													(uint8_t)'R'
#define CMD_READ_ERR_ID          															(uint8_t)'B'
#define UART1_CLEAR_FLAG																			(uint8_t)'C'

extern inline void TELEMETRY_StateSendTimCallback(void);
extern inline void TELEMETRY_DataSendTimCallback(void);
extern inline void TELEMETRY_Receive(void);

#endif
