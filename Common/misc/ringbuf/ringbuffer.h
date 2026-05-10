/*
 * ringbuffer.h
 *
 *  Created on: Apr 25, 2026
 *      Author: AnhLe
 */

#ifndef MISC_RINGBUF_RINGBUFFER_H_
#define MISC_RINGBUF_RINGBUFFER_H_

#include "stdint.h"

#define MAX_RING_BUFFER		1024

typedef struct {
	uint16_t 	head;				// write
	uint16_t 	tail;				// read
	int16_t 	count;
	uint16_t 	overflow;
	uint8_t 	buffer[MAX_RING_BUFFER];
} RingBuffer_t;

typedef enum {
	RB_OK 				= 0,
	RB_NOT_VALID_PARA	= -1,
	RB_NOT_ENOUGH_SPACE = -2,
	RB_NOT_ENOUGH_DATA	= -3,
} RingbufferErr_t;

RingbufferErr_t rb_init(RingBuffer_t *rb);
RingbufferErr_t rb_clear(RingBuffer_t *rb);

RingbufferErr_t rb_write(RingBuffer_t *rb ,uint8_t data[], uint16_t len);
RingbufferErr_t rb_read(RingBuffer_t *rb ,uint8_t data[], uint16_t len);

int16_t 	rb_remain(RingBuffer_t *rb);
int16_t		rb_available(RingBuffer_t *rb);


#endif /* MISC_RINGBUF_RINGBUFFER_H_ */
