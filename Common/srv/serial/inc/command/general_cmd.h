/*
 * general_cmd.h
 *
 *  Created on: May 10, 2026
 *      Author: AnhLe
 */

#ifndef SRV_SERIAL_SRC_COMMAND_GENERAL_CMD_H_
#define SRV_SERIAL_SRC_COMMAND_GENERAL_CMD_H_

#include "myInclude.h"

myError_t CmdGetFirmwareVersion(uint8_t msgIn[], uint16_t msgInLen, uint8_t msgOut[], uint16_t *msgOutLen);
myError_t CmdEcho(uint8_t msgIn[], uint16_t msgInLen, uint8_t msgOut[], uint16_t *msgOutLen);

#endif /* SRV_SERIAL_SRC_COMMAND_GENERAL_CMD_H_ */
