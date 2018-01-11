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

#include "fd.h"

#ifndef NULL
#define NULL ((void *) 0x00)
#endif

void
fd_init(struct fd *fd) {
	fd->data = NULL;
	fd->close = NULL;
	fd->read = NULL;
	fd->write = NULL;
}

void
fd_close(struct fd *fd) {
	if (fd->close != NULL)
		fd->close(fd->data);
}
