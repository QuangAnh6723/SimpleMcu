/*
 * serial_handler.h
 *
 *  Created on: May 10, 2026
 *      Author: AnhLe
 */

#ifndef SRV_SERIAL_INC_SERIAL_HANDLER_H_
#define SRV_SERIAL_INC_SERIAL_HANDLER_H_

#include "myInclude.h"

uint16_t findIndexHandler(uint8_t cmd, uint8_t subCmd);
myError_t Serial_runHandler(uint16_t idx, uint8_t msgIn[], uint16_t msgInlen, uint8_t msgOut[], uint16_t *msgOutLen);

#endif /* SRV_SERIAL_INC_SERIAL_HANDLER_H_ */
