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

#ifndef SWANSON_GPT_H
#define SWANSON_GPT_H

#include "guid.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct stream;

/** The size of the GPT header.
 * */

#define GPT_HEADER_SIZE 92

/** The maximum amount of characters
 * allowed in GPT partition name. This
 * includes the null-terminating character.
 * */

#define GPT_PARTITION_NAME_MAX 36

/** The size of a partition header.
 * Note that a partition entry occupies
 * one LBA. For indexing a partition entry,
 * use the LBA size. For checking read and
 * write functions, use this value.
 * */

#define GPT_PARTITION_ENTRY_SIZE 128

/** The maximum amount of partitions
 * that may be created in a GPT formatted
 * disk.
 * */

#define GPT_MAX_PARTITION_COUNT 128

/** The size of the partition header array.
 * */

#define GPT_PARTITION_ARRAY_SIZE (512 * GPT_MAX_PARTITION_COUNT)

/** The offset, in bytes, that the GPT header
 * is located.
 * */

#define GPT_START_OFFSET 512

/** The minimum size required for a disk
 * to be formatted with a GPT. This includes
 * the size for the master boot record, GPT
 * header, 128 partition headers (each 128 bytes
 * long), a backup GPT header, a backup GPT
 * partition header array and 128 LBAs for the
 * partitions (one per partition).
 * */

#define GPT_MINIMUM_DISK_SIZE (GPT_START_OFFSET + (GPT_MAX_PARTITION_COUNT * 512) + (2 * (GPT_PARTITION_ARRAY_SIZE + GPT_HEADER_SIZE)))

/** The GUID for a Swanson partition.
 * @see gpt_guid_swanson
 * */

#define GPT_GUID_SWANSON "86D277DA-CA76-49DC-9F6E-511CE31AF4D1"

/** The GUID for the partition that contains the
 * root file system.
 * */

#define GPT_GUID_ROOT "BD25C4F2-5E71-48F1-AE96-EF08F6AE028B"

/** The GUID for a Swanson partition
 * type. This may be more practical
 * to use for guid functions than the
 * macro, @ref GPT_GUID_SWANSON.
 * */

extern const struct guid gpt_guid_swanson;

/** The GUID for a partition containing
 * the root file system. Used by the kernel
 * to find the file system.
 * */

extern const struct guid gpt_guid_root;

/** Enumerates a list of errors
 * that may occur in a GPT function.
 * */

enum gpt_error {
	/** No error occured. */
	GPT_ERROR_NONE,
	/** An unknown error occured. */
	GPT_ERROR_UNKNOWN,
	/** The stream used for the GPT
	 * function is invalid. */
	GPT_ERROR_BAD_STREAM,
	/** The header contains an invalid
	 * GPT signature. This may be because
	 * the stream is not GPT formatted.*/
	GPT_ERROR_BAD_SIGNATURE,
	/** The header contains an
	 * invalid header checksum. */
	GPT_ERROR_BAD_HEADER,
	/** The header contains an
	 * invalid checksum for the
	 * partition header array. */
	GPT_ERROR_BAD_PARTITION_HEADERS,
	/** There is not enough space
	 * available for the GPT function. */
	GPT_ERROR_NEED_SPACE,
	/** Functionality was not implemented. */
	GPT_ERROR_NOT_IMPLEMENTED
};

/** GPT partition attributes. Used
 * to describe the partition to firmware
 * and software. */

enum gpt_attr {
	/** Indicates a partition is required
	 * for the computer to function properly. */
	GPT_ATTR_PLATFORM = 0,
	/** Indicates that the partition should be
	 * ignored by EFI firmware. */
	GPT_ATTR_IGNORE = 1,
	/** Indicates that the partition is bootable
	 * by legacy BIOS. */
	GPT_ATTR_BOOTABLE = 2

};

/** Returns a string representation
 * of an error code.
 * @param error The error code to get
 * the description of.
 * @returns A null-terminated string
 * that describes the error code.
 * */

const char *
gpt_strerror(enum gpt_error error);

/** Represents a GPT header.
 * This is usually used to find the partition
 * header table.
 * */

