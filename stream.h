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

#ifndef SWANSON_STREAM_H
#define SWANSON_STREAM_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** An abstract data reader and writer. */
struct stream {
	/** Implementation data. */
	void *data;
	/** Write callback. */
	uint64_t (*write)(void *data, const void *buf, uint64_t buf_size);
	/** Read callback. */
	uint64_t (*read)(void *data, void *buf, uint64_t buf_size);
	/** Get the position of the stream. */
	int (*getpos)(void *data, uint64_t *offset);
	/** Set the position of the stream. */
	int (*setpos)(void *data, uint64_t offset);
};

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_STREAM_H */
