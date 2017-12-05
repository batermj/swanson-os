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

#include "host.h"

#ifndef NULL
#define NULL ((void *) 0x00)
#endif

static enum vfs_error
host_fs_get_root_dir(void *fs_ptr,
                     struct vfs_dir *root_dir) {

	struct host_fs *fs;

	fs = (struct host_fs *) fs_ptr;

	*root_dir = fs->root_dir.dir;

	return VFS_ERROR_NONE;
}

void
host_dir_init(struct host_dir *dir) {
	vfs_dir_init(&dir->dir);
	dir->fs = NULL;
}

void
host_fs_init(struct host_fs *fs) {
	vfs_init(&fs->vfs);
	fs->vfs.get_root_dir = host_fs_get_root_dir;
	fs->root_path = "";
	host_dir_init(&fs->root_dir);
}
