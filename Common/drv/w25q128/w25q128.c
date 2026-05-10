/*
 * w25q128.c
 *
 *  Created on: Apr 25, 2026
 *      Author: AnhLe
 */
#include "myInclude.h"
#include "w25q128.h"

typedef struct {

} W25q128_t;

static W25q128_t w25;

void w25_init(W25q128_t *obj)
{
	w25 = *obj;
}

void w25_write(uint8_t data[], uint32_t size)
{

}

void w25_read()
{

}
