/*
 * led.c
 *
 *  Created on: Apr 25, 2026
 *      Author: AnhLe
 */

#include "../inc/led.h"

typedef struct {
	void (*on)     (void);
	void (*off)    (void);
	void (*toggle) (void);
};

void led_init()
{

}
