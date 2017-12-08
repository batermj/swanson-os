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

#include "memmap-test.h"

#include "assert.h"
#include "memmap.h"

#include <stdlib.h>

#ifndef NULL
#define NULL ((void *) 0x00)
#endif

void
memmap_test(void) {
	memmap_test_add();

	/* Skipping this for now.
	 *
	 * memmap_test_alloc();
	 */
}

void
memmap_test_add(void) {

	enum memmap_error err;
	unsigned char *buf;
	unsigned char *buf2;
	unsigned long int buf_size;
	struct memmap memmap;

	/* Allocate the first section
	 * of memory that will be used
	 * for the memory map. */

	buf_size = sizeof(struct memmap_section) * 3;

	buf = malloc(buf_size);
	assert(buf != NULL);

	/* Initialize the memory map. */

	memmap_init(&memmap);

	/* This function should fail
	 * if the section does not contain
	 * any bytes. */

	err = memmap_add(&memmap, buf, 0);
	assert(err == MEMMAP_ERROR_NEED_SPACE);

	/* This function should fail if the
	 * first section of memory is not large
	 * enough to bootstrap the memory map. */

	err = memmap_add(&memmap, buf, (sizeof(struct memmap_section) * 3) - 1);
	assert(err == MEMMAP_ERROR_NEED_SPACE);

	/* This function should pass because
	 * the buffer is large enough. */

	err = memmap_add(&memmap, buf, buf_size);
	assert(err == 0);

	/* Memory map:
	 *   buf:
	 *     0x01 : unused section entry
	 *     0x02 : used section entry
	 *     0x03 : used section entry
	 * */

	assert(memmap.unused_section_count == 1);
	assert(memmap.unused_section_array[0].addr == buf);
	assert(memmap.unused_section_array[0].size == buf_size);

	assert(memmap.used_section_count == 2);
	assert(memmap.used_section_array[0].addr == buf);
	assert(memmap.used_section_array[0].size == sizeof(struct memmap_section));
	assert(memmap.used_section_array[1].addr == &buf[sizeof(struct memmap_section)]);
	assert(memmap.used_section_array[1].size == (sizeof(struct memmap_section) * 2));

	/* Allocate second section of memory
	 * for the memory map. */

	buf2 = malloc(buf_size * 2);
	assert(buf2 != NULL);

	/* Add the second section to memory. */

	err = memmap_add(&memmap, buf2, buf_size * 2);
	assert(err == MEMMAP_ERROR_NONE);

	/* Memory map:
	 *   buf:
	 *     0x01 :
	 *     0x02 : used section entry
	 *     0x03 : used section entry
	 *   buf2:
	 *     0x00 : unused section entry
	 *     0x01 : unused section entry
	 * */

	/* Since the contents of the unused and
	 * the used section array are going to be
	 * resized, check that they were resized
	 * correctly. */

	assert(memmap.unused_section_count == 2);
	assert(memmap.unused_section_array[0].addr == buf);
	assert(memmap.unused_section_array[0].size == buf_size);
	assert(memmap.unused_section_array[1].addr == buf2);
	assert(memmap.unused_section_array[1].size == (buf_size * 2));

	/* All done with the memory map. */

	free(buf2);

	free(buf);
}

void
memmap_test_alloc(void) {

	char section_one[64];
	char section_two[64];
	struct memmap_section unused_section_array[2];
	unsigned long int unused_section_count;
	struct memmap memmap;
	void *addr;

	unused_section_array[0].addr = section_one;
	unused_section_array[0].size = sizeof(section_one);

	unused_section_array[1].addr = section_two;
	unused_section_array[1].size = sizeof(section_two);

	unused_section_count = 2;

	memmap_init(&memmap);

	memmap.unused_section_array = unused_section_array;
	memmap.unused_section_count = unused_section_count;

	// addr = memmap_alloc(&memmap, 1);
	addr = NULL;
	/* one section for the address that was allocated,
	 * and one section for the used section array itself */
	assert(memmap.used_section_count == 2);
	/* memmap uses the first address in the first
	 * unused section to account for used sections. */
	assert(memmap.used_section_array == ((void *) section_one));
	assert(memmap.used_section_array[0].addr == section_one);
	assert(memmap.used_section_array[0].size == (sizeof(struct memmap_section) * 2));
	assert(memmap.used_section_array[1].addr == &section_one[sizeof(struct memmap_section) * 2]);
	assert(memmap.used_section_array[1].size == 1);
	assert(addr == memmap.used_section_array[1].addr);
}

