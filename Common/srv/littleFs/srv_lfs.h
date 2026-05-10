/*
 * src_lfs.h
 *
 *  Created on: Apr 27, 2026
 *      Author: AnhLe
 */

#ifndef SRV_LITTLEFS_SRV_LFS_H_
#define SRV_LITTLEFS_SRV_LFS_H_

#include "myInclude.h"
#include "srv_lfs.h"
#include "lfs.h"

int w25_lfs_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size);
int w25_lfs_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size);
int w25_lfs_erase(const struct lfs_config *c, lfs_block_t block);
int w25_lfs_sync(const struct lfs_config *c);

myError_t srv_littleFs_create();

#endif /* SRV_LITTLEFS_SRV_LFS_H_ */
