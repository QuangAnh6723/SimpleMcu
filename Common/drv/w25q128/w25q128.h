/*
 * w25q128.h
 *
 *  Created on: Apr 25, 2026
 *      Author: AnhLe
 */

#ifndef DRV_W25Q128_W25Q128_H_
#define DRV_W25Q128_W25Q128_H_

#include "myInclude.h"

#define W25_EN_CS	HAL_GPIO_WritePin(W25_CS_GPIO_Port, W25_CS_Pin, 0)
#define W25_DI_CS	HAL_GPIO_WritePin(W25_CS_GPIO_Port, W25_CS_Pin, 1)

void w25_readID();
void w25_test();

void W25_WriteEnable();
void W25_WriteDisable();

void W25_WaitBusy();

#endif /* DRV_W25Q128_W25Q128_H_ */
