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

#ifndef SWANSON_FS_RAMFS_H
#define SWANSON_FS_RAMFS_H

#include "../vfs.h"

#include "dir.h"

#ifdef __cplusplus
extern "C" {
#endif

struct stream;

/** A ramfs file system structure.
 * Used by the kernel to find files
 * required to start the operating
 * system.
 * */

struct ramfs {
	/** The virtual file system bindings. */
	struct vfs vfs;
	/** The root directory of the ram file system. */
	struct ramfs_dir root_dir;
};

/** Initializes the ramfs.
 * @param ramfs An uninitialized ramfs.
 * */

void
ramfs_init(struct ramfs *ramfs);

/** Releases resources allocated
 * by the ramfs.
 * */

void
ramfs_free(struct ramfs *ramfs);

/** Imports a ramfs from a block of memory.
 * @param ramfs An initialized ramfs structure.
 * @param data A pointer to the memory block
 * containing the ramfs.
 * @param data_size The number of bytes associated
 * with the block of memory.
 * */

unsigned long int
ramfs_import(struct ramfs *ramfs,
             const void *data,
             unsigned long int data_size);

/** Exports a ramfs to a stream structure.
 * @param ramfs An initialized ramfs structure.
 * @param stream The stream to export the ramfs to.
 * @returns The number of bytes written to the stream.
 * */

unsigned long int
ramfs_export(const struct ramfs *ramfs,
             struct stream *stream);

/** Creates a directory.
 * @param ramfs An initialized ramfs structure.
 * @param path_str The string of the new directory's path.
 * @returns Zero on success, non-zero on failure.
 * */

int
ramfs_mkdir(struct ramfs *ramfs,
            const char *path_str);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_FS_RAMFS_H */
