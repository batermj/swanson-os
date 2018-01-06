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

#include "file.h"

#include "stream.h"

#include <stdlib.h>
#include <string.h>

void
ramfs_file_init(struct ramfs_file *file) {
	file->name_size = 0;
	file->data_size = 0;
	file->name = NULL;
	file->data = NULL;
}

void
ramfs_file_free(struct ramfs_file *file) {
	free(file->name);
	free(file->data);
}

unsigned long int
ramfs_file_decode(struct ramfs_file *file,
                  struct stream *stream) {

	unsigned long int read_count;
	uint32_t name_size;
	uint32_t data_size;

	ramfs_file_free(file);
	ramfs_file_init(file);

	read_count = 0;

	read_count += stream_decode_uint32le(stream, &name_size);
	read_count += stream_decode_uint32le(stream, &data_size);

	file->name_size = name_size;
	file->data_size = data_size;

	file->name = malloc(file->name_size + 1);

	file->data = malloc(file->data_size);

	if ((file->name == NULL) || (file->data == NULL)) {
		free(file->name);
		free(file->data);
		file->name = NULL;
		file->data = NULL;
		file->name_size = 0;
		file->data_size = 0;
		return read_count;
	}

	read_count += stream_read(stream, file->name, file->name_size);

	file->name[file->name_size] = 0;

	read_count += stream_read(stream, file->data, file->data_size);

	return read_count;
}

unsigned long int
ramfs_file_encode(const struct ramfs_file *file,
                  struct stream *stream) {

	unsigned long int write_count;

	write_count = 0;

	write_count += stream_encode_uint32le(stream, file->name_size);
	write_count += stream_encode_uint32le(stream, file->data_size);

	write_count += stream_write(stream, file->name, file->name_size);

	write_count += stream_write(stream, file->data, file->data_size);

	return write_count;
}

const char *
ramfs_file_get_name(const struct ramfs_file *file) {
	return file->name;
}

unsigned long int
ramfs_file_get_name_size(const struct ramfs_file *file) {
	return file->name_size;
}

void *
ramfs_file_get_data(struct ramfs_file *file) {
	return file->data;
}

unsigned long int
ramfs_file_get_size(const struct ramfs_file *file) {
	return file->data_size;
}
