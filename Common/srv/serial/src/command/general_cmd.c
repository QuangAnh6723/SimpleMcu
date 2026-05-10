/*
 * general_cmd.c
 *
 *  Created on: May 10, 2026
 *      Author: AnhLe
 */

#include "myInclude.h"


myError_t CmdGetFirmwareVersion(uint8_t msgIn[], uint16_t msgInLen, uint8_t msgOut[], uint16_t *msgOutLen)
{
	printf("%s\r\n", __func__);

	return MY_OK;
}

myError_t CmdEcho(uint8_t msgIn[], uint16_t msgInLen, uint8_t msgOut[], uint16_t *msgOutLen)
{
	printf("%s\r\n", __func__);

	return MY_OK;
}

