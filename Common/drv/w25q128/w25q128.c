/*
 * w25q128.c
 *
 *  Created on: Apr 25, 2026
 *      Author: AnhLe
 */
#include "myInclude.h"
#include "w25q128.h"

extern SPI_HandleTypeDef hspi1;

void W25_WriteEnable(void) {
    uint8_t cmd = 0x06;
    W25_EN_CS;
    HAL_SPI_Transmit(&hspi1, &cmd, 1, 100);
    W25_DI_CS;
}

void W25_WaitBusy(void) {
    uint8_t status = 0;
    uint8_t cmd = 0x05;
    do {
        W25_EN_CS;
        HAL_SPI_Transmit(&hspi1, &cmd, 1, 100);
        HAL_SPI_Receive(&hspi1, &status, 1, 100);
        W25_DI_CS;
    } while ((status & 0x01) == 0x01); // Poll bit BUSY
}

void W25_WriteDisable()
{
	uint8_t dataDisWrite[] = {0x04};
	W25_EN_CS;
	HAL_SPI_TransmitReceive(&hspi1, dataDisWrite, 0, sizeof(dataDisWrite), 1000);
	W25_DI_CS;
}

void w25_readID()
{
	printf("%s\r\n", __func__);

	W25_EN_CS;

	// read
	uint8_t dataTX[] = {0x9F, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff};
	uint8_t dataRX[8] = {0};

	HAL_SPI_TransmitReceive(&hspi1, dataTX, dataRX, sizeof(dataTX), 1000);

	show_hex(dataTX, sizeof(dataTX));
	show_hex(dataRX, sizeof(dataTX));

	W25_DI_CS;
	(void) 0;
}

// TODO : Implement more function

void w25_test()
{
//	w25_write();
//	w25_read();
}
