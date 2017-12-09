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

#define ADDR(addr) ((void *) addr)

void
memmap_test(void) {
	memmap_test_free();
	memmap_test_find();
	memmap_test_add();
	memmap_test_alloc();
}

void
memmap_test_free(void) {

	unsigned char *buf;
	struct memmap memmap;
	unsigned char *addr1;
	unsigned char *addr2;
	unsigned char *addr3;
	unsigned char *addr4;

	buf = malloc(1024);
	assert(buf != NULL);

	memmap_init(&memmap);

	memmap.unused_section_array = (struct memmap_section *) buf;
	memmap.unused_section_array[0].addr = buf;
	memmap.unused_section_array[0].size = 1024;
	memmap.unused_section_count = 1;

	memmap.used_section_array = (struct memmap_section *) &buf[sizeof(struct memmap_section)];
	memmap.used_section_array[0].addr = memmap.unused_section_array;
	memmap.used_section_array[0].size = sizeof(struct memmap_section);
	memmap.used_section_array[1].addr = memmap.used_section_array;
	memmap.used_section_array[1].size = sizeof(struct memmap_section) * 6;
	memmap.used_section_array[2].addr = &buf[sizeof(struct memmap_section) * 7];
	memmap.used_section_array[2].size = 1;
	memmap.used_section_array[3].addr = &buf[sizeof(struct memmap_section) * 7];
	memmap.used_section_array[3].size = 1;
	memmap.used_section_array[4].addr = &buf[sizeof(struct memmap_section) * 7];
	memmap.used_section_array[4].size = 1;
	memmap.used_section_array[5].addr = &buf[sizeof(struct memmap_section) * 7];
	memmap.used_section_array[5].size = 1;
	memmap.used_section_count = 6;

	addr1 = memmap.used_section_array[2].addr;
	addr2 = memmap.used_section_array[3].addr;
	addr3 = memmap.used_section_array[4].addr;
	addr4 = memmap.used_section_array[5].addr;

	memmap_free(&memmap, addr2);

	assert(memmap.used_section_count == 5);
	assert(memmap.used_section_array[2].addr == addr1);
	assert(memmap.used_section_array[4].addr == addr3);
	assert(memmap.used_section_array[5].addr == addr4);

	memmap_free(&memmap, addr4);

	assert(memmap.used_section_count == 4);
	assert(memmap.used_section_array[2].addr == addr1);
	assert(memmap.used_section_array[4].addr == addr3);

	memmap_free(&memmap, addr1);

	assert(memmap.used_section_count == 3);
	assert(memmap.used_section_array[4].addr == addr3);

	memmap_free(&memmap, addr3);

	assert(memmap.used_section_count == 2);

	/* Just a check to make sure freeing
	 * an address that does not exist does
	 * not create some strange error. This
	 * may show up on valgrind if it does. */

	memmap_free(&memmap, ADDR(0x49));

	assert(memmap.used_section_count == 2);

	free(buf);
}

void
memmap_test_find(void) {

	unsigned char *addr;
	unsigned char buf1[256];
	unsigned char buf2[256];

	struct memmap memmap;
	struct memmap_section unused_sections[2];
	struct memmap_section used_sections[4];

	/* Memory map:
	 *   buf1:
	 *     0x00 : used
	 *     0x08 : free
	 *     0x10 : used
	 *     0x20 : free
	 *     0x40 : used
	 *   buf2:
	 *     0x000 : used
	 *     0x002 : free
	 * */

	/* Setup the unused section array. */

	memmap_section_init(&unused_sections[0]);
	memmap_section_set(&unused_sections[0], buf1, sizeof(buf1));

	memmap_section_init(&unused_sections[1]);
	memmap_section_set(&unused_sections[1], buf2, sizeof(buf2));

	/* Setup the used section array */

	memmap_section_init(&used_sections[0]);
	memmap_section_set(&used_sections[0], buf1, 0x08);

	memmap_section_init(&used_sections[1]);
	memmap_section_set(&used_sections[1], &buf1[0x10], 0x10);

	memmap_section_init(&used_sections[2]);
	memmap_section_set(&used_sections[2], &buf1[0x40], 256 - 0x40);

	memmap_section_init(&used_sections[3]);
	memmap_section_set(&used_sections[3], &buf2[0x00], 0x02);

	/* Initialize the memory map. */

	memmap_init(&memmap);
	memmap.unused_section_array = unused_sections;
	memmap.unused_section_count = 2;
	memmap.used_section_array = used_sections;
	memmap.used_section_count = 4;

	/* Find an address with 254 bytes, which should be in the second buffer */

	addr = memmap_find(&memmap, 254);
	assert(addr == &buf2[0x02]);

	/* Find an address with 0x20 bytes, which should be in the first buffer. */

	addr = memmap_find(&memmap, 0x20);
	/* Note: the fact that it is at offset 0x20 is a coincidence. */
	assert(addr == &buf1[0x20]);

	/* Find an address with 0x08 bytes, which should be in the first buffer. */

	addr = memmap_find(&memmap, 0x08);
	/* Note: the same as above. */
	assert(addr == &buf1[0x08]);

	/* Test to make sure that this function fails properly. */

	addr = memmap_find(&memmap, 0xffffffff);
	assert(addr == NULL);
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

	unsigned char *buf;
	unsigned char *buf_end;
	unsigned long int buf_size;
	enum memmap_error error;
	struct memmap memmap;
	unsigned char *addr;
	unsigned char *addr2;

	/* Used this much memory for the
	 * memory map. */

	buf_size = 1024;

	/* Allocate the memory for the
	 * memory map. */

	buf = malloc(buf_size);
	assert(buf != NULL);

	/* The last byte in the memory
	 * used by the memory map. */

	buf_end = &buf[buf_size - 1];

	/* Initialize the memory map. */

	memmap_init(&memmap);

	/* Add the memory block to be used
	 * by the memory map. */

	error = memmap_add(&memmap, buf, buf_size);
	assert(error == MEMMAP_ERROR_NONE);

	/* Allocate memory */

	addr = memmap_alloc(&memmap, 64);
	/* Make sure the address isn't NULL. */
	assert(addr != NULL);
	/* Make sure the address is in
	 * the expected range. */
	assert(addr >= buf);
	assert(addr <= buf_end);

	/* Used sections:
	 *   Unused section array
	 *   Used section array
	 *   New address */
	assert(memmap.used_section_count == 3);
	assert(memmap.used_section_array[0].addr == memmap.unused_section_array);
	assert(memmap.used_section_array[1].addr == memmap.used_section_array);
	assert(memmap.used_section_array[2].addr == addr);

	/* Allocate some more memory. */

	addr2 = memmap_alloc(&memmap, 64);
	assert(addr2 != NULL);
	assert(addr2 >= buf);
	assert(addr2 <= buf_end);
	assert(memmap.used_section_count == 4);
	assert(memmap.used_section_array[0].addr == memmap.unused_section_array);
	assert(memmap.used_section_array[1].addr == addr);
	assert(memmap.used_section_array[2].addr == memmap.used_section_array);
	assert(memmap.used_section_array[3].addr == addr2);
	/* Make sure that the two memory blocks
	 * don't overlap. */
	assert(((addr2 + 64) <= addr) || (addr2 >= (addr + 64)));

	free(buf);
}
