/*
 * ringbuffer.c
 *
 *  Created on: Apr 25, 2026
 *      Author: AnhLe
 */


#include "ringbuffer.h"
#include "stddef.h"
#include "myInclude.h"

RingbufferErr_t rb_init(RingBuffer_t *rb)
{
	rb->head = 0;
	rb->tail = 0;
	rb->count = 0;

	return RB_OK;
}

RingbufferErr_t rb_clear(RingBuffer_t *rb)
{
	return rb_init(rb);
}

RingbufferErr_t rb_write(RingBuffer_t *rb ,uint8_t data[], uint16_t len)
{
	if (rb == NULL || data == NULL)
	{
		return RB_NOT_VALID_PARA;
	}

	if (len > rb_remain(rb))
	{
		return RB_NOT_ENOUGH_SPACE;
	}

	for (uint16_t i = 0; i < len; i++)
	{
		rb->buffer[rb->head++] = data[i];
		if(rb->head == MAX_RING_BUFFER)
		{
			rb->head = 0;
		}
	}

	rb->count += len;

	return RB_OK;
}

RingbufferErr_t rb_read(RingBuffer_t *rb ,uint8_t data[], uint16_t len)
{
	if (rb == NULL || data == NULL)
	{
		return RB_NOT_VALID_PARA;
	}

	if ( len > rb->count)
	{
		return RB_NOT_ENOUGH_DATA;
	}

	for (uint16_t i = 0; i < len; i++)
	{
		data[i] = rb->buffer[rb->tail++];
		if(rb->tail == MAX_RING_BUFFER)
		{
			rb->tail = 0;
		}
	}

	rb->count -= len;

	return RB_OK;
}
int16_t rb_remain(RingBuffer_t *rb)

{
	if (rb == NULL)
    {
        return RB_NOT_VALID_PARA;
    }

	return MAX_RING_BUFFER - rb->count - 1U;
}

int16_t rb_available(RingBuffer_t *rb)
{
	if (rb == NULL)
    {
        return RB_NOT_VALID_PARA;
    }

	return  rb->count;
}
