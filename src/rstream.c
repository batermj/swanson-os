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

#include "rstream.h"

#ifndef NULL
#define NULL ((void *) 0x00)
#endif

static int
rstream_getpos(void *rstream_ptr, uint64_t *pos) {

	struct rstream *rstream = (struct rstream *) rstream_ptr;

	*pos = (uint64_t) rstream->buf_pos;

	return 0;
}

static int
rstream_setpos(void *rstream_ptr, uint64_t pos) {

	struct rstream *rstream = (struct rstream *) rstream_ptr;

	if (pos > rstream->buf_len)
		pos = rstream->buf_len;

	rstream->buf_pos = pos;

	return 0;
}

static uint64_t
rstream_read(void *rstream_ptr,
             void *buf,
             uint64_t buf_size) {

	unsigned char *dst;
	const unsigned char *src;
	uint64_t i;
	uint64_t bytes_available;

	struct rstream *rstream = (struct rstream *) rstream_ptr;

	if (rstream->buf_pos >= rstream->buf_len)
		return 0;

	bytes_available = rstream->buf_len - rstream->buf_pos;

	if (buf_size > bytes_available)
		buf_size = bytes_available;

	dst = (unsigned char *) buf;
	src = (const unsigned char *) rstream->buf;
	src = &src[rstream->buf_pos];

	for (i = 0; i < buf_size; i++)
		dst[i] = src[i];

	rstream->buf_pos += buf_size;

	return buf_size;
}

static uint64_t
rstream_write(void *rstream_ptr,
              const void *buf,
              uint64_t buf_size) {
	(void) rstream_ptr;
	(void) buf;
	(void) buf_size;
	return 0;
}

static int
rstream_getsize(void *rstream_ptr,
                uint64_t *size) {

	struct rstream *rstream = (struct rstream *) rstream_ptr;

	if (size != NULL)
		*size = rstream->buf_len;

	return 0;
}

void
rstream_init(struct rstream *rstream) {
	stream_init(&rstream->stream);
	rstream->stream.data = rstream;
	rstream->stream.read = rstream_read;
	rstream->stream.write = rstream_write;
	rstream->stream.getpos = rstream_getpos;
	rstream->stream.setpos = rstream_setpos;
	rstream->stream.getsize = rstream_getsize;
	rstream->buf = NULL;
	rstream->buf_len = 0;
	rstream->buf_pos = 0;
}

struct stream *
rstream_to_stream(struct rstream *rstream) {
	return &rstream->stream;
}

void
rstream_setbuf(struct rstream *rstream,
               const void *buf,
               uint64_t buf_len) {

	rstream->buf = buf;
	rstream->buf_len = buf_len;
	rstream->buf_pos = 0;
}

