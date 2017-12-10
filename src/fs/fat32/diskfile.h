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

#ifndef SWANSON_FS_FAT32_DISKFILE_H
#define SWANSON_FS_FAT32_DISKFILE_H

#include "disk.h"

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/** A FAT32-formatted disk file.
 * */

struct fat32_diskfile {
	/** The base disk structure, containing
	 * the required callbacks. */
	struct fat32_disk disk;
	/** The file containing the FAT32 data. */
	FILE *file;
};

/** Called before any other function
 * that uses the disk file.
 * @param diskfile An uninitialized disk file structure.
 * */

void
fat32_diskfile_init(struct fat32_diskfile *diskfile);

/** Called when the disk file is no
 * longer going to be used.
 * @param diskfile An initialized disk file structure.
 * */

void
fat32_diskfile_done(struct fat32_diskfile *diskfile);

/** Opens a file containing FAT32 data.
 * @param diskfile An initialized disk file structure.
 * @param path The path of the disk file.
 * @param mode The mode to open the disk file in.
 * @returns Zero on success, non-zero on failure.
 * */

int
fat32_diskfile_open(struct fat32_diskfile *diskfile,
                    const char *path,
                    const char *mode);

/** Casts the disk file to a disk structure,
 * for use in other functions in the FAT32 library.
 * @param diskfile An initialized disk file structure.
 * @returns The base disk structure of the disk file.
 * This is never @ref NULL.
 * */

struct fat32_disk *
fat32_diskfile_to_disk(struct fat32_diskfile *diskfile);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_FS_FAT32_DISKFILE_H */
