/*
 * srv_serial.c
 *
 *  Created on: Apr 25, 2026
 *      Author: AnhLe
 */

#include "myInclude.h"

#include "../inc/drv_serial.h"
#include "../inc/srv_serial.h"
#include "../inc/serial_payload.h"
#include "../inc/serial_protocol.h"
#include "../inc/serial_handler.h"

#include "../inc/srv_serial_test.h"

#include "string.h"

extern UART_HandleTypeDef huart1;

static osEventFlagsId_t event_id;

static osThreadId_t 		serialThreadHandle;
static const osThreadAttr_t thread_attributes =
{
  .name = "serial task",
  .stack_size = 128 * 20,
  .priority = (osPriority_t) osPriorityNormal,
};

static SerialQueueHandle_t serialQueueHandle;

static uint8_t tempBuf[SERIAL_PAKET_SIZE_BUF_MAX];

myError_t serial_setFlag(uint32_t flag)
{
	if (event_id == NULL)
	{
		return MY_ERR;
	}

	if ((osEventFlagsSet(event_id, flag) & osFlagsError) != 0U)
	{
		return MY_ERR;
	}

	return MY_OK;
}

void resetFSM()
{
	serialQueueHandle.fsm = SERIAL_STATE_IDLE;
	serialQueueHandle.lenMonitor = 0;
	serialQueueHandle.lenPayloadMonitor = 0;
}

myError_t isTrueLenCrcPacket(SerialPacket_t *pPacket)
{
	uint8_t lcrc = pPacket->id.high ^ pPacket->id.low ^ pPacket->len.high ^ pPacket->len.low;
	if (pPacket->len.lcrc != lcrc)
	{
		return MY_ERR_CRC;
	}
	return MY_OK;
}

myError_t isTrueCrcPacket(SerialPacket_t *pPacket)
{
	return MY_OK;
}

static myError_t sendACK(uint8_t errorCode)
{
	SERIAL_INF("send ack");

	return MY_OK;
}

static myError_t sendNAK(uint8_t errorCode)
{
	SERIAL_INF("send NAK");

	return MY_OK;
}

myError_t serial_ProcessFSM(uint8_t data, uint16_t *pLen, SerialPacket_t *pPacket, SerialErrorQueueFlag_t *pFlag, SerialFSM_t *pFsm)
{
	// reset state
	static uint16_t lenPayload = 0;

	uint8_t *pData = (uint8_t *) pPacket;
	pData[*pLen] = data;
	*pLen +=1;
	*pFlag = SERIAL_PROCESSING;

	switch (*pFsm)
	{
		case SERIAL_STATE_IDLE:
			if (data != SERIAL_HOST_PACKET_HEADER_FI)
			{
				SERIAL_INF("Dummy data");
				resetFSM();
				break;
			}
			SERIAL_INF("---> Receive header_fi");
			*pFsm = SERIAL_STATE_WAIT_HEADER;
			break;
		case SERIAL_STATE_WAIT_HEADER:
			if (data != SERIAL_HOST_PACKET_HEADER_SE)
			{
				SERIAL_INF("Dummy data not SERIAL_HOST_PACKET_HEADER_SE");
				resetFSM();
				break;
			}
			SERIAL_INF("---> Receive header_se");
			*pFsm = SERIAL_STATE_WAIT_CMD_ID;
			break;
		case SERIAL_STATE_WAIT_CMD_ID:
			if (*pLen > (sizeof(SerialPacketHeader_t) + sizeof(SerialPacketCmdID_t) - 1))
			{
				SERIAL_INF("---> Receive id");
				*pFsm = SERIAL_STATE_WAIT_LEN;
			}
			break;
		case SERIAL_STATE_WAIT_LEN:
			lenPayload = 0;
			if (*pLen > (sizeof(SerialPacketHeader_t) + sizeof(SerialPacketCmdID_t) + sizeof(SerialPacketLenght_t) - 1))
			{
				SERIAL_INF("---> Received len");
				lenPayload = (pPacket->len.high << 8 | pPacket->len.low);
				printf("lenPayload =  %d\r\n", lenPayload);
				*pFsm = SERIAL_STATE_WAIT_PAYLOAD;
				// check crc len
			}
			break;
		case SERIAL_STATE_WAIT_PAYLOAD:
			if (*pLen > (sizeof(SerialPacketHeader_t) + sizeof(SerialPacketCmdID_t) + sizeof(SerialPacketLenght_t) + lenPayload))
			{
				SERIAL_INF("---> Received payload finished");
				show_hex(&pPacket->payload.cmd, lenPayload);

				*pFsm = SERIAL_STATE_WAIT_CRC;
				break;
			}
			break;
		case SERIAL_STATE_WAIT_CRC:
			if (isTrueCrcPacket(pPacket) != MY_OK)
			{
				SERIAL_INF("---> Received Wrong CRC\r\n");
			}
			SERIAL_INF("---> Received CRC");
			SERIAL_INF("---> Handle command");
			*pFlag = SERIAL_HANLE_CMD;
			resetFSM();
			break;
		default:
			break;

	}

	return MY_OK;
}

