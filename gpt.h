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

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct stream;

/** The GUID for a Swanson partition.
 * */

#define GPT_GUID_SWANSON "86D277DA-CA76-49DC-9F6E-511CE31AF4D1"

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
	GPT_ERROR_BAD_PARTITION_HEADERS
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

const char *gpt_strerror(enum gpt_error error);

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
	uint8_t disk_guid[16];
	/** The starting LBA of the partition header entries. */
	uint64_t partitions_array_lba;
	/** The number of partitions that exist. */
	uint32_t partition_count;
	/** The size of a single partition header. */
	uint32_t partition_entry_size;
	/** The checksum of the partition header array. */
	uint32_t partition_array_crc32;
};

/** Reads the GPT header.
 * This function also verifies header signature
 * and the the checksum for the header and partition
 * header array.
 * @param stream A stream containing GPT data.
 * @param header A pointer to the header structure
 * to fill with data found from the stream.
 * @returns See @ref gpt_error.
 * */

enum gpt_error gpt_header_read(struct stream *stream,
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

enum gpt_error gpt_header_write(struct stream *stream,
                                const struct gpt_header *header);

/** A GPT partition header.
 * This is used to describe where the partition
 * contents are and what it contains.
 * */

struct gpt_partition {
	/** The partition type GUID. See @ref GPT_GUID_SWANSON */
	uint8_t partition_type_guid[16];
	/** Partition GUID. This field is currently ignored.  */
	uint8_t partition_guid[16];
	/** The first inclusive LBA of the partition contents. */
	uint64_t first_lba;
	/** The last inclusive LBA of the partition contents. */
	uint64_t last_lba;
	/** Partition attributes. */
	uint64_t attributes;
	/** The name of the partition, in UTF-16LE encoding. */
	uint16_t name[36];
};

/** Reads a partition header
 * at the current location of
 * the stream.
 * @param stream The stream to read the
 * GPT partition header from.
 * @param partition The data structure to
 * fill with data found from the stream.
 * @returns See @ref gpt_error.
 * */

enum gpt_error gpt_partition_read(struct stream *stream,
                                  const struct gpt_partition *partition);

/** Writes a partition header at the
 * current location of the stream.
 * @param stream The stream to write the
 * partition header to.
 * @param partition The partition header
 * to write to the stream.
 * @returns See @ref gpt_error.
 * */

enum gpt_error gpt_partition_write(struct stream *stream,
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
	/** Accesses a GPT partition header. */
	int (*partition)(void *data, const struct gpt_partition *partition);
	/** Called if an error occurs while reading the GPT stream. */
	void (*error)(void *data, enum gpt_error error);
};

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

int gpt_access(struct stream *stream, struct gpt_accessor *accessor);

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

int gpt_mutate(struct stream *stream, const struct gpt_mutator *mutator);

/** Formats a stream with an empty GUID
 * partition table. This function will overwrite
 * existing GPT data.
 * @param stream A stream to write the GPT
 * data to.
 * @returns See @ref gpt_error.
 * */

enum gpt_error gpt_format(struct stream *stream);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_GPT_H */
