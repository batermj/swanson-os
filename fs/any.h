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

#ifndef SWANSON_FS_ANY_H
#define SWANSON_FS_ANY_H

#include "fs/vfs.h"
#include "fs/ext4.h"
#include "fs/host.h"

#ifdef __cplusplus
extern "C" {
#endif

/** A variant of all file systems
 * supported by the Swanson project.
 * */

struct any_fs {
	/** The virtual file system callbacks. */
	struct vfs vfs;
	/** A union of all the supported file
	 * systems. */
	union {
		/** The ext4 file system. */
		struct ext4_fs ext4;
		/** A host file system. */
		struct host_fs host;
	} u;
};

/** Initializes the file system structure.
 * This should be called before any other
 * functions that use the structure.
 * @param fs The file system structure.
 * */

void any_fs_init(struct any_fs *fs);

#ifdef __cplusplus
} /* extern "C" { */

#endif

#endif /* SWANSON_FS_ANY_H */
