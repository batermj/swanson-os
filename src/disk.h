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

#ifndef SWANSON_DISK_H
#define SWANSON_DISK_H

#include "stream.h"

#ifdef __cplusplus
extern "C" {
#endif

/** A disk structure.
 * The disk may be anything that behaves
 * like a stream, whether a physical hard disk,
 * a file, or a block of memory.
 * @see fdisk
 * @see partition
 * */

struct disk {
	/** The stream callbacks for the disk. */
	struct stream stream;
	/** The sector size of the disk. */
	uint64_t sector_size;
};

/** Initializes a disk structure.
 * This function should be called before
 * the structure is used.
 * @param disk An uninitialized disk
 * structure.
 * */

void
disk_init(struct disk *disk);

/** Converts a disk structure to
 * a stream structure.
 * @param disk An intialized disk structure.
 * @returns The stream structure of the
 * disk, this is never NULL.
 * */

struct stream *
disk_to_stream(struct disk *disk);

/** Sets the sector size of the disk.
 * While this may be any value, the
 * traditional values are: 512 for older
 * hard drives, 2048 for CD and DVDs and
 * 4096 for newer hard drives that use
 * advanced format.
 * @param disk An initialized disk structure.
 * @param sector_size The new sector size.
 * */

void
disk_set_sector_size(struct disk *disk,
                     uint64_t sector_size);

/** Reads sectors from a disk.
 * @param disk An initialized disk
 * structure.
 * @param buf The buffer to write the
 * sectors to.
 * @param sector_count The number of
 * sectors to read.
 * @returns The number of sectors that
 * were read.
 * */

uint64_t
disk_read(struct disk *disk,
          void *buf,
          uint64_t sector_count);

/** Seek to a new sector.
 * @param disk An initialized disk
 * structure.
 * @param sector_index The index of
 * the sector to move to.
 * @returns Zero on success, non-zero
 * otherwise.
 * */

int
disk_seek(struct disk *disk,
          uint64_t sector_index);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_DISK_H */
