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

#ifndef SWANSON_KERNEL_H
#define SWANSON_KERNEL_H

#include "fs/any.h"
#include "fs/ramfs/fs.h"
#include "memmap.h"
#include "partition.h"
#include "process.h"
#include "sstream.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** The code returned from the kernel entry point.
 * Indicates whether or not the kernel ran without a panic.
 * @see kernel_main
 * */

enum kernel_exitcode {
	/** Kernel exited without a panic. */
	KERNEL_SUCCESS = 0,
	/** Kernel exited after a panic. */
	KERNEL_FAILURE = 1
};

/** The kernel.
 * */

struct kernel {
	/** Implementation data. */
	void *data;
	/** The kernel's memory map. */
	struct memmap memmap;
	/** The array of disks that the
	 * kernel may read and write to. */
	struct disk *disk_array;
	/** The number of disks in the
	 * disk array. */
	uint64_t disk_count;
	/** The partition containing the
	 * operating system. */
	struct partition root_partition;
	/** The root file system. */
	struct any_fs root_fs;
	/** Initial ramfs file system. */
	struct ramfs initramfs;
	/** The root process. */
	struct process init;
};

/** Initializes the kernel structure.
 * @param kernel An uninitialized kernel structure.
 * */

void
kernel_init(struct kernel *kernel);

/** Releases resources allocated by the kernel.
 * @param kernel An initialized kernel structure.
 * */

void
kernel_free(struct kernel *kernel);

/** Add a disk to the disk array.
 * @param kernel An initialized kernel structure.
 * @param disk An initialized disk structure.
 * @returns Zero on success, non-zero on failure.
 * */

int
kernel_add_disk(struct kernel *kernel,
                struct disk *disk);

/** Add a block of memory for the kernel
 * to use when needed.
 * @param kernel An initialized kernel structure.
 * @param addr The address of the memory block.
 * @param size The size of the memory block, in bytes.
 * @returns Zero on success, non-zero otherwise.
 * */

int
kernel_add_memory(struct kernel *kernel,
                  void *addr,
                  unsigned long int size);

/** Mounts the initial file system executes
 * the 'init' program.
 * @param kernel An initialized kernel structure.
 * @param buf The buffer containing the ramfs.
 * @param buf_size The size of the buffer containing the ramfs.
 * @returns Zero on success, non-zero on failure.
 * */

int
kernel_load_initramfs(struct kernel *kernel,
                      const void *buf,
                      unsigned long int buf_size);

/** Entry point of the kernel.
 * @param kernel an initialized kernel structure.
 * @returns An exit code that indicates whether or
 * not the kernel exited with a panic.
 * */

enum kernel_exitcode
kernel_main(struct kernel *kernel);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_KERNEL_H */
