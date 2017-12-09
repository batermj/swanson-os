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

#ifndef SWANSON_SSTREAM_H
#define SWANSON_SSTREAM_H

#include "stream.h"

#ifdef __cplusplus
extern "C" {
#endif

/** A string stream. Used
 * for reading and writing to
 * a block of memory as if it were
 * a string.
 * */

struct sstream {
	/** The base stream structure. */
	struct stream stream;
	/** The string buffer. */
	void *buf;
	/** The number of bytes in
	 * the string buffer. */
	uint64_t buf_len;
	/** The position of the stream
	 * within the buffer. */
	uint64_t buf_pos;
};

/** Initializes the string stream
 * and the stream callbacks.
 * @param sstream An uninitialized string stream.
 * */

void
sstream_init(struct sstream *sstream);

/** Convert a string stream to
 * a base stream structure.
 * @param sstream An initialized string stream.
 * @returns A base stream structure. This
 * function does not return a null pointer.
 * */

struct stream *
sstream_to_stream(struct sstream *sstream);

/** Assigns the buffer to use for
 * the string data.
 * @param sstream An initialized string stream
 * structure.
 * @param buf The buffer to associate with the
 * string stream.
 * @param buf_size The size of the buffer.
 * */

void
sstream_setbuf(struct sstream *sstream,
               void *buf,
               uint64_t buf_size);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_SSTREAM_H */
