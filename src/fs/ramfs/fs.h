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

/** Decodes a ramfs from a stream.
 * @param ramfs An initialized ramfs structure.
 * @param stream The stream to read the ramfs from.
 * @returns The number of bytes read from the stream.
 * */

unsigned long int
ramfs_decode(struct ramfs *ramfs,
             struct stream *stream);

/** Encodes a ramfs to a stream structure.
 * @param ramfs An initialized ramfs structure.
 * @param stream The stream to decode the ramfs to.
 * @returns The number of bytes written to the stream.
 * */

unsigned long int
ramfs_encode(const struct ramfs *ramfs,
             struct stream *stream);

/** Creates a directory.
 * @param ramfs An initialized ramfs structure.
 * @param path_str The string of the new directory's path.
 * @returns Zero on success, non-zero on failure.
 * */

int
ramfs_make_dir(struct ramfs *ramfs,
               const char *path_str);

/** Creates a file.
 * @param ramfs An initialized ramfs structure.
 * @param path_str The string of the new file's path.
 * @returns Zero on success, non-zero on failure.
 * */

int
ramfs_make_file(struct ramfs *ramfs,
                const char *path_str);

/** Opens a directory from the file system.
 * @param ramfs An initialized ramfs structure.
 * @param path The path of the directory to open.
 * @returns The directory, if it is found.
 * A null pointer is returned if the directory isn't
 * found or if it is a file instead.
 * */

struct ramfs_dir *
ramfs_open_dir(struct ramfs *ramfs,
               const char *path);

/** Opens a file from the file system.
 * @param ramfs An initialized ramfs structure.
 * @param path The path of the file to open.
 * @returns The file, if it is found.
 * A null pointer is returned if the file isn't
 * found or if it is a directory instead.
 * */

struct ramfs_file *
ramfs_open_file(struct ramfs *ramfs,
                const char *path);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_FS_RAMFS_H */
