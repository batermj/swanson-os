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

#include "elf.h"

#include "stream.h"

#include <stdlib.h>
#include <string.h>

void
elf_segment_init(struct elf_segment *segment) {
	segment->data = NULL;
	segment->data_size = 0;
	segment->read_permission = false;
	segment->write_permission = false;
	segment->execute_permission = false;
}

void
elf_segment_free(struct elf_segment *segment) {
	free(segment->data);
	segment->data = NULL;
	segment->data_size = 0;
}

int
elf_segment_copy(struct elf_segment *dst,
                 const struct elf_segment *src) {

	/* free any data currently allocated
	 * by destination segment */

	elf_segment_free(dst);

	memcpy(dst, src, sizeof(*dst));

	dst->data = malloc(src->data_size);
	if (dst->data == NULL)
		return -1;

	memcpy(dst->data, src->data, src->data_size);

	dst->data_size = src->data_size;

	return 0;
}

void
elf_file_init(struct elf_file *file) {
	file->segment_array = NULL;
	file->segment_count = 0;
}

void
elf_file_free(struct elf_file *file) {

	uintmax_t i;

	for (i = 0; i < file->segment_count; i++)
		elf_segment_free(&file->segment_array[i]);

	free(file->segment_array);
	file->segment_array = NULL;
	file->segment_count = 0;
}

int
elf_file_decode(struct elf_file *file,
                struct stream *stream) {

	int err;
	unsigned long int read_count;
	unsigned char signature[4];
	uint16_t machine_type;

	read_count = stream_read(stream, signature, 4);
	if (read_count != 4)
		return -1;

	if ((signature[0] != 0x7f)
	 || (signature[1] != 'E')
	 || (signature[2] != 'L')
	 || (signature[3] != 'F'))
		return -1;

	err = stream_setpos(stream, 0x12);
	if (err != 0)
		return err;

	read_count = stream_decode_uint16le(stream, &machine_type);
	if (read_count != 2)
		return -1;
	else if (machine_type != 0xdf00)
		return -1;

	(void) file;

	return 0;
}

int
elf_file_push_segment(struct elf_file *file,
                      const struct elf_segment *segment) {

	int err;
	struct elf_segment *segment_array;
	uintmax_t segment_array_size;

	segment_array = file->segment_array;

	segment_array_size = file->segment_count + 1;
	segment_array_size *= sizeof(struct elf_segment);

	segment_array = realloc(segment_array, segment_array_size);
	if (segment_array == NULL)
		return -1;

	file->segment_array = segment_array;

	elf_segment_init(&segment_array[file->segment_count]);

	err = elf_segment_copy(&segment_array[file->segment_count],
	                       segment);
	if (err != 0)
		return err;

	file->segment_count++;

	return 0;
}
