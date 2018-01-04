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

#include "dir.h"

#include "file.h"

#include <stdlib.h>
#include <string.h>

void
ramfs_dir_init(struct ramfs_dir *dir) {
	dir->name_size = 0;
	dir->file_count = 0;
	dir->subdir_count = 0;
	dir->name = NULL;
	dir->file_array = NULL;
	dir->subdir_array = NULL;
}

void
ramfs_dir_free(struct ramfs_dir *dir) {
	free(dir->subdir_array);
	free(dir->file_array);
	free(dir->name);
}

unsigned long int
ramfs_dir_import(struct ramfs_dir *dir,
                 const void *data,
                 unsigned long int data_size) {

	const unsigned char *data8;
	unsigned long int initial_data_size;
	unsigned long int read_size;
	unsigned long int file_array_size;
	unsigned long int subdir_array_size;
	unsigned long int i;

	if (data_size < 12) {
		/* twelve bytes is the number
		 * of bytes taken up by the
		 * name size, file count, and
		 * subdirectory count. */
		return 0;
	}

	initial_data_size = data_size;

	data8 = (const unsigned char *) data;

	dir->name_size = 0;
	dir->name_size |= data8[0];
	dir->name_size |= data8[1] << 0x08;
	dir->name_size |= data8[2] << 0x10;
	dir->name_size |= data8[3] << 0x18;

	/* skip passed the name size */
	data8 = &data8[4];
	data_size -= 4;

	dir->file_count = 0;
	dir->file_count |= data8[0];
	dir->file_count |= data8[1] << 0x08;
	dir->file_count |= data8[2] << 0x10;
	dir->file_count |= data8[3] << 0x18;

	/* skip passed the file count */
	data8 = &data8[4];
	data_size -= 4;

	dir->subdir_count = 0;
	dir->subdir_count |= data8[0];
	dir->subdir_count |= data8[1] << 0x08;
	dir->subdir_count |= data8[2] << 0x10;
	dir->subdir_count |= data8[3] << 0x18;

	/* skip passed the sub directory count */
	data8 = &data8[4];
	data_size -= 4;

	/* there should be enough data for
	 * the name left over */
	if (data_size < dir->name_size) {
		return 0;
	}

	dir->name = malloc(dir->name_size);
	if (dir->name == NULL) {
		return 0;
	}

	memcpy(dir->name, data8, dir->name_size);

	data8 = &data8[dir->name_size];
	data_size -= dir->name_size;

	file_array_size = dir->file_count * sizeof(struct ramfs_file);

	if (data_size < file_array_size) {
		free(dir->name);
		return 0;
	}

	dir->file_array = malloc(file_array_size);
	if (dir->file_array == NULL) {
		free(dir->name);
		return 0;
	}

	data8 = &data8[file_array_size];
	data_size -= file_array_size;

	subdir_array_size = dir->subdir_count * sizeof(struct ramfs_dir);

	if (data_size < subdir_array_size) {
		free(dir->name);
		free(dir->file_array);
		return 0;
	}

	dir->subdir_array = malloc(subdir_array_size);
	if (dir->subdir_array == NULL) {
		free(dir->name);
		free(dir->file_array);
		return 0;
	}

	/* Initialize the file and subdirectory structures */

	for (i = 0; i < dir->file_count; i++)
		ramfs_file_init(&dir->file_array[i]);

	for (i = 0; i < dir->subdir_count; i++)
		ramfs_dir_init(&dir->subdir_array[i]);

	/* Import the files and subdirectories */

	for (i = 0; i < dir->file_count; i++) {

		read_size = ramfs_file_import(&dir->file_array[i], data8, data_size);
		if (read_size <= 0) {
			ramfs_dir_free(dir);
			return 0;
		}

		data_size -= read_size;

		data8 = &data8[read_size];
	}

	for (i = 0; i < dir->subdir_count; i++) {

		read_size = ramfs_dir_import(&dir->subdir_array[i], data8, data_size);
		if (read_size <= 0) {
			ramfs_dir_free(dir);
			return 0;
		}

		data_size -= read_size;

		data8 = &data8[read_size];
	}

	return initial_data_size - data_size;
}
