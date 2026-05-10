/*
 * serial.c
 *
 *  Created on: Apr 25, 2026
 *      Author: AnhLe
 */

#include <srv/serial/inc/drv_serial.h>
#include "drv/myDrivers/uart.h"
#include "misc/ringbuf/ringbuffer.h"
#include "myInclude.h"

static uint8_t 		gUartRxBuffer[1024];
static RingBuffer_t uartRingRx;
static SerialCtx_t 	serialCtx;

void serial_rx_callback(UART_HandleTypeDef *huart, uint16_t Size)
{
	SERIAL_INF(" ==> rx callback");

	if (rb_write(&uartRingRx, gUartRxBuffer, Size) != RB_OK)
	{
		SERIAL_INF("write to buffer err");
	}

	serialCtx.api.setFlag(FLAG_HAVE_DATA);
	SERIAL_INF("write to buffer ok");

	if (HAL_UARTEx_ReceiveToIdle_DMA(huart, gUartRxBuffer, sizeof(gUartRxBuffer)) != HAL_OK )
	{
		SERIAL_INF("err start DMA");
	}
}

static myError_t send(uint8_t data[], uint16_t len, uint32_t timeout)
{
	if ((serialCtx.huart == NULL) || (data == NULL) || (len == 0U))
	{
		return MY_ERR;
	}

	if (HAL_UART_Transmit(serialCtx.huart, data, len, timeout) != HAL_OK)
	{
		return MY_ERR;
	}

	return MY_OK;
}

static myError_t recv(uint8_t data[], uint16_t len, uint32_t timeout)
{
	if ((serialCtx.huart == NULL) || (data == NULL) || (len == 0U))
	{
		return MY_ERR;
	}

	if (HAL_UART_Receive(serialCtx.huart, data, len, timeout) != HAL_OK)
	{
		return MY_ERR;
	}

	return MY_OK;
}

myError_t serial_init(UART_HandleTypeDef *huart, SerialApi_t *api)
{
	if (huart == NULL)
	{
		return MY_ERR;
	}

	serialCtx.huart    = huart;
	serialCtx.per.send = send;
	serialCtx.per.recv = recv;

	serialCtx.api.setFlag = api->setFlag;

	rb_init(&uartRingRx);

	if (MY_OK != uart_registerCallback(huart, &serial_rx_callback))
	{
		SERIAL_INF("err register callback");
		return MY_ERR;
	}

	if (HAL_UARTEx_ReceiveToIdle_DMA(huart, gUartRxBuffer, sizeof(gUartRxBuffer)) != HAL_OK )
	{
		SERIAL_INF("err start DMA");
		return MY_ERR;
	}

	return MY_OK;
}

myError_t serial_sendHelloMsg()
{
	uint8_t msg[] = "serial hello nha\r\n";
	SerialPer_t *pSerialPer = &serialCtx.per;

	if (pSerialPer->send == NULL)
	{
		return MY_ERR;
	}

	return pSerialPer->send(msg, sizeof(msg) - 1U, 100);
}

void serial_putData(uint8_t data[], uint16_t len)
{
	if (rb_write(&uartRingRx, data, len) != RB_OK)
	{
		SERIAL_INF("write to buffer err");
	}
}

uint16_t serial_getALLData(uint8_t data[])
{
	uint16_t size = uartRingRx.count;
	if (rb_read(&uartRingRx, data, size) != 0)
	{
		return 0;
	}
	return size;
}

myError_t serial_send(uint8_t data[], uint16_t len)
{
	SerialPer_t *pSerialPer = &serialCtx.per;

	if (pSerialPer->send == NULL)
	{
		return MY_ERR;
	}
	return pSerialPer->send(data, len, 100);
}
