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

#ifndef SWANSON_FDISK_H
#define SWANSON_FDISK_H

#include "disk.h"

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/** A disk contained within a file
 * on the host computers file system.
 * */

struct fdisk {
	/** The base disk structure. */
	struct disk base;
	/** The file associated with the disk. */
	FILE *file;
	/** The path of the file associated with
	 * the disk. */
	char *path;
};

/** Initializes structure members.
 * This should be called before @ref fdisk_open.
 * @param fdisk An uninitialized disk structure.
 * */

void fdisk_init(struct fdisk *fdisk);

/** Closes the file associated with
 * the disk. This function is safe to
 * call more than once - meaning it won't
 * fail if a file isn't open.
 * @param fdisk An initialized disk structure.
 * */

void fdisk_close(struct fdisk *fdisk);

/** Opens a disk containing by a file
 * in the host operating system. @ref fdisk_init
 * should be called before this function.
 * @param fdisk An initialized disk structure.
 * @param path The path, on the host file system,
 * of the disk file.
 * @param mode The mode to open the file in. This
 * argument is passed to the standard library
 * function, fopen.
 * @returns If the file opens successfully, zero
 * is returned. Non-zero is returned otherwise.
 * */

int fdisk_open(struct fdisk *fdisk,
               const char *path,
               const char *mode);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_FDISK_H */
