/*
 * uart.c
 *
 *  Created on: Apr 25, 2026
 *      Author: AnhLe
 */

#include "myInclude.h"
#include "uart.h"

static UartHandle_t uartHandleList[UART_MAX_HANDLE];
uint8_t count = 0;
myError_t uart_registerCallback(UART_HandleTypeDef *huart, UartCallback callback)
{
	if (count > UART_MAX_HANDLE || callback == NULL)
	{
		return MY_ERR;
	}

	uartHandleList[count].huart 	= huart;
	uartHandleList[count].handler 	= callback;
	count++;

	return MY_OK;
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	for (uint8_t i = 0; i < count; i++)
	{
		if (uartHandleList[i].huart->Instance == huart->Instance)
		{
			if (uartHandleList[i].handler != NULL)
			{
				uartHandleList[i].handler(huart, Size);
			}
			break;
		}
	}
}
