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
#include "disk.h"
#include "gpt.h"

#ifndef NULL
#define NULL ((void *) 0x00)
#endif

static int probe_gpt_header(void *kernel_ptr, const struct gpt_header *header) {
	/* Nothing to do here. This function is a placeholder
	 * just in case the GPT header has to be examined for
	 * some reason. It may be removed in the future. */
	(void) kernel_ptr;
	(void) header;
	return 0;
}

static int probe_gpt_partition(void *kernel_ptr, const struct gpt_partition *partition) {

	(void) kernel_ptr;
	(void) partition;

	if (guid_cmp(&partition->partition_type_guid, &gpt_guid_swanson) != 0) {
		/* Not a Swanson partition. */
		return 0;
	}

	debug("Found Swanson partition.\n");

	return 0;
}

static void gpt_error(void *kernel_ptr, enum gpt_error error) {
	(void) kernel_ptr;
	debug("Failed accessing GPT formatted drive: %s.\n", gpt_strerror(error));
}

static enum kernel_exitcode probe_disk(struct kernel *kernel,
                                       struct disk *disk) {

	int err;
	struct gpt_accessor partition_finder;

	gpt_accessor_init(&partition_finder);
	partition_finder.data = kernel;
	partition_finder.header = probe_gpt_header;
	partition_finder.partition = probe_gpt_partition;
	partition_finder.error = gpt_error;

	err = gpt_access(&disk->stream, &partition_finder);
	if (err != 0)
		debug("Failed to access GPT partitions.\n");

	return KERNEL_FAILURE;
}

static enum kernel_exitcode find_fs(struct kernel *kernel) {

	uint64_t i;
	enum kernel_exitcode exitcode;

	if (kernel->disk_count == 0) {
		debug("No disks found.\n");
		return KERNEL_FAILURE;
	}

	for (i = 0; i < kernel->disk_count; i++) {
		exitcode = probe_disk(kernel, &kernel->disk_array[i]);
		if (exitcode == KERNEL_SUCCESS)
			return KERNEL_SUCCESS;
	}

	return KERNEL_FAILURE;
}

void kernel_init(struct kernel *kernel) {
	vfs_init(&kernel->vfs);
	memmap_init(&kernel->memmap);
	kernel->disk_array = NULL;
	kernel->disk_count = 0;
	partition_init(&kernel->main_partition);
}

enum kernel_exitcode kernel_main(struct kernel *kernel) {

	int err;
	struct vfs_file init_file;
	const char *init_path = "/sbin/init";

	err = find_fs(kernel);
	if (err != KERNEL_SUCCESS) {
		debug("Failed to find file system.\n");
		return err;
	}

	err = vfs_open_file(&kernel->vfs, &init_file, init_path);
	if (err != 0) {
		debug("Failed to open '%s'.\n", init_path);
		return KERNEL_FAILURE;
	}

	return KERNEL_SUCCESS;
}
