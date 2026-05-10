/*
 * bootloader.h
 *
 *  Created on: Apr 25, 2026
 *      Author: AnhLe
 */

#ifndef APP_BOOTLOADER_BOOTLOADER_H_
#define APP_BOOTLOADER_BOOTLOADER_H_

#include "myInclude.h"

#if DEBUG_BOOTLOADER

#define BOOT_INF(fmt, ...)			printf("[Boot] "fmt"\r\n" ##__VA_ARGS__)

#else

#define BOOT_INF(fmt, ...)

#endif

myError_t 	app_bootloader_create();
void 		app_bootloader_thread();

#endif /* APP_BOOTLOADER_BOOTLOADER_H_ */
