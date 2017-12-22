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

/** Identifies which file system
 * any fs has taken the form of.
 * */

enum any_fs_type {
	/** No active file system. */
	ANY_FS_TYPE_NONE,
	/** The file system is 'ramfs'. */
	ANY_FS_TYPE_RAMFS,
	/** The file system is 'ext4'. */
	ANY_FS_TYPE_EXT4,
	/** The file system is 'host'. */
	ANY_FS_TYPE_HOST
};

/** A variant of all file systems
 * supported by the Swanson project.
 * */

struct any_fs {
	/** The virtual file system callbacks. */
	struct vfs vfs;
	/** Identifies which file system is
	 * currently being used. */
	enum any_fs_type type;
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

void
any_fs_init(struct any_fs *fs);

/** Detect the file system contained
 * on a stream.
 * @param fs The file system stucture
 * to initialize if a file system is detected.
 * @param stream The stream to look for
 * a file system on.
 * @returns Zero if a file system was detected,
 * non-zero if no supported file system was found.
 * */

int
any_fs_detect(struct any_fs *fs,
              struct stream *stream);

#ifdef __cplusplus
} /* extern "C" { */

#endif

#endif /* SWANSON_FS_ANY_H */
