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

#include "sstream.h"

#include "sstream.h"

#ifndef NULL
#define NULL ((void *) 0x00)
#endif

static int sstream_getpos(void *sstream_ptr, uint64_t *pos) {

	struct sstream *sstream = (struct sstream *) sstream_ptr;

	*pos = (uint64_t) sstream->buf_pos;

	return 0;
}

static int sstream_setpos(void *sstream_ptr, uint64_t pos) {

	struct sstream *sstream = (struct sstream *) sstream_ptr;

	if (pos > sstream->buf_len)
		pos = sstream->buf_len;

	sstream->buf_pos = pos;

	return 0;
}

static uint64_t sstream_read(void *sstream_ptr,
                             void *buf,
                             uint64_t buf_size) {

	unsigned char *dst;
	const unsigned char *src;
	uint64_t i;
	uint64_t bytes_available;

	struct sstream *sstream = (struct sstream *) sstream_ptr;

	if (sstream->buf_pos >= sstream->buf_len)
		return 0;

	bytes_available = sstream->buf_len - sstream->buf_pos;

	if (buf_size > bytes_available)
		buf_size = bytes_available;

	dst = (unsigned char *) buf;
	src = (const unsigned char *) sstream->buf;
	src = &src[sstream->buf_pos];

	for (i = 0; i < buf_size; i++)
		dst[i] = src[i];

	sstream->buf_pos += buf_size;

	return buf_size;
}

static uint64_t sstream_write(void *sstream_ptr,
                              const void *buf,
                              uint64_t buf_size) {

	unsigned char *dst;
	const unsigned char *src;
	uint64_t i;
	uint64_t bytes_available;

	struct sstream *sstream = (struct sstream *) sstream_ptr;

	if (sstream->buf_pos >= sstream->buf_len)
		return 0;

	bytes_available = sstream->buf_len - sstream->buf_pos;

	if (buf_size > bytes_available)
		buf_size = bytes_available;

	src = (const unsigned char *) buf;
	dst = (unsigned char *) sstream->buf;
	dst = &dst[sstream->buf_pos];

	for (i = 0; i < buf_size; i++)
		dst[i] = src[i];

	sstream->buf_pos += buf_size;

	return buf_size;
}

static int sstream_getsize(void *sstream_ptr,
                           uint64_t *size) {

	struct sstream *sstream = (struct sstream *) sstream_ptr;

	if (size != NULL)
		*size = sstream->buf_len;

	return 0;
}

void sstream_init(struct sstream *sstream) {
	stream_init(&sstream->stream);
	sstream->stream.data = sstream;
	sstream->stream.read = sstream_read;
	sstream->stream.write = sstream_write;
	sstream->stream.getpos = sstream_getpos;
	sstream->stream.setpos = sstream_setpos;
	sstream->stream.getsize = sstream_getsize;
	sstream->buf = NULL;
	sstream->buf_len = 0;
	sstream->buf_pos = 0;
}

void sstream_setbuf(struct sstream *sstream,
                    void *buf,
                    uint64_t buf_len) {
	sstream->buf = buf;
	sstream->buf_len = buf_len;
	sstream->buf_pos = 0;
}

