/*
 * bootloader.c
 *
 *  Created on: Apr 25, 2026
 *      Author: AnhLe
 */

#include "../inc/bootloader.h"
#include "myInclude.h"

static osThreadId_t 		bootloaderThreadHandle;
static const osThreadAttr_t thread_attributes =
{
  .name = "2bootloader task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

myError_t app_bootloader_create()
{
	myError_t err = MY_OK;

	// create event groups

	// create timer

	// create semaphore

	// create task

	bootloaderThreadHandle = osThreadNew(app_bootloader_thread, NULL, &thread_attributes);
	if (bootloaderThreadHandle != osOK)
	{
		err = MY_ERR;
	}

	return err;
}

void app_bootloader_thread()
{
	BOOT_INF("hello ca nha");

	while(1)
	{
		BOOT_INF("running");
		osDelay(1000);
	}
}
