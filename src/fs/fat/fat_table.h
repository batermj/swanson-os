/* Copyright (C) 2017 Taylor Holberton
 *
 * This file is part of Swanson.
 *
 * Swanson is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Swanson is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Swanson.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SWANSON_FS_FAT_TABLE_H
#define SWANSON_FS_FAT_TABLE_H

#include <stdint.h>

#include "fat_opts.h"
#include "fat_misc.h"

#ifdef __cplusplus
extern "C" {
#endif

struct fatfs;

void
fatfs_fat_init(struct fatfs *fs);

int
fatfs_fat_purge(struct fatfs *fs);

uint32_t
fatfs_find_next_cluster(struct fatfs *fs,
                        uint32_t current_cluster);

void
fatfs_set_fs_info_next_free_cluster(struct fatfs *fs,
                                    uint32_t newValue);

int
fatfs_find_blank_cluster(struct fatfs *fs,
                         uint32_t start_cluster,
                         uint32_t *free_cluster);

int
fatfs_fat_set_cluster(struct fatfs *fs,
                      uint32_t cluster,
                      uint32_t next_cluster);

int
fatfs_fat_add_cluster_to_chain(struct fatfs *fs,
                               uint32_t start_cluster,
                               uint32_t newEntry);

int
fatfs_free_cluster_chain(struct fatfs *fs,
                         uint32_t start_cluster);

uint32_t
fatfs_count_free_clusters(struct fatfs *fs);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_FS_FAT_TABLE_H */
