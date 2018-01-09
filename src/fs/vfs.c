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

#include "vfs.h"

#ifndef NULL
#define NULL ((void *) 0x00)
#endif

void
vfs_file_init(struct vfs_file *file) {
	file->data = NULL;
	file->get_name = NULL;
	file->get_size = NULL;
	file->read = NULL;
	file->write = NULL;
}

void
vfs_dir_init(struct vfs_dir *dir) {
	dir->data = NULL;
	dir->get_name = NULL;
	dir->get_file = NULL;
	dir->get_file_count = NULL;
	dir->get_subdir = NULL;
	dir->get_subdir_count = NULL;
}

void
vfs_init(struct vfs *vfs) {
	vfs->data = NULL;
	vfs->get_root_dir = NULL;
}

int
vfs_open_file(struct vfs *vfs,
              struct vfs_file *file,
                  const char *path) {
	(void) vfs;
	(void) file;
	(void) path;
	return -1;
}
