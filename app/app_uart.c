/*
 * app_uart.c
 *
 *  Created on: 11/09/2017
 *      Author: OmarSevilla
 */

#include "app_uart.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define ECHO_BUFFER_LENGTH 1

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile static T_UBYTE rub_UartBytesToSend = 0;
volatile static T_UBYTE rub_UartBytesReceived = 0;
volatile static T_UBYTE raub_SendBytes[16];
volatile static T_UBYTE raub_ReceiveBytes[16];

void app_UART_Task(void)
{
	return;
}

void app_UART_Init(void)
{
	return;
}

void app_UART_SendByte(T_UBYTE lub_Data)
{
	if(rub_UartBytesToSend < 16U)
	{
		rub_UartBytesToSend++;
		raub_SendBytes[rub_UartBytesToSend - 1] = lub_Data;
	}
	else
	{
		//Data cannot be send
		/* Do Nothing */
	}
}

void app_UART_ReceiveByte(T_UBYTE lub_Data)
{
	if(rub_UartBytesReceived < 16U)
	{
		rub_UartBytesReceived++;
		raub_ReceiveBytes[rub_UartBytesReceived - 1] = lub_Data;
	}
	else
	{
		//Cannot be received
		/* Do nothing */
	}
}

T_UBYTE app_UART_ReadReceiveData(void)
{
	T_UBYTE lub_Data;

	if(rub_UartBytesReceived > 0)
	{
		/* Get first input */
		lub_Data = raub_ReceiveBytes[0];
		rub_UartBytesReceived--;

		/* Re order buffer */
		for(T_UBYTE i = 0; i < rub_UartBytesReceived; i++)
		{
			raub_ReceiveBytes[i] = raub_ReceiveBytes[i + 1];
			raub_ReceiveBytes[i + 1] = 0U;
		}
	}
	else
	{
		lub_Data = 0x5A;
	}

	return lub_Data;
}

void app_UART_SendDataTask(void)
{
	/* Send data only when UART TX register is empty and ring buffer has data to send out. */
	if (/*TODO:Check TX Flags && */(rub_UartBytesToSend > 0U))
	{
		/* TODO: Write into the TX Buffer the next data from raub_SendBytes */
		rub_UartBytesToSend--;

		/* Re order buffer */
		for(T_UBYTE i = 0; i < rub_UartBytesToSend; i++)
		{
			raub_SendBytes[i] = raub_SendBytes[i + 1];
			raub_SendBytes[i + 1] = 0U;
		}
	}
	else
	{
		if(rub_UartBytesToSend == 0U)
		{
			/* TODO: Disable TX Interrupts */
		}
	}
}

T_UBYTE app_UART_GetBytesToSend(void)
{
	return rub_UartBytesToSend;
}

T_UBYTE app_UART_GetBytesReceived(void)
{
	return rub_UartBytesReceived;
}

/**********************************************************
 * Name: UART0_IRQHandler_User
 * Description: TBD
 **********************************************************/
void UART0_IRQHandler_User(void)
{
	app_UART_SendDataTask();
	/* If new data arrived. */
	if (/*TODO: Read RX Flags*/FALSE)
	{
		/* If ring buffer is not full, add data to ring buffer. */
		app_UART_ReceiveByte(FALSE);

	}
	else
	{

	}
}
