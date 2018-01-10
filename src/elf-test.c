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

#include "elf-test.h"

#include "assert.h"
#include "elf.h"
#include "elf-data.h"
#include "rstream.h"

#include <string.h>

const unsigned char segment_data[] = {
	0x7f, 0x45, 0x4c, 0x46, 0x01, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x02, 0x00, 0xdf, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x34,
	0x00, 0x00, 0x01, 0x68, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 0x00, 0x20, 0x00, 0x01, 0x00, 0x28,
	0x00, 0x06, 0x00, 0x05, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xac,
	0x00, 0x00, 0x0f, 0xac, 0x00, 0x00, 0x00, 0x58, 0x00, 0x00, 0x00, 0x58, 0x00, 0x00, 0x00, 0x05,
	0x00, 0x00, 0x00, 0x01, 0x0f, 0x00, 0x04, 0x00
};

static void
elf_test_decode(void) {

	int err;
	struct rstream elf_rstream;
	struct stream *elf_stream;
	struct elf_file elf_file;

	rstream_init(&elf_rstream);

	rstream_setbuf(&elf_rstream,
	               elf_data,
	               elf_data_size);

	elf_stream = rstream_to_stream(&elf_rstream);

	elf_file_init(&elf_file);

	err = elf_file_decode(&elf_file, elf_stream);
	assert(err == 0);
	assert(elf_file.segment_count == 1);
	assert(elf_file.segment_array[0].data_size == 88);
	assert(elf_file.segment_array[0].data_size == sizeof(segment_data));
	assert(memcmp(elf_file.segment_array[0].data, segment_data, sizeof(segment_data)) == 0);

	elf_file_free(&elf_file);
}

void
elf_test(void) {
	elf_test_decode();
}
