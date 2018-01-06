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

unsigned long int
ramfs_file_import(struct ramfs_file *file,
                  const void *data,
                  unsigned long int data_size) {

	const unsigned char *data8;

	data8 = (const unsigned char *) data;

	if (data_size < 8) {
		/* the name size and the data
		 * size fields take up eight bytes */
		return 0;
	}

	file->name_size = 0;
	file->name_size |= data8[0];
	file->name_size |= data8[1] << 0x08;
	file->name_size |= data8[2] << 0x10;
	file->name_size |= data8[3] << 0x18;

	file->data_size = 0;
	file->name_size |= data8[4];
	file->name_size |= data8[5] << 0x08;
	file->name_size |= data8[6] << 0x10;
	file->name_size |= data8[7] << 0x18;

	data8 = &data8[8];

	data_size -= 8;

	/* Make sure there's data that fits the
	 * name and file data. */
	if (data_size < (file->name_size + file->data_size)) {
		return 0;
	}

	file->name = malloc(file->name_size);

	file->data = malloc(file->data_size);

	if ((file->name == NULL) || (file->data == NULL)) {
		free(file->name);
		free(file->data);
		return 0;
	}

	memcpy(file->name, data8, file->name_size);

	memcpy(file->data, &data8[file->name_size], file->data_size);

	return 8 + file->name_size + file->data_size;
}

unsigned long int
ramfs_file_export(const struct ramfs_file *file,
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
