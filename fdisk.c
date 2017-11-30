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

#include "fdisk.h"

#include <limits.h>

#ifndef NULL
#define NULL ((void *) 0x00)
#endif

static int fdisk_getpos(void *fdisk_ptr, uint64_t *pos) {

	struct fdisk *fdisk = (struct fdisk *) fdisk_ptr;

	if (fdisk->file == NULL)
		return -1;

	long int pos2 = ftell(fdisk->file);
	if (pos2 < 0)
		return -1;

	*pos = (uint64_t) pos2;

	return 0;
}

static int fdisk_setpos(void *fdisk_ptr, uint64_t pos) {

	struct fdisk *fdisk = (struct fdisk *) fdisk_ptr;

	if (fdisk->file == NULL)
		return -1;

	if (pos > LONG_MAX)
		pos = LONG_MAX;

	int err = fseek(fdisk->file, (long int) pos, SEEK_SET);
	if (err != 0)
		return -1;

	return 0;
}

static uint64_t fdisk_read(void *fdisk_ptr,
                           void *buf,
                           uint64_t buf_size) {

	struct fdisk *fdisk = (struct fdisk *) fdisk_ptr;

	return fread(buf, 1, buf_size, fdisk->file);
}

static uint64_t fdisk_write(void *fdisk_ptr,
                            const void *buf,
                            uint64_t buf_size) {

	struct fdisk *fdisk = (struct fdisk *) fdisk_ptr;

	return fwrite(buf, 1, buf_size, fdisk->file);
}

void fdisk_init(struct fdisk *fdisk) {
	fdisk->base.stream.data = fdisk;
	fdisk->base.stream.read = fdisk_read;
	fdisk->base.stream.write = fdisk_write;
	fdisk->base.stream.getpos = fdisk_getpos;
	fdisk->base.stream.setpos = fdisk_setpos;
	fdisk->file = NULL;
}

void fdisk_close(struct fdisk *fdisk) {
	if (fdisk->file != NULL) {
		fclose(fdisk->file);
		fdisk->file = NULL;
	}
}

int fdisk_open(struct fdisk *fdisk,
               const char *path,
               const char *mode) {

	if (fdisk->file != NULL)
		fclose(fdisk->file);

	fdisk->file = fopen(path, mode);
	if (fdisk->file == NULL)
		return -1;

	return 0;
}