myError_t srv_serial_create()
{
	myError_t err = MY_OK;

	// create event groups
	event_id = osEventFlagsNew(NULL);
	if (event_id == NULL)
	{
		SERIAL_INF("create event_id serial fail");
		err = MY_ERR;
	}
	// create timer

	// create semaphore

	// create mutex

	// create task
	serialThreadHandle = osThreadNew(srv_serial_thread, NULL, &thread_attributes);
	if (serialThreadHandle == NULL)
	{
		err = MY_ERR;
	}

	/* initialize queue handle and FSM state */
	memset(&serialQueueHandle, 0, sizeof(serialQueueHandle));
	resetFSM();
	serialQueueHandle.flag = SERIAL_NO_DATA;

	Test_srv_serial_test();

	return err;
}

void srv_serial_thread()
{
	SERIAL_INF("srv_serial_thread");

	uint32_t flags;
	uint16_t len = 0;
	SerialPacket_t *pPacket = (SerialPacket_t *) &serialQueueHandle.packet;
	SerialErrorQueueFlag_t *pFlag = (SerialErrorQueueFlag_t *) &serialQueueHandle.flag;
	SerialFSM_t	*pfsm = (SerialFSM_t*) &serialQueueHandle.fsm;
	uint16_t *pLen = (uint16_t*) &serialQueueHandle.lenMonitor;
	SerialApi_t api =
	{
		.setFlag = serial_setFlag,
	};

	uint8_t *msgIn = (uint8_t*) &serialQueueHandle.packet.payload.cmd;
	uint16_t msgInlen = 0;

	// init peripheral
	if (serial_init(&huart1, &api) != MY_OK)
	{
		SERIAL_INF("serial_init failed");
		osThreadExit();
	}

	while(1)
	{
		flags = osEventFlagsWait(event_id, SERIAL_FLAG_WAIT, osFlagsWaitAny, osWaitForever);
		if (flags & FLAG_HAVE_DATA)
		{
			len = serial_getALLData(tempBuf);
			printf("len %d byte\r\n", len);

			for (uint16_t i = 0; i < len; i++)
			{
				printf("0x%02X --> ",tempBuf[i]);
				serial_ProcessFSM(tempBuf[i], pLen, pPacket, pFlag, pfsm);

				if(*pFlag != SERIAL_HANLE_CMD)
				{
					continue;
				}
				*pFlag = SERIAL_PROCESSING;

				uint8_t idx = findIndexHandler(pPacket->payload.cmd, pPacket->payload.subCmd);
				if (idx == 0xFFFF)
				{
					// send NAK
					sendNAK(0);
					continue;
				}
				msgInlen = pPacket->len.high << 8 | pPacket->len.low;
				// send ack
				sendACK(0);

				Serial_runHandler(idx, msgIn, msgInlen, 0, 0);
				// handle command here
				printf("======================== handle finish ==================\r\n");
			}
			printf("\r\n");

		}
	}
}

