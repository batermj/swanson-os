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
	/** Get the size, in bytes, of the stream. */
	int (*getsize)(void *data, uint64_t *size);
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

/** Decodes a binary, 16-bit, little-endian number
 * from a stream at the current position.
 * @param stream An initialized stream structure.
 * @param n A pointer to a 16-bit unsigned integer
 * that the number will be decoded to.
 * @returns The number of bytes read. On success,
 * this number should be equal to four.
 * */

uint64_t stream_decode_uint16le(struct stream *stream,
                                uint16_t *n);

/** Decodes a binary, 32-bit, little-endian number
 * from a stream at the current position.
 * @param stream An initialized stream structure.
 * @param n A pointer to a 32-bit unsigned integer
 * that the number will be decoded to.
 * @returns The number of bytes read. On success,
 * this number should be equal to four.
 * */

uint64_t stream_decode_uint32le(struct stream *stream,
                                uint32_t *n);

/** Decodes a binary, 64-bit, little-endian number
 * from a stream at the current position.
 * @param stream An initialized stream structure.
 * @param n A pointer to a 64-bit unsigned integer
 * that the number will be decoded to.
 * @returns The number of bytes read. On success,
 * this number should be equal to eight.
 * */

uint64_t stream_decode_uint64le(struct stream *stream,
                                uint64_t *n);

/** Encodes a binary, 16-bit, little-endian number
 * into a stream at the current position.
 * @param stream An initialized stream structure.
 * @param n The number to encode.
 * @returns The number of bytes written.
 * */

uint64_t stream_encode_uint16le(struct stream *stream,
                                uint16_t n);

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

/** Get the size of the stream. In other
 * words, get the number of bytes occupied
 * by the data that is currently in the stream.
 * @param stream An initialized stream structure.
 * @param size The address to the variable that
 * receives the stream size.
 * @returns Zero on success, a negative error
 * code otherwise.
 * */

int stream_getsize(struct stream *stream,
                   uint64_t *size);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_STREAM_H */
