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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "kernel.h"
#include "options.h"

#ifdef SWANSON_WITH_INITRAMFS_DATA_H
#include "initramfs-data.h"
#endif /* SWANSON_WITH_INITRAMFS_DATA_H */

static int
add_memory_from_options(struct kernel *kernel,
                        const struct options *options) {

	int err;
	void *memory;
	unsigned long int memory_size;

	memory_size = options_get_memory(options);

	memory = malloc(memory_size);
	if (memory == NULL)
		return -1;

	err = kernel_add_memory(kernel, memory, memory_size);
	if (err != 0) {
		free(memory);
		return err;
	}

	return 0;
}

static int
add_disks_from_options(struct kernel *kernel,
                       struct options *options) {

	int err;
	struct disk *disk;
	unsigned long int disk_count;
	unsigned long int i;

	disk_count = options_get_disk_count(options);

	for (i = 0; i < disk_count; i++) {

		disk = options_get_disk(options, i);
		if (disk == NULL)
			continue;

		err = kernel_add_disk(kernel, disk);
		if (err != 0)
			continue;
	}

	return 0;
}

static int
parse_args(struct kernel *kernel,
           int argc, const char **argv) {

	int err;
	struct options options;

	options_init(&options);

	err = options_parse_args(&options, argc, argv);
	if (err != 0) {
		options_free(&options);
		return err;
	}

	err = add_memory_from_options(kernel, &options);
	if (err != 0) {
		options_free(&options);
		return err;
	}

	err = add_disks_from_options(kernel, &options);
	if (err != 0) {
		options_free(&options);
		return err;
	}

	options_free(&options);

	return 0;
}

static void
free_kernel_memory(struct kernel *kernel) {
	unsigned long int i;
	for (i = 0; i < kernel->memmap.unused_section_count; i++) {
		free(kernel->memmap.unused_section_array[0].addr);
	}
	kernel_free(kernel);
}

int
main(int argc, const char **argv) {

	int err;
	enum kernel_exitcode exitcode;
	struct kernel kernel;

	kernel_init(&kernel);

	err = parse_args(&kernel, argc - 1, &argv[1]);
	if (err != 0) {
		free_kernel_memory(&kernel);
		return EXIT_FAILURE;
	}

	err = kernel_load_initramfs(&kernel,
	                            initramfs_data,
	                            initramfs_data_size);
	if (err != 0) {
		free_kernel_memory(&kernel);
		return EXIT_FAILURE;
	}

	exitcode = kernel_main(&kernel);

	free_kernel_memory(&kernel);

	if (exitcode == KERNEL_SUCCESS)
		return EXIT_SUCCESS;
	else
		return EXIT_FAILURE;
}
