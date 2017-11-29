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

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct vfs_entry {
	void *data;
};

struct vfs_stream {
	/** Implementation data. */
	void *data;
	/** Sets the position of the stream. */
	int (*setpos)(void *data, uint64_t pos);
	/** Gets the position of the stream. */
	int (*getpos)(void *data, uint64_t *pos);
	/** Writes data to the stream. */
	int (*write)(void *data,
	             const void *buf,
	             uint64_t buf_size,
	             uint64_t *write_size);
	/** Reads data from the stream. */
	int (*read)(void *data,
	            void *buf,
	            uint64_t buf_size,
	            uint64_t *read_size);
};

struct vfs_file {
	struct vfs_stream stream;
	uint64_t offset;
	uint64_t length;
};

struct vfs_dir {
	void *data;
};

/** Virtual file system. */
struct vfs {
	/** Implementation data. */
	void *data;
	/** Root directory */
	struct vfs_dir root_dir;
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
