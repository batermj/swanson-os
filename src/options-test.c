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

#include "assert.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
test_parse_args(void) {

	int err;
	struct options options;
	struct disk *disk;
	int argc = 4;
	const char *argv[] = {
		"--disk", "disk1.img",
		"--disk", "disk2.img",
		NULL
	};

	options_init(&options);

	err = options_parse_args(&options, argc, argv);
	assert(err == 0);

	assert(options_get_disk_count(&options) == 2);

	disk = options_get_disk(&options, 0);
	assert(disk != NULL);

	disk = options_get_disk(&options, 1);
	assert(disk != NULL);

	disk = options_get_disk(&options, 2);
	assert(disk == NULL);

	options_free(&options);
}

void
options_test(void) {
	test_parse_args();
}
