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

#ifndef SWANSON_FDISK_H
#define SWANSON_FDISK_H

#include "disk.h"

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/** A disk contained within a file
 * on the host computers file system. */
struct fdisk {
	/** The base disk structure. */
	struct disk base;
	/** The file associated with the disk. */
	FILE *file;
};

void fdisk_init(struct fdisk *fdisk);

void fdisk_close(struct fdisk *fdisk);

int fdisk_open(struct fdisk *fdisk,
               const char *path,
               const char *mode);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_FDISK_H */
