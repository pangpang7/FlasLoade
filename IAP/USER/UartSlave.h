#ifndef __UART_SLAVE_H
#define __UART_SLAVE_H

#include "SI6600.h"

void UART1_port_init(PORT_TypeDef * PORTx);
void UART_Init(UART_TypeDef* UARTx);
void UART_x_Tx(UART_TypeDef* UARTx,u8* data, u32 lg);
void UART_x_Rx(UART_TypeDef* UARTx,u8* data, u32 lg);
void UartSlaveInit(void);
void UartSlaveProtocolProc(void);
#endif
