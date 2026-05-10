/*
 * serial_payload.h
 *
 *  Created on: May 7, 2026
 *      Author: AnhLe
 */

#ifndef SRV_SERIAL_INC_SERIAL_PAYLOAD_H_
#define SRV_SERIAL_INC_SERIAL_PAYLOAD_H_

#include "command/general_cmd.h"

// DEFINE COMMAND

#define SERIAL_CMD_GENERAL						0xA0

// DEFINE SUB COMMAND
#define SERIAL_SUB_CMD_GENERAL_FW_VERSION		0x00
#define SERIAL_SUB_CMD_GENERAL_ECHo				0x02


#endif /* SRV_SERIAL_INC_SERIAL_PAYLOAD_H_ */
