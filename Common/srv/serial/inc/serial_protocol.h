/*
 * protocol_serial.h
 *
 *  Created on: May 7, 2026
 *      Author: AnhLe
 */

#ifndef SRV_SERIAL_INC_SERIAL_PROTOCOL_H_
#define SRV_SERIAL_INC_SERIAL_PROTOCOL_H_

#include "myInclude.h"

#define SERIAL_HOST_PACKET_HEADER_FI			0x55
#define SERIAL_HOST_PACKET_HEADER_SE			0xAA

#define SERIAL_DEVICE_PACKET_HEADER_FI			0xAA
#define SERIAL_DEVICE_PACKET_HEADER_SE			0x55

typedef struct SerialPacket_t SerialPacket_t;

typedef struct SerialPacketHeader_t 	SerialPacketHeader_t;
typedef struct SerialPacketCmdID_t 		SerialPacketCmdID_t;
typedef struct SerialPacketLenght_t 	SerialPacketLenght_t;
typedef struct SerialPacketPayload_t 	SerialPacketPayload_t;
typedef struct SerialPacketCrc_t 		SerialPacketCrc_t;

#define SERIAL_MAX_PAYLOAD				1024

struct SerialPacketHeader_t
{
	uint8_t first;
	uint8_t	second;
};

struct SerialPacketCmdID_t
{
	uint8_t high;
	uint8_t	low;
};

struct SerialPacketLenght_t
{
	uint8_t high;
	uint8_t low;
	uint8_t lcrc;
};

struct SerialPacketPayload_t
{
	uint8_t cmd;
	uint8_t	subCmd;
	uint8_t result;
	uint8_t data[SERIAL_MAX_PAYLOAD - 3];
};

struct SerialPacketCrc_t
{
	uint8_t high;
	uint8_t	low;
};

struct SerialPacket_t
{
	SerialPacketHeader_t 	header;
	SerialPacketCmdID_t  	id;
	SerialPacketLenght_t 	len;
	SerialPacketPayload_t	payload;
	SerialPacketCrc_t		crc;
};

#define SERIAL_PAKET_SIZE_BUF_MAX		(sizeof(SerialPacket_t) * 2)

typedef enum SerialFSM SerialFSM_t;

enum SerialFSM
{
	SERIAL_STATE_IDLE = 0,
	SERIAL_STATE_WAIT_HEADER,
	SERIAL_STATE_WAIT_CMD_ID,
	SERIAL_STATE_WAIT_LEN,
	SERIAL_STATE_WAIT_PAYLOAD,
	SERIAL_STATE_WAIT_CRC,
};

typedef enum SerialErrorQueueFlag_t SerialErrorQueueFlag_t;

enum SerialErrorQueueFlag_t
{
	SERIAL_NO_DATA,
	SERIAL_PROCESSING,
	SERIAL_HANLE_CMD
};

typedef struct SerialQueueHandle_t SerialQueueHandle_t;

struct SerialQueueHandle_t
{
	SerialFSM_t 	fsm;
	SerialPacket_t 	packet;
	uint16_t 		lenMonitor;
	uint16_t 		lenPayloadMonitor;
	SerialErrorQueueFlag_t	flag;
};

void serialPacket_show(SerialPacket_t *packet);

#endif /* SRV_SERIAL_INC_SERIAL_PROTOCOL_H_ */
