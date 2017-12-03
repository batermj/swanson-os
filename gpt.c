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

const char *gpt_strerror(enum gpt_error error) {
	if (error == GPT_ERROR_BAD_STREAM)
		return "Invalid Stream";
	else if (error == GPT_ERROR_BAD_SIGNATURE)
		return "Invalid Signature";
	else if (error == GPT_ERROR_BAD_HEADER)
		return "Invalid Header Checksum";
	else if (error == GPT_ERROR_BAD_PARTITION_HEADERS)
		return "Invalid Partition Headers Checksum";
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
	header->partitions_array_lba = ~0x00ULL;
	header->partition_count = 0;
	header->partition_entry_size = 128;
	header->partition_array_crc32 = 0;
}

enum gpt_error gpt_header_read(struct stream *stream,
                               struct gpt_header *header) {

	int err;
	uint64_t read_count;
	char buf[512];
	struct sstream sector;

	sstream_init(&sector);

	sstream_setbuf(&sector, buf, sizeof(buf));

	err = stream_setpos(stream, 512);
	if (err != 0)
		return GPT_ERROR_UNKNOWN;

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

	read_count += stream_decode_uint64le(&sector.stream, &header->partitions_array_lba);
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

	if (crc32(buf, sizeof(buf)) != header->header_crc32)
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
	write_count += stream_encode_uint32le(&sector.stream, header->header_crc32);
	write_count += stream_encode_uint32le(&sector.stream, header->reserved);
	write_count += stream_encode_uint64le(&sector.stream, header->current_lba);
	write_count += stream_encode_uint64le(&sector.stream, header->backup_lba);
	write_count += stream_encode_uint64le(&sector.stream, header->first_usable_lba);
	write_count += stream_encode_uint64le(&sector.stream, header->last_usable_lba);

	write_count += stream_write(&sector.stream, header->disk_guid.buffer, GUID_SIZE);

	write_count += stream_encode_uint64le(&sector.stream, header->partitions_array_lba);
	write_count += stream_encode_uint32le(&sector.stream, header->partition_count);
	write_count += stream_encode_uint32le(&sector.stream, header->partition_entry_size);
	write_count += stream_encode_uint32le(&sector.stream, header->partition_array_crc32);

	if (write_count != header->header_size)
		return GPT_ERROR_UNKNOWN;

	/* calculate header checksum */

	header_checksum = crc32(buf, sizeof(buf));

	err = stream_setpos(&sector.stream, 16);
	if (err != 0)
		return GPT_ERROR_UNKNOWN;

	write_count = stream_encode_uint32le(&sector.stream, header_checksum);
	if (write_count != 4)
		return GPT_ERROR_UNKNOWN;

	/* TODO : calculate partition header array checksum */

	err = stream_setpos(stream, 512);
	if (err != 0)
		return GPT_ERROR_UNKNOWN;

	write_count = stream_write(stream, buf, sizeof(buf));
	if (write_count != sizeof(buf))
		return GPT_ERROR_UNKNOWN;

	return GPT_ERROR_NONE;
}

enum gpt_error gpt_format(struct stream *stream) {

	struct gpt_header header;

	gpt_header_init(&header);

	return gpt_header_write(stream, &header);
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
	uint32_t i;

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

	err = stream_setpos(stream, header.partitions_array_lba * 512);
	if (err != 0)
		return err;

	for (i = 0; i < header.partition_count; i++) {
		/* TODO : read partitions */
	}

	return 0;
}

enum gpt_error gpt_add_partition(struct stream *stream,
                                 uint32_t *partition_index) {

	enum gpt_error err;
	struct gpt_header header;
	struct gpt_partition partition;

	err = gpt_header_read(stream, &header);
	if (err != GPT_ERROR_NONE)
		return err;

	/* TODO */

	err = gpt_header_write(stream, &header);
	if (err != GPT_ERROR_NONE)
		return err;

	(void) partition;
	(void) partition_index;

	return GPT_ERROR_NONE;
}

