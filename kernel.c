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

#include "kernel.h"

#include "debug.h"

#ifndef NULL
#define NULL ((void *) 0x00)
#endif

void kernel_init(struct kernel *kernel) {
	vfs_init(&kernel->vfs);
	memmap_init(&kernel->memmap);
	kernel->disk_array = NULL;
	kernel->disk_count = 0;
}

enum kernel_exitcode kernel_main(struct kernel *kernel) {

	int err;
	struct vfs_file init_file;
	const char *init_path = "/sbin/init";

	err = vfs_open_file(&kernel->vfs, &init_file, init_path);
	if (err != 0) {
		debug("Failed to open '%s'\n", init_path);
		return KERNEL_FAILURE;
	}

	return KERNEL_SUCCESS;
}
