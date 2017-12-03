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

#include "gpt.h"

#include "crc32.h"
#include "sstream.h"

#include "debug.h"

#ifndef NULL
#define NULL ((void *) 0x00)
#endif

const char *gpt_strerror(enum gpt_error error) {
	if (error == GPT_ERROR_BAD_STREAM)
		return "Invalid Stream";
	else if (error == GPT_ERROR_BAD_SIGNATURE)
		return "Invalid Signature";
	else if (error == GPT_ERROR_BAD_HEADER)
		return "Invalid Header Checksum";
	else if (error == GPT_ERROR_BAD_PARTITION_HEADERS)
		return "Invalid Partition Headers Checksum";
	else if (error == GPT_ERROR_NEED_SPACE)
		return "Need More Space";
	else if (error == GPT_ERROR_NONE)
		return "No Error";
	else
		return "Unknown Error";
}

void gpt_header_init(struct gpt_header *header) {
	header->signature[0] = 'E';
	header->signature[1] = 'F';
	header->signature[2] = 'I';
	header->signature[3] = ' ';
	header->signature[4] = 'P';
	header->signature[5] = 'A';
	header->signature[6] = 'R';
	header->signature[7] = 'T';
	header->version = 0x00010000;
	header->header_size = 92;
	header->header_crc32 = 0;
	header->reserved = 0;
	header->current_lba = 1;
	header->backup_lba = 1;
	header->first_usable_lba = ~0x00ULL;
	header->last_usable_lba = ~0x00ULL;
	guid_init(&header->disk_guid);
	header->partition_array_lba = 2ULL;
	header->partition_count = 0;
	header->partition_entry_size = 128;
	header->partition_array_crc32 = 0;
}

enum gpt_error gpt_header_read(struct stream *stream,
                               struct gpt_header *header) {

	uint64_t read_count;
	uint32_t header_crc32;
	char buf[512];
	struct sstream sector;

	sstream_init(&sector);

	sstream_setbuf(&sector, buf, sizeof(buf));

	read_count = stream_read(stream, buf, sizeof(buf));
	if (read_count != sizeof(buf))
		return GPT_ERROR_UNKNOWN;

	read_count = 0;

	read_count += stream_read(&sector.stream, header->signature, sizeof(header->signature));

	read_count += stream_decode_uint32le(&sector.stream, &header->version);
	read_count += stream_decode_uint32le(&sector.stream, &header->header_size);
	read_count += stream_decode_uint32le(&sector.stream, &header->header_crc32);
	read_count += stream_decode_uint32le(&sector.stream, &header->reserved);
	read_count += stream_decode_uint64le(&sector.stream, &header->current_lba);
	read_count += stream_decode_uint64le(&sector.stream, &header->backup_lba);
	read_count += stream_decode_uint64le(&sector.stream, &header->first_usable_lba);
	read_count += stream_decode_uint64le(&sector.stream, &header->last_usable_lba);

	read_count += stream_read(&sector.stream, header->disk_guid.buffer, GUID_SIZE);

	read_count += stream_decode_uint64le(&sector.stream, &header->partition_array_lba);
	read_count += stream_decode_uint32le(&sector.stream, &header->partition_count);
	read_count += stream_decode_uint32le(&sector.stream, &header->partition_entry_size);
	read_count += stream_decode_uint32le(&sector.stream, &header->partition_array_crc32);

	if (read_count != header->header_size)
		return GPT_ERROR_UNKNOWN;

	/* the checksum field should be zero
	 * when the checksum is being calculated */
	buf[16] = 0;
	buf[17] = 0;
	buf[18] = 0;
	buf[19] = 0;

	header_crc32 = crc32(buf, header->header_size);
	if (header_crc32 != header->header_crc32)
		return GPT_ERROR_BAD_HEADER;

	return GPT_ERROR_NONE;
}

