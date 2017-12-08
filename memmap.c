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

#include "memmap.h"

#include "null.h"

/** Checks to see if an unused section can fit a memory
 * block of a specified size. */
static void *
find_addr_in_section(const struct memmap *memmap,
                     const struct memmap_section *section,
                     unsigned long int size) {

	unsigned char *addr;
	unsigned long int i;
	struct memmap_section *used_section;
	unsigned char *unused_addr;
	unsigned char *unused_addr_end;
	unsigned char *used_addr;

	if (section->size <= 0)
		return NULL;
	else if (size > section->size)
		return NULL;

	unused_addr = (unsigned char *) section->addr;

	unused_addr_end = &unused_addr[section->size - 1];

	addr = (unsigned char *) section->addr;

	for (i = 0; i < memmap->used_section_count; i++) {
		used_section = &memmap->used_section_array[i];
		/* Check to see if this used section is contained
		 * by the relevant unused section. */
		used_addr = (unsigned char *) used_section->addr;
		if ((used_addr < unused_addr)
		 || (used_addr > unused_addr_end)) {
			/* The used section is not contained by
			 * the revelant unused section. */
			continue;
		}

		/* A quick sanity check, to protect from
		 * integer overflow. */
		if (used_section->size > section->size)
			/* Section is messed up, and could possible
			 * corrupt the memory table. */
			continue;

		/* Check to see if the used section overlaps
		 * the current range of the candidate address. */
		if ((addr + size) > used_addr) {
			/* Section overlaps. Move the candidate
			 * address passed the end of the used
			 * section. */
			addr = &used_addr[used_section->size];
			/* But, if the size in the section that
			 * is leftover is too small, fail. */
			if ((section->size - used_section->size) < size)
				return NULL;
		} else {
			/* Section does not overlap, found a
			 * suitable address. */
			return addr;
		}
	}

	/* No memory sections found in this section,
	 * or the ones that were found have been passed. */

	return addr;
}

/** Creates the used section array by allocating memory in the first
 * suitable unused memory section. */
static int
bootstap_used_section_array(struct memmap *memmap) {

	struct memmap_section *unused_section;
	unsigned long int i;

	unused_section = NULL;

	/* Find an unused section that can support the size
	 * required by the used section array. */
	for (i = 0; i < memmap->unused_section_count; i++) {
		/* Can the unused section fit the used section array? */
		if (memmap->unused_section_array[i].size >= sizeof(struct memmap_section)) {
			unused_section = &memmap->unused_section_array[i];
			break;
		}
	}

	/* Check that an unused section was found.  */
	if (unused_section == NULL)
		return -1;

	memmap->used_section_array = unused_section->addr;
	memmap->used_section_array[0].addr = unused_section->addr;
	memmap->used_section_array[0].size = sizeof(struct memmap_section);

	return 0;
}

/** Allocates a used section for the used section array. */
static struct memmap_section *
alloc_used_section(struct memmap *memmap) {

	struct memmap_section *section;
	struct memmap_section *used_section_array;
	unsigned long int used_section_array_size;

	section = NULL;

	if (memmap->used_section_array == NULL) {
		if (bootstap_used_section_array(memmap) != 0)
			return NULL;
	}

	used_section_array_size = (memmap->used_section_count + 1) * sizeof(struct memmap_section);
	used_section_array = memmap->used_section_array;
	used_section_array = memmap_realloc(memmap, used_section_array, used_section_array_size);
	if (used_section_array == NULL)
		return NULL;

	return section;
}

void
memmap_section_init(struct memmap_section *section) {
	section->addr = NULL;
	section->size = 0;
}

void
memmap_section_set(struct memmap_section *section,
                   void *addr,
                   unsigned long int size) {
	section->addr = addr;
	section->size = size;
}

void
memmap_init(struct memmap *memmap) {
	memmap->unused_section_array = NULL;
	memmap->unused_section_count = 0;
	memmap->used_section_array = NULL;
	memmap->used_section_count = 0;
}

int
memmap_add(struct memmap *memmap,
           void *addr,
           unsigned long int size) {

	if (memmap->unused_section_array == NULL) {

		/* Bootstrap section arrays
		 * with new memory section. */

		/* Make sure the new memory section
		 * can fit an entry in the unused
		 * section array and two in the
		 * used section array. */

		if (size < (sizeof(struct memmap_section) * 3)) {
			return MEMMAP_ERROR_NEED_SPACE;
		}

		/* Setup the unused section array.
		 * It will contain one entry, and that
		 * is for the new buffer that was just
		 * added.
		 * */

		memmap->unused_section_array = (struct memmap_section *) addr;

		memmap->unused_section_array[0].addr = addr;
		memmap->unused_section_array[0].size = size;

		memmap->unused_section_count = 1;

		/* Setup the used section array.
		 * The number of entries is two because
		 * one is for the section array and one
		 * is for the used section array.
		 * */

		memmap->used_section_array = ((struct memmap_section *) addr) + 1;

		memmap->used_section_array[0].addr = memmap->unused_section_array;
		memmap->used_section_array[0].size = sizeof(struct memmap_section);

		memmap->used_section_array[1].addr = memmap->used_section_array;
		memmap->used_section_array[1].size = sizeof(struct memmap_section) * 2;

		memmap->used_section_count = 2;
	}

	return MEMMAP_ERROR_NONE;
}

void *
memmap_find(const struct memmap *memmap,
            unsigned long int size) {

	void *addr;
	unsigned long int i;
	struct memmap_section *unused_section;

	for (i = 0; i < memmap->unused_section_count; i++) {

		unused_section = &memmap->unused_section_array[i];

		addr = find_addr_in_section(memmap, unused_section, size);
		if (addr != NULL)
			return addr;
	}

	return NULL;
}

void *
memmap_alloc(struct memmap *memmap,
             unsigned long int size) {

	struct memmap_section *section;
	void *addr;

	addr = memmap_find(memmap, size);
	if (addr == NULL)
		return NULL;

	section = alloc_used_section(memmap);
	if (section == NULL)
		return NULL;

	section->addr = addr;
	section->size = size;

	return addr;
}

void *
memmap_realloc(struct memmap *memmap,
               void *old_addr,
               unsigned long int size) {

	void *addr;

	addr = memmap_alloc(memmap, size);
	if (addr == NULL)
		return NULL;

	/* TODO : copy old memory to new memory */

	memmap_free(memmap, old_addr);

	return addr;
}

void
memmap_free(struct memmap *memmap,
            void *addr) {
	(void) memmap;
	(void) addr;
}
