/*
 * serial_protocol.c
 *
 *  Created on: May 9, 2026
 *      Author: AnhLe
 */

#include "../inc/serial_protocol.h"
#include "myInclude.h"

static void print_hex_list(const uint8_t *data, uint32_t size)
{
	for (uint32_t i = 0; i < size; i++)
	{
		printf("0x%02X", data[i]);
		if (i + 1U < size)
		{
			printf(", ");
		}
	}
}

void serialPacket_show(SerialPacket_t *packet)
{
	uint16_t size = ((uint16_t)packet->len.high << 8) | packet->len.low;
	uint16_t data_len = (size > 3U) ? (uint16_t)(size - 3U) : 0U;

	printf("============== Packet ==============\r\n");

	printf("{\r\n");
	printf("    header: ");
	print_hex_list((const uint8_t *)&packet->header, sizeof(SerialPacketHeader_t));
	printf(",\r\n");

	printf("    command_id: ");
	print_hex_list((const uint8_t *)&packet->id, sizeof(SerialPacketCmdID_t));
	printf(",\r\n");

	printf("    len: %u -> ", size);
	print_hex_list((const uint8_t *)&packet->len, sizeof(SerialPacketLenght_t));
	printf(",\r\n");

	printf("    payload:\r\n");
	printf("    {\r\n");
	printf("        cmd: 0x%02X,\r\n", packet->payload.cmd);
	printf("        sub: 0x%02X,\r\n", packet->payload.subCmd);
	printf("        result: 0x%02X,\r\n", packet->payload.result);
	printf("        data: ");
	if (data_len == 0U)
	{
		printf("[]");
	}
	else
	{
		print_hex_list(packet->payload.data, data_len);
	}
	printf(",\r\n");
	printf("    },\r\n");

	printf("    crc: 0x%02X, 0x%02X\r\n", packet->crc.high, packet->crc.low);
	printf("}\r\n");
}
