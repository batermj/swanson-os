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

#ifndef SWANSON_ELF_H
#define SWANSON_ELF_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

struct stream;

/** A segment within an ELF file.
 * */

struct elf_segment {
	/** Section data. */
	void *data;
	/** The number of bytes in the
	 * segment data. */
	uintmax_t data_size;
	/** The virtual address of the
	 * segment. */
	uint32_t virtual_address;
	/** Whether or not the segment
	 * has read permission. */
	bool read_permission;
	/** Whether or not the segmnet
	 * has write permission.
	 * */
	bool write_permission;
	/** Whether or not the segment
	 * has execute permission.
	 * */
	bool execute_permission;
};

/** Initializes an ELF segment.
 * @param segment An uninitialized ELF segment.
 * */

void
elf_segment_init(struct elf_segment *segment);

/** Releases resources allocated by an ELF segment.
 * @param segment An initialized ELF segment.
 * */

void
elf_segment_free(struct elf_segment *segment);

/** An ELF file.
 * */

struct elf_file {
	/** The segments within the ELF file. */
	struct elf_segment *segment_array;
	/** The number of segments within the ELF file. */
	uintmax_t segment_count;
};

/** Initializes an ELF file.
 * @param file An uninitialized ELF file.
 * */

void
elf_file_init(struct elf_file *file);

/** Releases resources allocated by the
 * ELF file.
 * @param file An initialized ELF file.
 * */

void
elf_file_free(struct elf_file *file);

/** Reads an ELF file. The previous contents
 * of the ELF file structure are discarded.
 * @param file An initialized ELF file.
 * @param stream A stream containing ELF file data.
 * @returns Zero on success, non-zero on failure.
 * */

int
elf_file_decode(struct elf_file *file,
                struct stream *stream);

/** Adds a segment to an ELF file structure.
 * @param file An initialized ELF file structure.
 * @param segment The segment to add to the ELF file structure.
 * A copy is made of this structure.
 * @returns Zero on success, non-zero on failure.
 * */

int
elf_file_push_segment(struct elf_file *file,
                      const struct elf_segment *segment);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_ELF_H */
