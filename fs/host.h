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

#ifndef SWANSON_FS_HOST_H
#define SWANSON_FS_HOST_H

#include "fs/vfs.h"

#ifdef __cplusplus
extern "C" {
#endif

/** A file system located in the
 * host operating system.
 * */

struct host_fs {
	/** The virtual file system
	 * callbacks. */
	struct vfs vfs;
	/** The path of the directory
	 * that the host file system
	 * will be located at. */
	const char *root_path;
};

/** Initializes a host file system.
 * @param fs An uninitialized host
 * file system structure.
 * */

void host_fs_init(struct host_fs *fs);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_FS_HOST_H */
