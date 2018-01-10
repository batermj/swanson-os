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
	uint32_t ph_offset;
	uint16_t ph_count;
	uint16_t ph_size;
	uint16_t ph_index;
	uint8_t endianness;
	uint8_t elf_class;
	uint32_t segment_type;
	uint32_t segment_flags;
	uint32_t segment_file_offset;
	uint32_t segment_file_size;
	uint32_t segment_addr;
	uint32_t segment_addr_size;
	uint32_t paddr;
	struct elf_segment segment;

	/* Verify ELF signature */

	err = stream_setpos(stream, 0x00);
	if (err != 0)
		return err;

	read_count = stream_read(stream, signature, 4);
	if (read_count != 4)
		return -1;

	if ((signature[0] != 0x7f)
	 || (signature[1] != 'E')
	 || (signature[2] != 'L')
	 || (signature[3] != 'F'))
		return -1;

	/* Verify class is 32-bit */

	err = stream_setpos(stream, 0x04);
	if (err != 0)
		return err;

	read_count = stream_read(stream, &elf_class, 1);
	if (read_count != 1)
		return -1;

	if (elf_class != 1) {
		/* 32-bit: 1 */
		/* 64-bit: 2 */
		return -1;
	}

	/* Verify endianness is big-endian */

	err = stream_setpos(stream, 0x05);
	if (err != 0)
		return err;

	read_count = stream_read(stream, &endianness, 1);
	if (read_count != 1)
		return -1;

	if (endianness != 2) {
		/* 1 : little endian */
		/* 2 : big endian */
		return -1;
	}

	/* Verify machine type is Moxie */

	err = stream_setpos(stream, 0x12);
	if (err != 0)
		return err;

	read_count = stream_decode_uint16be(stream, &machine_type);
	if (read_count != 2)
		return -1;
	else if (machine_type != 0xdf)
		return -1;

	/* Get program header offset */

	err = stream_setpos(stream, 0x1c);
	if (err != 0)
		return err;

	ph_offset = 0;

	read_count = stream_decode_uint32be(stream, &ph_offset);
	if (read_count != 4)
		return -1;

	/* Verify program header size */

	err = stream_setpos(stream, 0x2a);
	if (err != 0)
		return err;

	read_count = stream_decode_uint16be(stream, &ph_size);
	if (read_count != 2)
		return -1;

	/* Get program header count */

	err = stream_setpos(stream, 0x2c);
	if (err != 0)
		return err;

	read_count = stream_decode_uint16be(stream, &ph_count);
	if (read_count != 2)
		return -1;

	/* Read program headers */

	for (ph_index = 0; ph_index < ph_count; ph_index++) {

		elf_segment_init(&segment);

		err = stream_setpos(stream, ph_offset + (ph_index * ph_size));
		if (err != 0)
			return err;

		read_count = stream_decode_uint32be(stream, &segment_type);
		if (read_count != 4)
			continue;

		if (segment_type == 0x01) {
			/* LOAD : Segment contains loadable data */
		} else if (segment_type == 0x02) {
			/* DYNAMIC : Segment contains dynamic linking information */
			continue;
		} else if (segment_type == 0x03) {
			/* INTERP : Segment contains path to interpreter */
			continue;
		} else {
			continue;
		}

		read_count = stream_decode_uint32be(stream, &segment_file_offset);
		if (read_count != 4)
			continue;

		read_count = stream_decode_uint32be(stream, &segment_addr);
		if (read_count != 4)
			continue;

		/* physical address is read but ignored afterwards */
		read_count = stream_decode_uint32be(stream, &paddr);
		if (read_count != 4)
			continue;

		read_count = stream_decode_uint32be(stream, &segment_file_size);
		if (read_count != 4)
			continue;

		read_count = stream_decode_uint32be(stream, &segment_addr_size);
		if (read_count != 4)
			continue;

		read_count = stream_decode_uint32be(stream, &segment_flags);
		if (read_count != 4)
			continue;

		if (segment_flags & 0x01)
			segment.execute_permission = true;

		if (segment_flags & 0x02)
			segment.write_permission = true;

		if (segment_flags & 0x04)
			segment.read_permission = true;

		/* if the segment's file size is greater than
		 * the size it occupies in memory, this segment
		 * may be corrupt on file */
		if (segment_file_size > segment_addr_size)
			continue;

		segment.data = calloc(1, segment_addr_size);
		if (segment.data == NULL)
			return -1;

		segment.data_size = segment_addr_size;

		segment.virtual_address = segment_addr;

		err = stream_setpos(stream, segment_file_offset);
		if (err != 0) {
			elf_segment_free(&segment);
			return err;
		}

		read_count = stream_read(stream, segment.data, segment_file_size);
		if (read_count != segment_file_size) {
			elf_segment_free(&segment);
			return -1;
		}

		err = elf_file_push_segment(file, &segment);
		if (err != 0) {
			return -1;
		}

		elf_segment_free(&segment);
	}

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
