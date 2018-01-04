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

#ifndef SWANSON_GPT_SOURCE_H
#define SWANSON_GPT_SOURCE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct gpt_header;
struct gpt_partition;

/** Represents a medium from which
 * GPT data can be read from and
 * written to.
 * */

struct gpt_source {
	/** Implementation data. */
	void *data;
	/** Reads the GPT header from source. */
	int (*read_header)(void *data, struct gpt_header *header);
	/** Reads the backup GPT header from source. */
	int (*read_header_backup)(void *data, struct gpt_header *header);
	/** Writes the GPT header to source. */
	int (*write_header)(void *data, const struct gpt_header *header);
	/** Writes the GPT backup header to source. */
	int (*write_header_backup)(void *data, const struct gpt_header *header);
	/** Reads a partition entry from the GPT source. */
	int (*read_partition)(void *data, uint32_t index, struct gpt_partition *partition);
	/** Reads a partition backup entry from the GPT source. */
	int (*read_partition_backup)(void *data, uint32_t index, struct gpt_partition *partition);
	/** Writes a partition entry to the GPT source. */
	int (*write_partition)(void *data, uint32_t index, const struct gpt_partition *partition);
	/** Writes a partition backup entry to the GPT source. */
	int (*write_partition_backup)(void *data, uint32_t index, const struct gpt_partition *partition);
};

/** Initializes a GPT source structure.
 * @param source An uninitialized GPT source.
 * */

void
gpt_source_init(struct gpt_source *source);

/** Reads the primary GPT header from the source.
 * @param source An initialized GPT source structure.
 * @param header The header structure to initialize
 * from the GPT source.
 * @returns See @ref gpt_error.
 * */

enum gpt_error
gpt_source_read_header(struct gpt_source *source,
                       struct gpt_header *header);

/** Writes the primary GPT header to the source.
 * @param source An initialized GPT source structure.
 * @param header The header to write.
 * @returns See @ref gpt_error.
 * */

enum gpt_error
gpt_source_write_header(struct gpt_source *source,
                        const struct gpt_header *header);

/** Writes the backup GPT header to the source.
 * @param source An initialized GPT source structure.
 * @param header The header to write.
 * @returns See @ref gpt_error.
 * */

enum gpt_error
gpt_source_write_header_backup(struct gpt_source *source,
                               const struct gpt_header *header);

/** Updates the primary and backup
 * GPT headers.
 * @param source The source containing the
 * headers.
 * @param header The structure use for the
 * backup and primary headers.
 * @returns See @ref gpt_error.
 * */

enum gpt_error
gpt_source_update_header(struct gpt_source *source,
                         const struct gpt_header *header);

/** Read a partition from the GPT source.
 * @param source An initialized GPT source.
 * @param partition_index The index of the
 * partition.
 * @param partition The partition structure
 * to fill with information from the partition
 * table.
 * @returns See @ref gpt_error.
 * */

enum gpt_error
gpt_source_read_partition(struct gpt_source *source,
                          uint32_t partition_index,
                          struct gpt_partition *partition);

/** Write the primary partition header to
 * the partition header array.
 * @param source An initialized GPT source.
 * @param partition_index The index of the partition header.
 * @param partition The structure containing information
 * about the partition.
 * @returns See @ref gpt_error.
 * */

enum gpt_error
gpt_source_write_partition(struct gpt_source *source,
                           uint32_t partition_index,
                           const struct gpt_partition *partition);

/** Write the backup partition header to
 * the backup partition header array.
 * @param source An initialized GPT source.
 * @param partition_index The index of the partition header.
 * @param partition The structure containing information
 * about the partition.
 * @returns See @ref gpt_error.
 * */

enum gpt_error
gpt_source_write_partition_backup(struct gpt_source *source,
                                  uint32_t partition_index,
                                  const struct gpt_partition *partition);

/** Write the primary and backup partition
 * headers.
 * @param source An initialized GPT source.
 * @param partition_index The index of the partition
 * to update.
 * @param partition The partition information to
 * update with.
 * @returns See @ref gpt_error.
 * */

enum gpt_error
gpt_source_update_partition(struct gpt_source *source,
                            uint32_t partition_index,
                            const struct gpt_partition *partition);

/** Format a GPT source.
 * @param source An initialized GPT source structure.
 * @returns See @ref gpt_error.
 * */

enum gpt_error
gpt_source_format(struct gpt_source *source);

/** Adds a partition to the partition table.
 * @param source An initialized partition source.
 * @param partition_size The size, in bytes, to allocate
 * for the partition. This number is rounded up to the
 * next highest 512-byte boundary.
 * @param partition_id If this function is successful,
 * this parameter will contain the partition index, for
 * use in subsequent functions. This parameter may be
 * @ref NULL.
 * @returns See @ref gpt_error.
 * */

enum gpt_error
gpt_source_add_partition(struct gpt_source *source,
                         uint64_t partition_size,
                         uint32_t *partition_id);

/** Find an LBA in an unused section that
 * can fit a specified amount of data.
 * @param source An initialized GPT source.
 * @param size The size, in bytes, that the
 * LBA should be able to fit.
 * @param starting_lba If the function succeeds,
 * this will contain the LBA that will fit the
 * specified size.
 * @returns See @ref gpt_error.
 * */

enum gpt_error
gpt_source_allocate(struct gpt_source *source,
                    uint64_t size,
                    uint64_t *starting_lba);

/** Calculates the checksum for the header
 * and the partition entry array. This should
 * be called when the gpt source is no longer
 * going to be modified.
 * @param source An initialized GPT source.
 * @returns See @ref gpt_error.
 * */

enum gpt_error
gpt_source_do_checksums(struct gpt_source *source);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_GPT_SOURCE_H */
