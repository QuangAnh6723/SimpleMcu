/*
 * srv_serial.h
 *
 *  Created on: Apr 25, 2026
 *      Author: AnhLe
 */

#ifndef SRV_SERIAL_INC_SRV_SERIAL_H_
#define SRV_SERIAL_INC_SRV_SERIAL_H_

#include "myInclude.h"

#if 	DEBUG_SRV_SERIAL
#define SERIAL_INF(fmt,...)			printf("[Serial]"fmt"\r\n" ##__VA_ARGS__)
#else
#define SERIAL_INF(fmt,...)
#endif

myError_t srv_serial_create();

void srv_serial_thread();

#endif /* SRV_SERIAL_INC_SRV_SERIAL_H_ */
