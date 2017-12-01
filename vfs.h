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

#ifndef SWANSON_VFS_H
#define SWANSON_VFS_H

#include "stream.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** A directory entry in the virtual file system. */
struct vfs_entry {
	/** Implementation data */
	void *data;
};

/** A handle for a file in the virtual file system. */
struct vfs_file {
	/** The stream used for reading and writing file
	 * data. */
	struct stream stream;
	/** The offset within the stream that the file
	 * data starts in. */
	uint64_t offset;
	/** The size of the file data, in bytes. */
	uint64_t length;
};

/** A handle for a directory within the virtual file system. */
struct vfs_dir {
	/** Implementation data. */
	void *data;
};

/** Virtual file system. */
struct vfs {
	/** Implementation data. */
	void *data;
};

/** Initializes the virtual file system. */
void vfs_init(struct vfs *vfs);

/** Attempts to open a file from the
 * virtual file system. */
int vfs_open_file(struct vfs *vfs,
                  struct vfs_file *file,
                  const char *path);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_VFS_H */
