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
	signature[0] = 'E';
	signature[1] = 'F';
	signature[2] = 'I';
	signature[3] = ' ';
	signature[4] = 'P';
	signature[5] = 'A';
	signature[6] = 'R';
	signature[7] = 'T';
	header->version = 0x00010000;
	header->header_size = 92;
	header->header_crc32 = 0;
	header->reserved = 0;
	header->current_lba = 1;
	header->backup_lba = 1;
	header->first_usable_lba = ~0x00ULL;
	header->last_usable_lba = ~0x00ULL;
	header->disk_guid[0] = 0;
	header->partitions_array_lba = ~0x00ULL;
	header->partition_count = 0;
	header->partition_entry_size = 128;
	header->partition_array_crc32 = 0;
}

enum gpt_error gpt_header_write(struct stream *stream,
                                const struct gpt_header *header) {

	uint64_t write_count = 0;

	write_count += stream_write(stream, "EFI PART", 8)
	write_count += stream_encode_uint32le(stream, 0x0010000 /* version */);
	write_count += stream_encode_uint32le(stream, 92 /* header size */);
	write_count += stream_encode_uint32le(stream, 0 /* header checksum */);
	write_count += stream_encode_uint32le(stream, 0 /* reserved */);
	write_count += stream_encode_uint64le(stream, header->current_lba);
	write_count += stream_encode_uint64le(
	if (write_count != header->header_size)
		return GPT_ERROR_UNKNOWN;

	/* TODO : calculate header checksum */

	return GPT_ERROR_NONE;
}

enum gpt_error gpt_format(struct stream *stream) {

	int err;
	uint64_t write_count = 0;
	unsigned char sector[512];
	struct sstream lb1;

	sstream_init(&lb1);

	sstream_setbuf(&lb1, sector, sizeof(sector));

	/* signature */
	write_count += stream_write(&lb1.stream, "EFI PART", 8);
	/* revision (1.0) */
	write_count += stream_write(&lb1.stream, "\x00\x00\x01\x00", 4);
	/* header size (92 bytes) */
	write_count += stream_write(&lb1.stream, "\x5c\x00\x00\x00", 4);
	/* header crc32 (calculated later) */
	write_count += stream_write(&lb1.stream, "\x00\x00\x00\x00", 4);
	/* reserved */
	write_count += stream_write(&lb1.stream, "\x00\x00\x00\x00", 4);
	/* current LBA (0x01) */
	write_count += stream_write(&lb1.stream, "\x01\x00\x00\x00\x00\x00\x00\x00", 8);
/*
	if (write_count != 92)
		return GPT_ERROR_UNKNOWN;
*/
	err = stream_setpos(stream, 512);
	if (err != 0)
		return GPT_ERROR_UNKNOWN;

	write_count = stream_write(stream, sector, sizeof(sector));
	if (write_count != 512)
		return GPT_ERROR_UNKNOWN;

	return GPT_ERROR_NONE;
}
