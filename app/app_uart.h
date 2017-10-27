/*
 * app_uart.h
 *
 *  Created on: 11/09/2017
 *      Author: OmarSevilla
 */

#ifndef APP_UART_H_
#define APP_UART_H_

#include "../Headers/stdtypedef.h"

extern void app_UART_Task(void);
extern void app_UART_Init(void);
extern void app_UART_SendDataTask(void);
extern void app_UART_SendByte(T_UBYTE lub_Data);
extern void app_UART_ReceiveByte(T_UBYTE lub_Data);
extern T_UBYTE app_UART_ReadReceiveData(void);
extern T_UBYTE app_UART_GetBytesToSend(void);
extern T_UBYTE app_UART_GetBytesReceived(void);
extern void UART0_IRQHandler_User(void);
extern void UART2_IRQHandler_User(void);

#endif /* APP_UART_H_ */
