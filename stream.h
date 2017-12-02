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

#ifndef SWANSON_STREAM_H
#define SWANSON_STREAM_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** An abstract data reader and writer.
 * */

struct stream {
	/** Implementation data. */
	void *data;
	/** Read callback. */
	uint64_t (*read)(void *data, void *buf, uint64_t buf_size);
	/** Write callback. */
	uint64_t (*write)(void *data, const void *buf, uint64_t buf_size);
	/** Get the position of the stream. */
	int (*getpos)(void *data, uint64_t *offset);
	/** Set the position of the stream. */
	int (*setpos)(void *data, uint64_t offset);
};

/** Initializes the stream members
 * to null. After this function is
 * called, the callback functions
 * may be set.
 * @param stream An uninitialized
 * stream structure.
 * */

void stream_init(struct stream *stream);

/** Read from a stream.
 * @param stream The stream to read from.
 * @param buf The buffer to put the data in.
 * @param buf_size The number of bytes to read.
 * @returns The number of bytes read.
 * */

uint64_t stream_read(struct stream *stream,
                     void *buf,
                     uint64_t buf_size);

/** Write to a stream.
 * @param stream The stream to write to.
 * @param buf The buffer to write to the stream.
 * @param buf_size The number of bytes to write.
 * @returns the number of bytes written.
 * */

uint64_t stream_write(struct stream *stream,
                      const void *buf,
                      uint64_t buf_size);

/** Encodes a binary, 32-bit, little-endian number
 * into a stream at the current position.
 * @param stream An initialized stream structure.
 * @param n The number to encode.
 * @returns The number of bytes written.
 * */

uint64_t stream_encode_uint32le(struct stream *stream,
                                uint32_t n);

/** Encodes a binary, 64-bit, little-endian number
 * into a stream at the current position.
 * @param stream An initialized stream structure.
 * @param n The number to encode.
 * @returns The number of bytes written.
 * */

uint64_t stream_encode_uint64le(struct stream *stream,
                                uint64_t n);

/** Get the stream position.
 * @param stream The stream to get the position of.
 * @param offset The position of the stream.
 * @returns Zero on success, non-zero otherwise.
 * */

int stream_getpos(struct stream *stream,
                  uint64_t *offset);

/** Set the stream position.
 * @param stream The stream to set the position of.
 * @param offset The new stream position.
 * @returns Zero on success, non-zero otherwise.
 * */

int stream_setpos(struct stream *stream,
                  uint64_t offset);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_STREAM_H */
