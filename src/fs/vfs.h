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

#ifndef SWANSON_VFS_H
#define SWANSON_VFS_H

#include "stream.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Errors that may occur
 * in a virtual file system function.
 * */

enum vfs_error {
	/** No error occured. */
	VFS_ERROR_NONE,
	/** An unknown error occured. */
	VFS_ERROR_UNKNOWN,
	/** Functionality not implemented yet. */
	VFS_ERROR_NOT_IMPLEMENTED
};

/** A handle for a file in the virtual file system.
 * */

struct vfs_file {
	/** Implementation data. */
	void *data;
	/** Get the file name. */
	const char *(*get_name)(void *data);
	/** Get the file size. */
	uintmax_t (*get_size)(void *data);
	/** Read the file data. */
	int (*read)(void *data, void *buf, uintmax_t buf_size);
	/** Write file data. */
	int (*write)(void *data, const void *buf, uintmax_t buf_size);
};

/** Initializes a virtual file system
 * file structure.
 * @param file An uninitialized file structure.
 * */

void
vfs_file_init(struct vfs_file *file);

/** A handle for a directory within the virtual file system.
 * */

struct vfs_dir {
	/** Implementation data. */
	void *data;
	/** Get the directory name. */
	const char *(*get_name)(void *data);
	/** Get a file in the directory. */
	int (*get_file)(void *dir_ptr,
	                uintmax_t index,
	                struct vfs_file *file);
	/** Get the number of entries in the directory. */
	int (*get_file_count)(void *data,
	                      uintmax_t *file_count);
	/** Get a subdirectory in the directory. */
	int (*get_subdir)(void *data,
	                  uintmax_t index,
	                  struct vfs_dir *subdir);
	/** Get the number of subdirectories in
	 * the directory. */
	int (*get_subdir_count)(void *data,
	                        uintmax_t *subdir_count);
};

/** Initializes a virtual file
 * system directory. This function
 * must be called before this structure
 * is used elsewhere.
 * @param dir The directory structure to
 * initialize.
 * */

void
vfs_dir_init(struct vfs_dir *dir);

/** Virtual file system.
 * */

struct vfs {
	/** Implementation data. */
	void *data;
	/** Fetches the root directory. */
	enum vfs_error (*get_root_dir)(void *data, struct vfs_dir *root_dir);
};

/** Initializes the virtual file system.
 * @param vfs An uninitialized virtual file
 * system structure.
 * */

void
vfs_init(struct vfs *vfs);

/** Attempts to open a file from the
 * virtual file system.
 * @param vfs An initialized virtual file
 * system structure.
 * @param file A pointer to the file
 * structure that will be used for read
 * and write operations.
 * @param path The path of the file to open.
 * @returns On success, zero. On failure,
 * a non-zero error code.
 * */

int
vfs_open_file(struct vfs *vfs,
              struct vfs_file *file,
              const char *path);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_VFS_H */