enum gpt_error gpt_header_write(struct stream *stream,
                                const struct gpt_header *header) {

	int err;
	char buf[512];
	struct sstream sector;
	uint64_t write_count;
	uint32_t header_checksum;

	sstream_init(&sector);

	sstream_setbuf(&sector, buf, sizeof(buf));

	write_count = 0;

	write_count += stream_write(&sector.stream, header->signature, sizeof(header->signature));

	write_count += stream_encode_uint32le(&sector.stream, header->version);
	write_count += stream_encode_uint32le(&sector.stream, header->header_size);
	write_count += stream_encode_uint32le(&sector.stream, 0 /* header checksum */);
	write_count += stream_encode_uint32le(&sector.stream, header->reserved);
	write_count += stream_encode_uint64le(&sector.stream, header->current_lba);
	write_count += stream_encode_uint64le(&sector.stream, header->backup_lba);
	write_count += stream_encode_uint64le(&sector.stream, header->first_usable_lba);
	write_count += stream_encode_uint64le(&sector.stream, header->last_usable_lba);

	write_count += stream_write(&sector.stream, header->disk_guid.buffer, GUID_SIZE);

	write_count += stream_encode_uint64le(&sector.stream, header->partition_array_lba);
	write_count += stream_encode_uint32le(&sector.stream, header->partition_count);
	write_count += stream_encode_uint32le(&sector.stream, header->partition_entry_size);
	write_count += stream_encode_uint32le(&sector.stream, header->partition_array_crc32);

	if (write_count != header->header_size)
		return GPT_ERROR_UNKNOWN;

	/* calculate header checksum */

	header_checksum = crc32(buf, header->header_size);

	err = stream_setpos(&sector.stream, 16);
	if (err != 0)
		return GPT_ERROR_UNKNOWN;

	write_count = stream_encode_uint32le(&sector.stream, header_checksum);
	if (write_count != 4)
		return GPT_ERROR_UNKNOWN;

	/* TODO : calculate partition header array checksum */

	write_count = stream_write(stream, buf, sizeof(buf));
	if (write_count != sizeof(buf))
		return GPT_ERROR_UNKNOWN;

	return GPT_ERROR_NONE;
}

void gpt_partition_init(struct gpt_partition *partition) {
	guid_init(&partition->partition_type_guid);
	guid_init(&partition->partition_guid);
	partition->first_lba = 0;
	partition->last_lba = 0;
	partition->attributes = 0;
	gpt_partition_set_name(partition, "");
}

enum gpt_error gpt_partition_read(struct stream *stream,
                                  struct gpt_partition *partition) {

	uint64_t read_count;

	read_count = 0;
	read_count += guid_read(stream, &partition->partition_type_guid);
	read_count += guid_read(stream, &partition->partition_guid);
	read_count += stream_decode_uint64le(stream, &partition->first_lba);
	read_count += stream_decode_uint64le(stream, &partition->last_lba);
	read_count += stream_decode_uint64le(stream, &partition->attributes);
	read_count += stream_read(stream, partition->name, sizeof(partition->name));

	if (read_count != GPT_PARTITION_ENTRY_SIZE)
		return GPT_ERROR_UNKNOWN;

	return GPT_ERROR_NONE;
}

void gpt_partition_set_name(struct gpt_partition *partition,
                            const char *name) {
	unsigned int i;
	unsigned int i_max;
	i_max = sizeof(partition->name);
	i_max /= sizeof(partition->name[0]);
	for (i = 0; (i < (i_max - 1)) && name[i]; i++)
		partition->name[i] = name[i];
	/* zero the rest */
	while (i < i_max)
		partition->name[i++] = 0;
}

void gpt_partition_set_lba(struct gpt_partition *partition,
                           uint64_t lba) {
	partition->first_lba = lba;
}

void gpt_partition_set_size(struct gpt_partition *partition,
                            uint64_t size) {
	partition->last_lba = partition->first_lba + (size + 512) / 512;
}

enum gpt_error gpt_partition_write(struct stream *stream,
                                   const struct gpt_partition *partition) {

	uint64_t write_count;

	write_count = 0;
	write_count += guid_write(stream, &partition->partition_type_guid);
	write_count += guid_write(stream, &partition->partition_guid);
	write_count += stream_encode_uint64le(stream, partition->first_lba);
	write_count += stream_encode_uint64le(stream, partition->last_lba);
	write_count += stream_encode_uint64le(stream, partition->attributes);
	write_count += stream_write(stream, partition->name, sizeof(partition->name));

	if (write_count != GPT_PARTITION_ENTRY_SIZE)
		return GPT_ERROR_UNKNOWN;

	return GPT_ERROR_NONE;
}

enum gpt_error gpt_format(struct stream *stream) {

	int err;
	uint64_t stream_size;
	uint64_t last_lba;
	uint64_t first_usable_lba;
	uint64_t last_usable_lba;
	struct gpt_header header;
	struct gpt_header backup_header;

	/* Set the stream size to zero,
	 * just in case the stream implementation
	 * returns successfully, but does not set
	 * the size. */
	stream_size = 0;

	err = stream_getsize(stream, &stream_size);
	if (err != 0)
		return GPT_ERROR_UNKNOWN;
	else if (stream_size < GPT_MINIMUM_DISK_SIZE)
		return GPT_ERROR_NEED_SPACE;

