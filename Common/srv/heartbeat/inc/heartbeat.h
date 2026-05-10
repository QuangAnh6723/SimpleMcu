/*
 * heartbeat.h
 *
 *  Created on: Apr 25, 2026
 *      Author: AnhLe
 */

#ifndef SRV_HEARTBEAT_INC_HEARTBEAT_H_
#define SRV_HEARTBEAT_INC_HEARTBEAT_H_

#include "myInclude.h"

myError_t srv_heartbeat_create();
void 	  srv_heartbeat_thread();

#endif /* SRV_HEARTBEAT_INC_HEARTBEAT_H_ */
