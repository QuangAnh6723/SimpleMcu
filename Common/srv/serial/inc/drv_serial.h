/*
 * serial.h
 *
 *  Created on: Apr 25, 2026
 *      Author: AnhLe
 */

#ifndef DRV_SERIAL_INC_SERIAL_H_
#define DRV_SERIAL_INC_SERIAL_H_

#include "myInclude.h"

#if 	DEBUG_SRV_SERIAL
#define SERIAL_INF(fmt,...)			printf("[Serial]"fmt"\r\n" ##__VA_ARGS__)
#else
#define SERIAL_INF(fmt,...)
#endif

#define FLAG_HAVE_DATA		(1 << 0)

#define SERIAL_FLAG_WAIT	FLAG_HAVE_DATA


typedef struct
{
	myError_t (*send) (uint8_t data[], uint16_t len, uint32_t timeout);
	myError_t (*recv) (uint8_t data[], uint16_t len, uint32_t timeout);
} SerialPer_t;

typedef struct
{
	myError_t (*setFlag)(uint32_t flag);
} SerialApi_t;

typedef struct SerialCtx_t
{
	UART_HandleTypeDef *huart;
	SerialPer_t	per;
	SerialApi_t api;
} SerialCtx_t;

myError_t serial_init(UART_HandleTypeDef *huart, SerialApi_t *api);

myError_t serial_sendHelloMsg();
uint16_t  serial_getALLData(uint8_t data[]);
myError_t serial_send(uint8_t data[], uint16_t len);

void serial_putData(uint8_t data[], uint16_t len);

#endif /* DRV_SERIAL_INC_SERIAL_H_ */