	last_lba = stream_size / 512;
	/* mbr + gpt header + gpt partition headers */
	first_usable_lba = 130;
	/* last usable lba - gpt partition headers - gpt backup header */
	last_usable_lba = last_lba - 129;

	gpt_header_init(&header);
	header.first_usable_lba = first_usable_lba;
	header.last_usable_lba = last_usable_lba;
	header.backup_lba = last_lba;

	err = stream_setpos(stream, 512);
	if (err != 0)
		return GPT_ERROR_UNKNOWN;

	err = gpt_header_write(stream, &header);
	if (err != GPT_ERROR_NONE)
		return err;

	gpt_header_init(&backup_header);

	backup_header.backup_lba = 1;
	backup_header.current_lba = last_lba;
	backup_header.partition_array_lba = last_lba - 128;
	backup_header.first_usable_lba = first_usable_lba;
	backup_header.last_usable_lba = last_usable_lba;

	err = stream_setpos(stream, (last_lba - 1) * 512);
	if (err != 0)
		return GPT_ERROR_UNKNOWN;

	err = gpt_header_write(stream, &header);
	if (err != GPT_ERROR_NONE)
		return err;

	return GPT_ERROR_NONE;
}

void gpt_accessor_init(struct gpt_accessor *accessor) {
	accessor->data = NULL;
	accessor->header = NULL;
	accessor->backup_header = NULL;
	accessor->partition = NULL;
	accessor->backup_partition = NULL;
	accessor->error = NULL;
}

int gpt_access(struct stream *stream,
               struct gpt_accessor *accessor) {

	int err;
	struct gpt_header header;
	struct gpt_partition partition;
	uint32_t i;
	uint64_t partition_offset;

	err = stream_setpos(stream, 512);
	if (err != 0)
		return -1;

	err = gpt_header_read(stream, &header);
	if (err != GPT_ERROR_NONE) {
		if (accessor->error != NULL)
			accessor->error(accessor->data, err);
		return -1;
	}

	if (accessor->header != NULL) {
		err = accessor->header(accessor->data, &header);
		if (err != 0)
			return err;
	}

	if (header.partition_count == 0)
		return 0;

	partition_offset = header.partition_array_lba * 512;

	for (i = 0; i < header.partition_count; i++) {

		err = stream_setpos(stream, partition_offset);
		if (err != 0)
			return err;

		gpt_partition_init(&partition);

		err = gpt_partition_read(stream, &partition);
		if (err != GPT_ERROR_NONE) {
			if (accessor->error != NULL)
				accessor->error(accessor->data, err);
			return -1;
		}

		if (accessor->partition != NULL) {
			err = accessor->partition(accessor->data, &partition);
			if (err != 0)
				return err;
		}

		partition_offset += 512;
	}

	return 0;
}

enum gpt_error gpt_find_space(struct stream *stream,
                              uint64_t size,
                              uint64_t *lba) {
	(void) stream;
	(void) size;
	(void) lba;
	return GPT_ERROR_NONE;
}

enum gpt_error gpt_add_partition(struct stream *stream,
                                 uint32_t *partition_index_ptr) {

	int err;
	struct gpt_header header;
	struct gpt_partition partition;
	uint32_t partition_index;
	uint64_t partition_offset;
	uint64_t partition_lba;

	err = gpt_find_space(stream, 512, &partition_lba);
	if (err != GPT_ERROR_NONE)
		return err;

	err = stream_setpos(stream, 512);
	if (err != 0)
		return GPT_ERROR_UNKNOWN;

	err = gpt_header_read(stream, &header);
	if (err != GPT_ERROR_NONE)
		return err;

	if (header.partition_count >= GPT_MAX_PARTITION_COUNT)
		return GPT_ERROR_NEED_SPACE;

	header.partition_count++;

	partition_index = header.partition_count - 1;

	partition_offset = (header.partition_array_lba + partition_index) * 512;

	err = stream_setpos(stream, partition_offset);
	if (err != 0)
		return GPT_ERROR_UNKNOWN;

	gpt_partition_init(&partition);

	err = gpt_partition_write(stream, &partition);
	if (err != GPT_ERROR_NONE)
		return err;

	/* TODO */

	err = stream_setpos(stream, 512);
	if (err != 0)
		return GPT_ERROR_UNKNOWN;

	err = gpt_header_write(stream, &header);
	if (err != GPT_ERROR_NONE)
		return err;

	if (partition_index_ptr != NULL)
		*partition_index_ptr = partition_index;

	(void) partition;

	return GPT_ERROR_NONE;
}

