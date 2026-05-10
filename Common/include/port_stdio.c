/*
 * port_stdio.c
 *
 *  Created on: Apr 25, 2026
 *      Author: AnhLe
 */
#include <stdio.h>
#include "stm32f4xx.h"
#include "config.h"

#if DEBUG_PRINT == DEBUG_PRINT_RTT

#include "misc/rtt/SEGGER_RTT.h"

int _write(int file, char *ptr, int len)
{
    (void) file;
    SEGGER_RTT_Write(0, ptr, len);
    return len;
}

#elif DEBUG_PRINT == DEBUG_PRINT_SWO

int _write(int file, char *ptr, int len)
{
	int i;
	(void) file;
    for (i = 0; i < len; i++) {
        ITM_SendChar((*ptr++));
    }
    return len;
}
#else
	// implement later

#endif

