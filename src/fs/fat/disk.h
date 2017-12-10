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

/** @file */

#ifndef SWANSON_FS_FAT_DISK_H
#define SWANSON_FS_FAT_DISK_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Used for reading serialized
 * FAT data.
 * */

struct fat_disk {
	/** Implementation data. */
	void *data;
	/** Reads sectors from disk. */
	int (*read)(void *data,
	            uint32_t sector,
	            void *buffer,
	            uint32_t sector_count);
	/** Writes sectors to disk. */
	int (*write)(void *data,
	             uint32_t sector_index,
	             const void *buffer,
	             uint32_t sector_count);
};

/** Initializes the disk structure.
 * @param disk An uninitialized disk structure.
 * */

void
fat_disk_init(struct fat_disk *disk);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_FS_FAT_DISK_H */
