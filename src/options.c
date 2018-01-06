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

#include "options.h"

#include "fdisk.h"

#include <stdlib.h>
#include <string.h>

void
options_init(struct options *options) {
	options->memory = 1 * 1024 * 1024;
	options->disk_array = NULL;
	options->disk_count = 0;
}

void
options_free(struct options *options) {

	unsigned long int i;

	for (i = 0; i < options->disk_count; i++)
		fdisk_close(&options->disk_array[i]);

	free(options->disk_array);

	options->disk_array = NULL;
	options->disk_count = 0;
}

int
options_add_disk(struct options *options,
                 const char *disk_path) {

	struct fdisk *disk;
	struct fdisk *disk_array;
	size_t disk_count;
	size_t disk_array_size;

	disk_array = options->disk_array;

	disk_count = options->disk_count + 1;

	disk_array_size = 0;
	disk_array_size += sizeof(options->disk_array[0]);
	disk_array_size *= disk_count;

	disk_array = realloc(disk_array, disk_array_size);
	if (disk_array == NULL)
		return -1;

	disk = &disk_array[disk_count - 1];

	fdisk_init(disk);

	fdisk_open(disk, disk_path, "r+");

	options->disk_array = disk_array;
	options->disk_count = disk_count;

	return 0;
}

unsigned long int
options_get_memory(const struct options *options) {
	return options->memory;
}

struct disk *
options_get_disk(struct options *options,
                 unsigned long int index) {
	if (index >= options->disk_count) {
		return NULL;
	} else {
		return fdisk_to_disk(&options->disk_array[index]);
	}
}

unsigned long int
options_get_disk_count(const struct options *options) {
	return options->disk_count;
}

int
options_parse_args(struct options *options,
                   int argc, const char **argv) {

	int i;
	int err;

	for (i = 0; i < argc; i++) {
		if (strcmp(argv[i], "--disk") == 0) {

			if ((i + 1) >= argc) {
				return -1;
			}

			err = options_add_disk(options, argv[i + 1]);
			if (err != 0)
				return err;

			i++;

		} else {
			return -1;
		}
	}

	return 0;
}

int
options_set_memory(struct options *options,
                   unsigned long int memory) {

	if (memory == 0)
		return -1;

	options->memory = memory;

	return 0;
}
