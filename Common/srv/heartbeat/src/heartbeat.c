/*
 * heartbeat.c
 *
 *  Created on: Apr 25, 2026
 *      Author: AnhLe
 */

#include "../inc/heartbeat.h"
#include "myInclude.h"

static osThreadId_t 		heartbeatThreadHandle;
static const osThreadAttr_t thread_attributes =
{
  .name = "heartbeat task",
  .stack_size = 128 * 2,
  .priority = (osPriority_t) osPriorityNormal,
};

myError_t srv_heartbeat_create()
{
	myError_t err = MY_OK;

	// create event groups

	// create timer

	// create semaphore

	// create task

	heartbeatThreadHandle = osThreadNew(srv_heartbeat_thread, NULL, &thread_attributes);
	if (heartbeatThreadHandle != osOK)
	{
		err = MY_ERR;
	}

	return err;
}

void ledStatus_toggle()
{
	HAL_GPIO_TogglePin(LED_STATUS_GPIO_Port, LED_STATUS_Pin);
//	HAL_GPIO_TogglePin(BUZZER_GPIO_Port, BUZZER_Pin);
//	HAL_GPIO_TogglePin(LED_0_GPIO_Port, LED_0_Pin);
//	HAL_GPIO_TogglePin(LED_1_GPIO_Port, LED_1_Pin);
}

void srv_heartbeat_thread()
{
	HAL_GPIO_TogglePin(BUZZER_GPIO_Port, BUZZER_Pin);
	osDelay(50);
	HAL_GPIO_TogglePin(BUZZER_GPIO_Port, BUZZER_Pin);

	while(1)
	{
		ledStatus_toggle();
		osDelay(100);
	}
}

