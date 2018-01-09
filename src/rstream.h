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

#ifndef SWANSON_RSTREAM_H
#define SWANSON_RSTREAM_H

#include "stream.h"

#ifdef __cplusplus
extern "C" {
#endif

/** A read-only stream. Used
 * for reading data compiled by the
 * resource compiler.
 * */

struct rstream {
	/** The base stream structure. */
	struct stream stream;
	/** The string buffer. */
	const void *buf;
	/** The number of bytes in
	 * the string buffer. */
	uint64_t buf_len;
	/** The position of the stream
	 * within the buffer. */
	uint64_t buf_pos;
};

/** Initializes the read-only stream
 * and the stream callbacks.
 * @param rstream An uninitialized read-only stream.
 * */

void
rstream_init(struct rstream *rstream);

/** Convert a read-only stream to
 * a base stream structure.
 * @param rstream An initialized read-only stream.
 * @returns A base stream structure. This
 * function does not return a null pointer.
 * */

struct stream *
rstream_to_stream(struct rstream *rstream);

/** Assigns the buffer to use for the data.
 * @param rstream An initialized read-only stream structure.
 * @param buf The buffer to associate with the read-only stream.
 * @param buf_size The size of the buffer.
 * */

void
rstream_setbuf(struct rstream *rstream,
               const void *buf,
               uint64_t buf_size);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_RSTREAM_H */
