/*
 * log_littleFs.h
 *
 *  Created on: May 10, 2026
 *      Author: AnhLe
 */

#ifndef SRV_LITTLEFS_LOG_LITTLEFS_H_
#define SRV_LITTLEFS_LOG_LITTLEFS_H_

#include "myInclude.h"

#if 	DEBUG_SRV_LITTLEFS

#define LITTLEFS_INF(fmt,...)			printf("[Serial]"fmt" \r\n" ##__VA_ARGS__)
#define LITTLEFS_FUNC()					printf("%s\r\n", __func__)

#else

#define LITTLEFS_INF(fmt,...)
#define LITTLEFS_FUNC()

#endif

#endif /* SRV_LITTLEFS_LOG_LITTLEFS_H_ */
