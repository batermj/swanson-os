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

#ifndef SWANSON_FS_FAT_CACHE_H
#define SWANSON_FS_FAT_CACHE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct fatfs;
struct fat_file;

int
fatfs_cache_init(struct fatfs *fs,
                 struct fat_file *file);

int
fatfs_cache_get_next_cluster(struct fatfs *fs,
                             struct fat_file *file,
                             uint32_t clusterIdx,
                             uint32_t *pNextCluster);

int
fatfs_cache_set_next_cluster(struct fatfs *fs,
                             struct fat_file *file,
                             uint32_t clusterIdx,
                             uint32_t nextCluster);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_FS_FAT_CACHE_H */
