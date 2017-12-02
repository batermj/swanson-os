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

#ifndef NULL
#define NULL ((void *) 0x00)
#endif

void stream_init(struct stream *stream) {
	stream->data = NULL;
	stream->getpos = NULL;
	stream->setpos = NULL;
	stream->read = NULL;
	stream->write = NULL;
}

int stream_getpos(struct stream *stream,
                  uint64_t *pos) {
	if (stream->getpos == NULL)
		return -1;
	else
		return stream->getpos(stream->data, pos);
}

int stream_setpos(struct stream *stream,
                  uint64_t pos) {
	if (stream->setpos == NULL)
		return -1;
	else
		return stream->setpos(stream->data, pos);
}

uint64_t stream_read(struct stream *stream,
                     void *buf,
                     uint64_t buf_size) {
	if (stream->read == NULL)
		return 0;
	else
		return stream->read(stream->data, buf, buf_size);
}

uint64_t stream_write(struct stream *stream,
                      const void *buf,
                      uint64_t buf_size) {
	if (stream->write == NULL)
		return 0;
	else
		return stream->write(stream->data, buf, buf_size);
}

uint64_t stream_encode_uint32le(struct stream *stream,
                                uint32_t n) {

	uint8_t buf_data[4];

	buf_data[0] = (n >> 0x00) & 0xff;
	buf_data[1] = (n >> 0x08) & 0xff;
	buf_data[2] = (n >> 0x10) & 0xff;
	buf_data[3] = (n >> 0x18) & 0xff;

	return stream_write(stream, buf, 4);
}

uint64_t stream_encode_uint64le(struct stream *stream,
                                uint64_t n) {

	uint8_t buf_data[8];

	buf_data[0] = (n >> 0x00) & 0xff;
	buf_data[1] = (n >> 0x08) & 0xff;
	buf_data[2] = (n >> 0x10) & 0xff;
	buf_data[3] = (n >> 0x18) & 0xff;
	buf_data[4] = (n >> 0x20) & 0xff;
	buf_data[5] = (n >> 0x28) & 0xff;
	buf_data[6] = (n >> 0x30) & 0xff;
	buf_data[7] = (n >> 0x38) & 0xff;

	return stream_write(stream, buf, 8);
}
