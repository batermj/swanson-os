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

#include "fs.h"

#include "path.h"

#include <string.h>

#ifndef NULL
#define NULL ((void *) 0x00)
#endif

void
ramfs_init(struct ramfs *ramfs) {
	vfs_init(&ramfs->vfs);
	ramfs_dir_init(&ramfs->root_dir);
}

void
ramfs_free(struct ramfs *ramfs) {
	ramfs_dir_free(&ramfs->root_dir);
}

unsigned long int
ramfs_import(struct ramfs *ramfs,
             const void *data,
             unsigned long int data_size) {

	return ramfs_dir_import(&ramfs->root_dir, data, data_size);
}

unsigned long int
ramfs_export(const struct ramfs *ramfs,
             struct stream *stream) {

	return ramfs_dir_export(&ramfs->root_dir, stream);
}

int
ramfs_mkdir(struct ramfs *ramfs,
            const char *path_str) {

	int err;
	const char *name;
	unsigned int name_count;
	unsigned int i;
	unsigned int j;
	struct path path;
	struct ramfs_dir *parent_dir;
	struct ramfs_dir *subdir;

	path_init(&path);

	err = path_parse(&path, path_str);
	if (err != 0) {
		path_free(&path);
		return -1;
	}

	parent_dir = &ramfs->root_dir;

	name_count = path_get_name_count(&path);

	if (name_count == 0) {
		path_free(&path);
		return -1;
	}

	for (i = 0; i < (name_count - 1); i++) {

		name = path_get_name(&path, i);
		if (name == NULL) {
			path_free(&path);
			return -1;
		}

		for (j = 0; j < parent_dir->subdir_count; i++) {
			subdir = &parent_dir->subdir_array[i];
			if (subdir == NULL) {
				continue;
			} else if (strcmp(subdir->name, name) == 0) {
				parent_dir = subdir;
				break;
			}
		}

		if (j >= parent_dir->subdir_count) {
			/* not found */
			path_free(&path);
			return -1;
		}
	}

	if (i != (name_count - 1)) {
		path_free(&path);
		return -1;
	}

	err = ramfs_dir_add_subdir(parent_dir, path.name_array[i].data);
	if (err != 0) {
		path_free(&path);
		return -1;
	}

	path_free(&path);

	return 0;
}
