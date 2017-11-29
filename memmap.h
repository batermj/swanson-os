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

#ifndef SWANSON_MEMMAP_H
#define SWANSON_MEMMAP_H

#ifdef __cplusplus
extern "C" {
#endif

/** A section of memory in the memory map. */
struct memmap_section {
	/** The starting address of the memory map section. */
	void *addr;
	/** The number of bytes in the memory map section. */
	unsigned long int size;
};

/** A memory map of the system. */
struct memmap {
	/** An array of the unused sections in the memory map. */
	struct memmap_section *unused_section_array;
	/** The number of unused sections in the memory map. */
	unsigned long int unused_section_count;
	/** An array of used sections in the memory map. */
	struct memmap_section *used_section_array;
	/** The number of used sections in the memory map. */
	unsigned long int used_section_count;
};

/** Initializes the memory map. */
void memmap_init(struct memmap *memmap);

/** Allocates memory from the memory map. */
void *memmap_alloc(struct memmap *memmap, unsigned long int size);

/** Resizes a block of memory that already exists. */
void *memmap_realloc(struct memmap *memmap, void *addr, unsigned long int size);

/** Releases a block of memory in the memory map. */
void memmap_free(struct memmap *memmap, void *addr);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_MEMMAP_H */
