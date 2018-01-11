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

#ifndef SWANSON_FD_H
#define SWANSON_FD_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** The underlying structure behind
 * a file descriptor. Used to represent
 * files, sockets, pipes and more.
 * */

struct fd {
	/** Implementation data */
	void *data;
	/** Closes the file. */
	void (*close)(void *data);
	/** Read callback. */
	int32_t (*read)(void *data, void *buf, uint32_t count);
	/** Write callback. */
	int32_t (*write)(void *data, const void *buf, uint32_t count);
};

/** Initializes a file descriptor.
 * @param fd An uninitialized file descriptor.
 * */

void
fd_init(struct fd *fd);

/** Closes a file descriptor. Calls
 * the close callback, if it is implemented.
 * @param fd An initialized file descriptor.
 * */

void
fd_close(struct fd *fd);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_FD_H */
