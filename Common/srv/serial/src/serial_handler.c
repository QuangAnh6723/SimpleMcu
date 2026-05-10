/*
 * serial_handler.c
 *
 *  Created on: May 10, 2026
 *      Author: AnhLe
 */

#include "../inc/serial_handler.h"
#include "../inc/serial_payload.h"

#include "myInclude.h"

typedef struct SerialHandler_t
{
	uint8_t cmd;
	uint8_t subCmd;
	myError_t (*handler)(uint8_t msgIn[], uint16_t msgInLen, uint8_t msgOut[], uint16_t *msgOutLen);
} SerialCmdHandler_t;

SerialCmdHandler_t cmdHandlerTable[] =
{
	// General command
	{SERIAL_CMD_GENERAL, SERIAL_SUB_CMD_GENERAL_FW_VERSION, CmdGetFirmwareVersion},
	{SERIAL_CMD_GENERAL, SERIAL_SUB_CMD_GENERAL_ECHo, 		CmdEcho				 },
};

#define SERIAL_TABLE_SIZE 		(sizeof(cmdHandlerTable)/ sizeof(SerialCmdHandler_t))

uint16_t findIndexHandler(uint8_t cmd, uint8_t subCmd)
{
	uint16_t size = SERIAL_TABLE_SIZE;

	for (uint16_t i = 0; i < size; i++)
	{
		if (cmd == cmdHandlerTable[i].cmd && subCmd == cmdHandlerTable[i].subCmd)
		{
			return i;
		}
	}
	return 0xFFFF;
}

myError_t Serial_runHandler(uint16_t idx, uint8_t msgIn[], uint16_t msgInlen, uint8_t msgOut[], uint16_t *msgOutLen)
{
	if (cmdHandlerTable[idx].handler == NULL)
	{
		return MY_ERR;
	}
	return cmdHandlerTable[idx].handler(msgIn, msgInlen, msgOut, msgOutLen);
}


