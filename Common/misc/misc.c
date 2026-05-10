/*
 * misc.c
 *
 *  Created on: May 7, 2026
 *      Author: AnhLe
 */

#include "misc.h"
#include "myInclude.h"

void show_hex(void *data, uint32_t size)
{
	uint8_t *pData = (uint8_t *) data;
	for (uint32_t i = 0; i < size; i++)
	{
		printf("%02X ", pData[i]);
		if ((i + 1) % 32 == 0)
		{
			printf("\r\n");
		}
	}
	printf("\r\n");
}

void show_ascii(void *data, uint32_t size)
{
	uint8_t *pData = (uint8_t *)data;
	for (uint32_t i = 0; i < size; i++)
	{
		printf("%c", pData[i]);
//		if ((i + 1) % 32)
//		{
//			printf("\r\n");
//		}
	}
	printf("\r\n");
}
