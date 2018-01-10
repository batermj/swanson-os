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

#include "stream.h"

#include "null.h"

void
stream_init(struct stream *stream) {
	stream->data = NULL;
	stream->getpos = NULL;
	stream->setpos = NULL;
	stream->read = NULL;
	stream->write = NULL;
	stream->getsize = NULL;
}

void
stream_copy(struct stream *dst,
            struct stream *src) {
	dst->data = src->data;
	dst->getpos = src->getpos;
	dst->setpos = src->setpos;
	dst->read = src->read;
	dst->write = src->write;
	dst->getsize = src->getsize;
}

int
stream_getpos(struct stream *stream,
              uint64_t *pos) {
	if (stream->getpos == NULL)
		return -1;
	else
		return stream->getpos(stream->data, pos);
}

int
stream_setpos(struct stream *stream,
              uint64_t pos) {
	if (stream->setpos == NULL)
		return -1;
	else
		return stream->setpos(stream->data, pos);
}

uint64_t
stream_read(struct stream *stream,
            void *buf,
            uint64_t buf_size) {
	if (stream->read == NULL)
		return 0;
	else
		return stream->read(stream->data, buf, buf_size);
}

uint64_t
stream_write(struct stream *stream,
             const void *buf,
             uint64_t buf_size) {
	if (stream->write == NULL)
		return 0;
	else
		return stream->write(stream->data, buf, buf_size);
}

uint64_t
stream_decode_uint16be(struct stream *stream,
                       uint16_t *n_ptr) {

	uint8_t buf[2];
	uint64_t read_count;
	uint16_t n;

	read_count = stream_read(stream, buf, sizeof(buf));
	if (read_count != sizeof(buf))
		return read_count;

	n = 0;
	n |= (buf[0] << 0x08) & 0x0000ff00;
	n |= (buf[1] << 0x00) & 0x000000ff;

	if (n_ptr != NULL)
		*n_ptr = n;

	return read_count;
}

uint64_t
stream_decode_uint16le(struct stream *stream,
                       uint16_t *n_ptr) {

	uint8_t buf[2];
	uint64_t read_count;
	uint16_t n;

	read_count = stream_read(stream, buf, sizeof(buf));
	if (read_count != sizeof(buf))
		return read_count;

	n = 0;
	n |= (buf[0] << 0x00) & 0x000000ff;
	n |= (buf[1] << 0x08) & 0x0000ff00;

	if (n_ptr != NULL)
		*n_ptr = n;

	return read_count;
}

uint64_t
stream_decode_uint32be(struct stream *stream,
                       uint32_t *n_ptr) {

	uint8_t buf[4];
	uint64_t read_count;
	uint32_t n;

	read_count = stream_read(stream, buf, sizeof(buf));
	if (read_count != sizeof(buf))
		return read_count;

	n = 0;
	n |= (buf[0] << 0x18) & 0xff000000;
	n |= (buf[1] << 0x10) & 0x00ff0000;
	n |= (buf[2] << 0x08) & 0x0000ff00;
	n |= (buf[3] << 0x00) & 0x000000ff;

	if (n_ptr != NULL)
		*n_ptr = n;

	return read_count;
}

uint64_t
stream_decode_uint32le(struct stream *stream,
                       uint32_t *n_ptr) {

	uint8_t buf[4];
	uint64_t read_count;
	uint32_t n;

	read_count = stream_read(stream, buf, sizeof(buf));
	if (read_count != sizeof(buf))
		return read_count;

	n = 0;
	n |= (buf[0] << 0x00) & 0x000000ff;
	n |= (buf[1] << 0x08) & 0x0000ff00;
	n |= (buf[2] << 0x10) & 0x00ff0000;
	n |= (buf[3] << 0x18) & 0xff000000;

	if (n_ptr != NULL)
		*n_ptr = n;

	return read_count;
}

uint64_t
stream_decode_uint64le(struct stream *stream,
                       uint64_t *n_ptr) {

	uint8_t buf[8];
	uint64_t read_count;
	uint64_t n;

	read_count = stream_read(stream, buf, sizeof(buf));
	if (read_count != sizeof(buf))
		return read_count;

	n = 0;

	n |= (buf[4] << 0x00);
	n |= (buf[5] << 0x08);
	n |= (buf[6] << 0x10);
	n |= (buf[7] << 0x18);

	n <<= 32;

	n |= (buf[0] << 0x00);
	n |= (buf[1] << 0x08);
	n |= (buf[2] << 0x10);
	n |= (buf[3] << 0x18);

	if (n_ptr != NULL)
		*n_ptr = n;

	return read_count;
}

uint64_t
stream_encode_uint16le(struct stream *stream,
                       uint16_t n) {
	uint8_t buf[2];
	buf[0] = (n >> 0x00) & 0xff;
	buf[1] = (n >> 0x08) & 0xff;
	return stream_write(stream, buf, 2);
}

uint64_t
stream_encode_uint32le(struct stream *stream,
                       uint32_t n) {

	uint8_t buf[4];

	buf[0] = (n >> 0x00) & 0xff;
	buf[1] = (n >> 0x08) & 0xff;
	buf[2] = (n >> 0x10) & 0xff;
	buf[3] = (n >> 0x18) & 0xff;

	return stream_write(stream, buf, 4);
}

uint64_t
stream_encode_uint64le(struct stream *stream,
                       uint64_t n) {

	uint8_t buf[8];

	buf[0] = (n >> 0x00) & 0xff;
	buf[1] = (n >> 0x08) & 0xff;
	buf[2] = (n >> 0x10) & 0xff;
	buf[3] = (n >> 0x18) & 0xff;
	buf[4] = (n >> 0x20) & 0xff;
	buf[5] = (n >> 0x28) & 0xff;
	buf[6] = (n >> 0x30) & 0xff;
	buf[7] = (n >> 0x38) & 0xff;

	return stream_write(stream, buf, 8);
}

int
stream_getsize(struct stream *stream,
               uint64_t *size) {
	if (stream->getsize == NULL)
		return -1;
	else
		return stream->getsize(stream->data, size);
}
