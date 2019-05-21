#ifndef __telemetry_H
#define __telemetry_H

extern inline void usart1TxCallback(void);
extern inline void usart1RxCallback(void);
extern inline void stateSendTimCallback(void);
extern inline void dataSendTimCallback(void);
extern inline void telemetryReceive(void);

#endif
