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

/** @file */

#ifndef SWANSON_MEMMAP_H
#define SWANSON_MEMMAP_H

#ifdef __cplusplus
extern "C" {
#endif

/** An enumeration of the
 * possible errors that may
 * occur in a memory map
 * function.
 * */

enum memmap_error {
	/** No error occured. */
	MEMMAP_ERROR_NONE,
	/** An unknown error occurred. */
	MEMMAP_ERROR_UNKNOWN,
	/** More space is required for
	 * the memory map. */
	MEMMAP_ERROR_NEED_SPACE
};

/** A section of memory in the memory map.
 * */

struct memmap_section {
	/** The starting address of the memory map section. */
	void *addr;
	/** The number of bytes in the memory map section. */
	unsigned long int size;
};

/** Initializes a memory section.
 * @param section An uninitialized section.
 * */

void
memmap_section_init(struct memmap_section *section);

/** Sets the block of memory associated
 * with the section.
 * @param section An initialized memory section.
 * @param addr The new address for the section.
 * @param size The size, in bytes, of the block
 * of memory.
 * */

void
memmap_section_set(struct memmap_section *section,
                   void *addr,
                   unsigned long int size);

/** The minimum size of a memory map.
 * Defined as a memory map that has enough
 * to allocate zero bytes.
 * */

#define MEMMAP_MINIMUM_SIZE (sizeof(struct memmap_section) * 3)

/** A memory map of the system.
 * */

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

/** Initializes the memory map.
 * This function should be called first.
 * @param memmap An uninitialized memory map.
 * */

void
memmap_init(struct memmap *memmap);

/** Adds a section of memory for the
 * memory map to use. The very first
 * section of memory should be able to
 * fit at least three @ref memmap_section
 * structures. Larger and fewer sections
 * of memory are better than smaller and
 * more sections of memory.
 * @param memmap An initialized memory map.
 * @param addr The address of the memory to add.
 * @param size The size of the memory block.
 * @returns Zero on success, non-zero on failure.
 * */

int
memmap_add(struct memmap *memmap,
           void *addr,
           unsigned long int size);

/** Find an address able to fit a
 * certain amount of memory. This
 * function is used internally and
 * doesn't serve much purpose outside
 * of the library. This function does
 * not reserve memory for the address
 * that is returned.
 * @param memmap An initialized memory map.
 * @param size The size, in bytes, that the
 * address should be able to accomodate.
 * @returns The address, on success. @ref NULL
 * on failure.
 * */

void *
memmap_find(const struct memmap *memmap,
            unsigned long int size);

/** Allocates memory from the memory map.
 * @param memmap An initialized memory map.
 * @param size The number of bytes to allocate.
 * @returns On success, the address of the memory
 * block. On failure, zero (or NULL).
 * */

void *
memmap_alloc(struct memmap *memmap,
             unsigned long int size);

/** Resizes a block of memory that already exists.
 * @param memmap An initialized memory map.
 * @param addr The address of the memory block to resize.
 * @param size The new size of the memory block.
 * @returns On success, the new address of the memory
 * block. On failure, zero (or NULL).
 * */

void *
memmap_realloc(struct memmap *memmap,
               void *addr,
               unsigned long int size);

/** Releases a block of memory in the memory map.
 * @param memmap An initialized memory map.
 * @param addr The address of the memory block to free.
 * */

void
memmap_free(struct memmap *memmap,
            void *addr);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_MEMMAP_H */
