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
#include "stream.h"

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

	unsigned long int i;

	for (i = 0; i < dir->subdir_count; i++)
		ramfs_dir_free(&dir->subdir_array[i]);

	for (i = 0; i < dir->file_count; i++)
		ramfs_file_free(&dir->file_array[i]);

	free(dir->subdir_array);
	free(dir->file_array);
	free(dir->name);
}

int
ramfs_dir_add_subdir(struct ramfs_dir *dir,
                     const char *name) {

	int err;
	struct ramfs_dir *subdir_array;
	unsigned long int subdir_array_size;

	if (ramfs_dir_name_exists(dir, name))
		return -1;

	subdir_array_size = dir->subdir_count + 1;
	subdir_array_size *= sizeof(dir->subdir_array[0]);

	subdir_array = dir->subdir_array;

	subdir_array = realloc(subdir_array, subdir_array_size);
	if (subdir_array == NULL) {
		return -1;
	}

	ramfs_dir_init(&subdir_array[dir->subdir_count]);

	err = ramfs_dir_set_name(&subdir_array[dir->subdir_count], name);
	if (err != 0) {
		ramfs_dir_free(&subdir_array[dir->subdir_count]);
		return -1;
	}

	dir->subdir_array = subdir_array;
	dir->subdir_count++;

	return 0;
}

int
ramfs_dir_name_exists(const struct ramfs_dir *dir,
                      const char *name) {

	unsigned int i;

	for (i = 0; i < dir->file_count; i++) {
		if (strcmp(dir->file_array[i].name, name) == 0)
			return 1;
	}

	for (i = 0; i < dir->subdir_count; i++) {
		if (strcmp(dir->subdir_array[i].name, name) == 0)
			return 1;
	}

	return 0;
}

unsigned long int
ramfs_dir_decode(struct ramfs_dir *dir,
                 struct stream *stream) {

	unsigned long int i;
	unsigned long int read_count;
	uint32_t name_size;
	uint32_t file_count;
	uint32_t subdir_count;

	/* release memory that may be
	 * currently occupied by the directory */

	ramfs_dir_free(dir);
	ramfs_dir_init(dir);

	name_size = 0;
	file_count = 0;
	subdir_count = 0;

	read_count = 0;
	read_count += stream_decode_uint32le(stream, &name_size);
	read_count += stream_decode_uint32le(stream, &file_count);
	read_count += stream_decode_uint32le(stream, &subdir_count);

	dir->name_size = name_size;
	dir->file_count = file_count;
	dir->subdir_count = subdir_count;

	dir->name = malloc(name_size + 1);

	dir->file_array = malloc(dir->file_count * sizeof(struct ramfs_file));

	dir->subdir_array = malloc(dir->subdir_count * sizeof(struct ramfs_dir));

	if ((dir->name == NULL)
	 || (dir->file_array == NULL)
	 || (dir->subdir_array == NULL)) {
		free(dir->name);
		free(dir->file_array);
		free(dir->subdir_array);
		dir->name = NULL;
		dir->file_array = NULL;
		dir->subdir_array = NULL;
		return read_count;
	}

	read_count += stream_read(stream, dir->name, dir->name_size);

	dir->name[dir->name_size] = 0;

	/* Initialize the file and subdirectory structures */

	for (i = 0; i < dir->file_count; i++)
		ramfs_file_init(&dir->file_array[i]);

	for (i = 0; i < dir->subdir_count; i++)
		ramfs_dir_init(&dir->subdir_array[i]);

	/* Import the files and subdirectories */

	for (i = 0; i < dir->file_count; i++)
		read_count += ramfs_file_decode(&dir->file_array[i], stream);

	for (i = 0; i < dir->subdir_count; i++)
		read_count += ramfs_dir_decode(&dir->subdir_array[i], stream);

	return read_count;
}

unsigned long int
ramfs_dir_encode(const struct ramfs_dir *dir,
                 struct stream *stream) {

	unsigned int i;
	unsigned long int write_count;

	write_count = 0;

	write_count += stream_encode_uint32le(stream, dir->name_size);
	write_count += stream_encode_uint32le(stream, dir->file_count);
	write_count += stream_encode_uint32le(stream, dir->subdir_count);

	write_count += stream_write(stream, dir->name, dir->name_size);

	for (i = 0; i < dir->file_count; i++)
		write_count += ramfs_file_encode(&dir->file_array[i], stream);

	for (i = 0; i < dir->subdir_count; i++)
		write_count += ramfs_dir_encode(&dir->subdir_array[i], stream);

	return write_count;
}

int
ramfs_dir_set_name(struct ramfs_dir *dir,
                   const char *name) {

	char *tmp_name;
	unsigned long int name_size;

	name_size = strlen(name);

	tmp_name = malloc(name_size + 1);
	if (tmp_name == NULL)
		return -1;

	memcpy(tmp_name, name, name_size);

	tmp_name[name_size] = 0;

	free(dir->name);

	dir->name = tmp_name;
	dir->name_size = name_size;

	return 0;
}
