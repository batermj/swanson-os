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
};

/** Initializes a disk structure.
 * This function should be called before
 * the structure is used.
 * @param disk An uninitialized disk
 * structure.
 * */

void disk_init(struct disk *disk);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_DISK_H */
