/*
* Defines for usart buffer lenght and usart related functions
*/

#define BUFFER_BLOCK_LEN            ((uint16_t)512)
#define BUFFER_LEN            			((uint8_t)3)
#define BUFFER_STATE_LEN            ((uint16_t)19)
#define BUFFER_DEBUG_LEN            ((uint16_t)94)
#define BUFFER_ERROR_LEN            ((uint16_t)14)
#define BUFFER_COMMAND_LEN          ((uint8_t)4)
#define BUFFER_RTC_SET_LEN          ((uint8_t)20)
#define ERROR_MSG_LEN               ((uint8_t)4)
#define COMMAND_ACK_MSG_LEN         ((uint8_t)4)
#define ERROR_MSG_IDENTIFIER_POS    ((uint8_t)2)
#define COMMAND_ACK_IDENTIFIER_POS  ((uint8_t)2)

/*
* Command and error messages identifiers
*/

#define ACK_MSG                     "[A-]"
#define ERROR_MSG                   "[E-]"
#define MESSAGE_INIT_ID             ((uint8_t)'[')
#define MESSAGE_END_ID              ((uint8_t)']')
#define RESET_TELEM_ID              ((uint8_t)'M')
#define START_ACQ_ID                ((uint8_t)'B')
#define STOP_ACQ_ID                 ((uint8_t)'V')
#define GIVE_RTC_TIME_ID            ((uint8_t)'J')
#define GIVE_RTC_DATE_ID            ((uint8_t)'H')
#define SET_RTC_ID                  ((uint8_t)'R')
#define CMD_READ_ERR_ID          		((uint8_t)'B')