struct gpt_header {
	/** The header signature. Should
	 * be 'EFI PART'.  */
	uint8_t signature[8];
	/** The version of the GPT layout. */
	uint32_t version;
	/** The size of this header.
	 * Should be 92 bytes. */
	uint32_t header_size;
	/** The checksum of the header.
	 * This field is zero when the
	 * checksum is being calculated. */
	uint32_t header_crc32;
	/** Reserved and should be zero. */
	uint32_t reserved;
	/** The current LBA. This value
	 * should be equal to one, since the
	 * GPT header comes right after the
	 * master boot record. */
	uint64_t current_lba;
	/** The LBA containing the backup of
	 * this header. */
	uint64_t backup_lba;
	/** First LBA that a partition may use. */
	uint64_t first_usable_lba;
	/** Last LBA that a partition may use. */
	uint64_t last_usable_lba;
	/** Disk GUID */
	struct guid disk_guid;
	/** The starting LBA of the partition header entries. */
	uint64_t partition_array_lba;
	/** The number of partitions that exist. */
	uint32_t partition_count;
	/** The size of a single partition header. */
	uint32_t partition_entry_size;
	/** The checksum of the partition header array. */
	uint32_t partition_array_crc32;
};

/** Initializes a GPT header with
 * sensible values.
 * @param header The GPT header structure
 * to initialize.
 * */

void
gpt_header_init(struct gpt_header *header);

/** Reads the GPT header.
 * This function also verifies header signature
 * and the the checksum for the header and partition
 * header array.
 * @param stream A stream containing GPT data.
 * @param header A pointer to the header structure
 * to fill with data found from the stream.
 * @returns See @ref gpt_error.
 * */

enum gpt_error
gpt_header_read(struct stream *stream,
                struct gpt_header *header);

/** Writes the GPT Header.
 * This function calculates the checksum for the
 * header and partition header array before writing
 * to the stream. Be sure that the partition header
 * array is written to the stream before calling this
 * function, otherwise an invalid partition header array
 * checksum will be written to the stream.
 * @param stream A stream to write the GPT header to.
 * @param header The header that will be written to
 * the stream.
 * @returns See @ref gpt_error.
 * */

enum gpt_error
gpt_header_write(struct stream *stream,
                 const struct gpt_header *header);

/** A GPT partition header.
 * This is used to describe where the partition
 * contents are and what it contains.
 * */

struct gpt_partition {
	/** The partition type GUID. See @ref GPT_GUID_SWANSON */
	struct guid partition_type_guid;
	/** Partition GUID. This field is currently ignored.  */
	struct guid partition_guid;
	/** The first inclusive LBA of the partition contents. */
	uint64_t first_lba;
	/** The last inclusive LBA of the partition contents. */
	uint64_t last_lba;
	/** Partition attributes. */
	uint64_t attributes;
	/** The name of the partition, in UTF-16LE encoding. */
	uint16_t name[36];
};

/** Initializes a partition's internal members.
 * This should be called before the partition structure
 * is used.
 * @param partition An uninitialized partition structure.
 * */

void
gpt_partition_init(struct gpt_partition *partition);

/** Sets the name of the partition.
 * @param partition An initialized partition.
 * @param name The name to give the partition.
 * If this exceeds @ref GPT_PARTITION_NAME_MAX,
 * the name will be truncated to fit.
 * */

void
gpt_partition_set_name(struct gpt_partition *partition,
                       const char *name);

/** Sets the starting LBA of the partition.
 * After calling this function, @ref gpt_partition_set_size
 * should be called afterwards, to ensure the last LBA of the
 * partition is updated.
 * @param partition An initialized partition.
 * @param lba The starting LBA of the partition.
 * This value should be greater than or equal to
 * 130. If it is less than 130, it will be rounded
 * up to 130.
 * */

void
gpt_partition_set_lba(struct gpt_partition *partition,
                      uint64_t lba);

/** Set the size of the partition, rounded up to the
 * nearest 512 boundary. @ref gpt_partition_set_lba must
 * be called before using this function.
 * @param partition An initialized partition.
 * @param size The new size of the partition.
 * This is rounded up to a 512 boundary.
 * */

void
gpt_partition_set_size(struct gpt_partition *partition,
                       uint64_t size);

/** Reads a partition header
 * at the current location of
 * the stream.
 * @param stream The stream to read the
 * GPT partition header from.
 * @param partition The data structure to
 * fill with data found from the stream.
 * @returns See @ref gpt_error.
 * */

