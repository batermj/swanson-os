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
