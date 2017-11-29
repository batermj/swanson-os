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

#ifndef SWANSON_KERNEL_H
#define SWANSON_KERNEL_H

#include "memmap.h"
#include "vfs.h"

#ifdef __cplusplus
extern "C" {
#endif

/** The code returned from the kernel entry point.
 * Indicates whether or not the kernel ran without a panic. */
enum kernel_exitcode {
	/** Kernel exited without a panic. */
	KERNEL_SUCCESS = 0,
	/** Kernel exited after a panic. */
	KERNEL_FAILURE = 1
};

/** The kernel */
struct kernel {
	/** Implementation data. */
	void *data;
	/** The kernel's virtual file system. */
	struct vfs vfs;
	/** The kernel's memory map. */
	struct memmap memmap;
};

/** Initializes the kernel structure. */
void kernel_init(struct kernel *kernel);

/** Entry point of the kernel. */
enum kernel_exitcode kernel_main(struct kernel *kernel);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_KERNEL_H */
