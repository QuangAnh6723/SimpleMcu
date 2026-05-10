/*
 * src_serial_test.c
 *
 *  Created on: May 9, 2026
 *      Author: AnhLe
 */

#include "../inc/srv_serial_test.h"
#include "../inc/serial_protocol.h"
#include "../inc/serial_payload.h"
#include "../inc/drv_serial.h"

#include "misc/crc/crc16.h"
#include "misc/misc.h"
#include "string.h"

#include "myInclude.h"

#define HOST_FI 		SERIAL_HOST_PACKET_HEADER_FI
#define HOST_SE 		SERIAL_HOST_PACKET_HEADER_SE

static osThreadId_t 		serialThreadHandle;
static const osThreadAttr_t thread_attributes =
{
  .name = "serial task",
  .stack_size = 128 * 20,
  .priority = (osPriority_t) osPriorityNormal,
};

uint8_t packet[SERIAL_PAKET_SIZE_BUF_MAX] = { 0 };

extern myError_t serial_setFlag(uint32_t flag);

static uint16_t calcCrcPacket(void *packet, uint16_t len)
{
	return crc16_ccitt(packet, len);
}

static uint16_t buildHostPacket(uint8_t packetArr[], uint8_t cmd, uint8_t subCmd, uint8_t resultCode, uint8_t data[], uint16_t payloadSize)
{
	SerialPacket_t *packet = (SerialPacket_t*) packetArr;
	static uint16_t commandID = 0;
	uint16_t crc = 0;

	packet->header.first  = HOST_FI;
	packet->header.second = HOST_SE;

	packet->id.high = (commandID >> 8) & 0xFF;
	packet->id.low  = (commandID >> 0) & 0xFF;
	commandID ++;

	packet->len.high = (payloadSize >> 8) & 0xFF;
	packet->len.low  = (payloadSize >> 0) & 0xFF;
	packet->len.lcrc = packet->id.high ^ packet->id.low  ^ packet->len.high ^ packet->len.low;

	packet->payload.cmd    = cmd;
	packet->payload.subCmd = subCmd;
	packet->payload.result = resultCode;

	// 10 bytes + payload.data size
	if (data != 0)
	{
		memcpy(&packet->payload.data, data, payloadSize - 3);
	}

	// crc calc form command ID (2 byte id, 2 bytes len, 1 byte crc len)
	crc = calcCrcPacket(&packet->id, payloadSize + 5);

	packetArr[7 + payloadSize] = (crc >> 8) & 0xFF ;
	packetArr[7 + payloadSize + 1] = (crc >> 0) & 0xFF ;

	packet->crc.high = (crc >> 8) & 0xFF;
	packet->crc.low = (crc >> 0) & 0xFF ;

	printf("full packet: ");
	show_hex(packetArr, 7 + payloadSize + 2);

	serialPacket_show(packet);

	return 7 + payloadSize + 2;
}

void putDataToSerial(uint8_t data[], uint8_t len)
{
	serial_putData(data, len);
	serial_setFlag(FLAG_HAVE_DATA);
}

void TestCmd_GetFirmwareVersion()
{
	uint8_t cmd 	= SERIAL_CMD_GENERAL;
	uint8_t subCmd 	= SERIAL_SUB_CMD_GENERAL_FW_VERSION;
	uint8_t resultCode = 0;
	static uint8_t data[] = {0x11, 0x12};
	data[1] ++;
	// NO DATA
	uint16_t payloadSize = 3 + sizeof(data);

	uint16_t len = buildHostPacket(packet, cmd, subCmd,resultCode, data, payloadSize);
	SERIAL_INF("TestCmd_GetFirmwareVersion");
	SERIAL_INF("CMD GET VERSION, ");

	show_hex(packet, len);
	putDataToSerial(packet, len);
}

void TestCmd_Echo()
{
	uint8_t cmd 	= SERIAL_CMD_GENERAL;
	uint8_t subCmd 	= SERIAL_SUB_CMD_GENERAL_ECHo;
	uint8_t resultCode = 0;
	static uint8_t data[] = {0x11, 0x12};
	data[1] ++;
	// NO DATA
	uint16_t payloadSize = 3 + sizeof(data);

	uint16_t len = buildHostPacket(packet, cmd, subCmd,resultCode, data, payloadSize);
	SERIAL_INF("TestCmd_Echo");
	SERIAL_INF("CMD ECHO, ");

	show_hex(packet, len);
	putDataToSerial(packet, len);
}

static void test_srv_serial_thread()
{
	SERIAL_INF("test_srv_serial_thread Running");
	osDelay(2000);

	while(1)
	{
		SERIAL_INF("=========================== start test =====================");
		TestCmd_Echo();
		osDelay(2000);
	}
}

void Test_srv_serial_test()
{
	serialThreadHandle = osThreadNew(test_srv_serial_thread, NULL, &thread_attributes);
	if (serialThreadHandle == NULL)
	{

	}
}


