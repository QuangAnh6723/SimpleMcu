/*
 * srv_lfs.c
 *
 *  Created on: Apr 27, 2026
 *      Author: AnhLe
 */

#include "myInclude.h"
#include "srv_lfs.h"
#include "lfs.h"
#include "log_littleFs.h"

#include "drv/w25q128/w25q128.h"

static osThreadId_t 		littleFsThreadHandle;
static const osThreadAttr_t thread_attributes =
{
  .name = "littleFs task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

lfs_t lfs;
lfs_file_t file;

const struct lfs_config cfg =
{
    // block device operations
    .read  = w25_lfs_read,
    .prog  = w25_lfs_prog,
    .erase = w25_lfs_erase,
    .sync  = w25_lfs_sync,

    // block device configuration
    .read_size = 16,
    .prog_size = 256,				// page size
    .block_size = 4096,				// <-- sector
    .block_count = 4096,			//
    .cache_size = 256,
    .lookahead_size = 32,
    .block_cycles = 500,
};

int w25_lfs_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size)
{
    uint32_t addr = (block * c->block_size) + off;
    uint8_t cmd[4];

    cmd[0] = 0x03;               // Read Data Command
    cmd[1] = (addr >> 16) & 0xFF;
    cmd[2] = (addr >> 8) & 0xFF;
    cmd[3] = addr & 0xFF;

    W25_EN_CS;
    HAL_SPI_Transmit(&hspi1, cmd, 4, 100);
    HAL_SPI_Receive(&hspi1, (uint8_t*)buffer, size, 1000);
    W25_DI_CS;

    return LFS_ERR_OK;
}

int w25_lfs_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size)
{
    uint32_t addr = (block * c->block_size) + off;
    uint8_t cmd[4];

    cmd[0] = 0x02;               // Page Program Command
    cmd[1] = (addr >> 16) & 0xFF;
    cmd[2] = (addr >> 8) & 0xFF;
    cmd[3] = addr & 0xFF;

    W25_WaitBusy();              // Chờ nếu chip đang bận việc khác
    W25_WriteEnable();           // BẮT BUỘC trước khi ghi

    W25_EN_CS;
    HAL_SPI_Transmit(&hspi1, cmd, 4, 100);
    HAL_SPI_Transmit(&hspi1, (uint8_t*)buffer, size, 1000);
    W25_DI_CS;

    W25_WaitBusy();              // Chờ chip hoàn tất ghi nội bộ
    return LFS_ERR_OK;
}

int w25_lfs_erase(const struct lfs_config *c, lfs_block_t block)
{
    uint32_t addr = block * c->block_size;
    uint8_t cmd[4];

    cmd[0] = 0x20;               // Sector Erase Command
    cmd[1] = (addr >> 16) & 0xFF;
    cmd[2] = (addr >> 8) & 0xFF;
    cmd[3] = addr & 0xFF;

    W25_WaitBusy();
    W25_WriteEnable();           // BẮT BUỘC trước khi xóa

    W25_EN_CS;
    HAL_SPI_Transmit(&hspi1, cmd, 4, 100);
    W25_DI_CS;

    W25_WaitBusy();              // Xóa sector mất khoảng 45ms - 400ms
    return LFS_ERR_OK;
}

int w25_lfs_sync(const struct lfs_config *c)
{
    // Không cần xử lý vì các hàm trên đã đợi chip hết bận (WaitBusy)
    return LFS_ERR_OK;
}

extern void ledStatus_toggle();

void srv_littleFs_thread()
{
	printf("%s\r\n", __func__);

	while(1)
	{
		osDelay(1000);
	}
}

void littleFs_init()
{
	// mount the filesystem
	int err = lfs_mount(&lfs, &cfg);

	// reformat if we can't mount the filesystem
	// this should only happen on the first boot
	if (err) {
		lfs_format(&lfs, &cfg);
		lfs_mount(&lfs, &cfg);
	}

	// read current count
	uint32_t boot_count = 0;
	lfs_file_open(&lfs, &file, "boot_count", LFS_O_RDWR | LFS_O_CREAT);
	lfs_file_read(&lfs, &file, &boot_count, sizeof(boot_count));

	// update boot count
	boot_count += 1;
	lfs_file_rewind(&lfs, &file);
	lfs_file_write(&lfs, &file, &boot_count, sizeof(boot_count));

	// remember the storage is not updated until the file is closed successfully
	lfs_file_close(&lfs, &file);

	// release any resources we were using
	lfs_unmount(&lfs);

	// print the boot count
	printf("boot_count: %lu\n", boot_count);
}

myError_t srv_littleFs_create()
{
	myError_t err = MY_OK;
	printf("%s\r\n", __func__);

	littleFsThreadHandle = osThreadNew(srv_littleFs_thread, NULL, &thread_attributes);
	if (littleFsThreadHandle != osOK)
	{
		err = MY_ERR;
	}

	littleFs_init();

	return err;
}