enum gpt_error
gpt_partition_read(struct stream *stream,
                   struct gpt_partition *partition);

/** Writes a partition header at the
 * current location of the stream.
 * @param stream The stream to write the
 * partition header to.
 * @param partition The partition header
 * to write to the stream.
 * @returns See @ref gpt_error.
 * */

enum gpt_error
gpt_partition_write(struct stream *stream,
                    const struct gpt_partition *partition);

/** An accessor structure for GPT.
 * Used for reading data structures
 * in the GPT format.
 * */

struct gpt_accessor {
	/** Data to pass to the callback functions. */
	void *data;
	/** Accesses the GPT header. */
	int (*header)(void *data, const struct gpt_header *header);
	/** Accesses the backup GPT header. */
	int (*backup_header)(void *data, const struct gpt_header *header);
	/** Accesses a GPT partition header. */
	int (*partition)(void *data, const struct gpt_partition *partition);
	/** Accesses a backup GPT partition header. */
	int (*backup_partition)(void *data, const struct gpt_partition *partition);
	/** Called if an error occurs while reading the GPT stream. */
	void (*error)(void *data, enum gpt_error error);
};

/** Initializes a GPT accessor structure.
 * This function is good to use because it ensures
 * that all function pointers that aren't used are
 * set to zero. This, in turn, prevents the access
 * function from calling a function that doesn't exist.
 * @param accessor An uninitialized GPT accessor.
 * */

void
gpt_accessor_init(struct gpt_accessor *accessor);

/** Reads a GPT stream and passes the
 * data structures to the accessor.
 * @param stream A stream containing the GPT data.
 * @param accessor An accessor structure with the
 * callbacks to trigger while reading the GPT data.
 * @returns If a non-zero error code was
 * returned by a callback function, then
 * that is returned by this function. If
 * an error occurs while reading the stream,
 * then a negative one is returned.
 * */

int
gpt_access(struct stream *stream,
           struct gpt_accessor *accessor);

/** A mutator structure for GPT.
 * Used for modifying data structures
 * in a GPT stream.
 * */

struct gpt_mutator {
	/** Data to pass to the callback functions. */
	const void *data;
	/** Mutate the GPT header. */
	int (*header)(const void *data, struct gpt_header *header);
	/** Mutate a GPT partition header. */
	int (*partition)(const void *data, struct gpt_partition *partition);
	/** Called if an error occurs while reading the GPT stream. */
	void (*error)(void *data, enum gpt_error error);
};

/** Reads a GPT stream and passes the
 * data structures to the mutator.
 * @param stream A stream containing the GPT data.
 * @param mutator A mutator structure containing the
 * callbacks to trigger while reading the GPT data.
 * @returns If a non-zero error code was
 * returned by a callback function, then
 * that is returned by this function. If
 * an error occurs while reading the stream,
 * then a negative one is returned.
 * */

int
gpt_mutate(struct stream *stream,
           const struct gpt_mutator *mutator);

/** Formats a stream with an empty GUID
 * partition table. This function will overwrite
 * existing GPT data.
 * @param stream A stream to write the GPT
 * data to.
 * @returns See @ref gpt_error.
 * */

enum gpt_error
gpt_format(struct stream *stream);

/** Searches for an LBA that can
 * fit a specified amount of storage.
 * @param stream An initialized stream, formatted
 * with GPT.
 * @param size The number of bytes that the LBA
 * must fit.
 * @param lba The LBA, if found, that can fit
 * the specified amount of storage.
 * @returns See @ref gpt_error.
 * */

enum gpt_error
gpt_find_space(struct stream *stream,
               uint64_t size,
               uint64_t *lba);

/** Creates a new partition.
 * The partition is created as a
 * Swanson partition.
 * @param stream The stream to add the
 * partition to. The stream must be GPT
 * formatted first.
 * @param partition_index The index of
 * the new partition. This index can be
 * used in subsequent calls.
 * @param partition_size The size of the
 * new partition. This must be greater than
 * zero. If zero is passed to this function,
 * then it is rounded up to the nearest LBA.
 * @returns See @ref gpt_error.
 * */

enum gpt_error
gpt_add_partition(struct stream *stream,
                  uint32_t *partition_index,
                  uint64_t partition_size);

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

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_GPT_H */
