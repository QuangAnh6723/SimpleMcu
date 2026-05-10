/*
 * uart.h
 *
 *  Created on: Apr 25, 2026
 *      Author: AnhLe
 */

#ifndef DRV_MYDRIVERS_UART_H_
#define DRV_MYDRIVERS_UART_H_

#include "myInclude.h"

#define UART_MAX_HANDLE		3

typedef void (*UartCallback) (UART_HandleTypeDef *huart, uint16_t Size);

typedef struct
{
	UART_HandleTypeDef *huart;
	UartCallback 		handler;
} UartHandle_t;



myError_t uart_registerCallback(UART_HandleTypeDef *huart, UartCallback callback);

#endif /* DRV_MYDRIVERS_UART_H_ */
