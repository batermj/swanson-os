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

#include "path-test.h"

#include "assert.h"
#include "path.h"

#include <string.h>

static void
path_test_parse(void) {

	int err;
	struct path path;

	path_init(&path);

	err = path_parse(&path, "/usr/./../bin/bash");
	assert(err == 0);

	assert(path.name_count == 5);

	assert(path.name_array[0].size == 3);
	assert(strcmp(path.name_array[0].data, "usr") == 0);

	assert(path.name_array[1].size == 1);
	assert(strcmp(path.name_array[1].data, ".") == 0);

	assert(path.name_array[2].size == 2);
	assert(strcmp(path.name_array[2].data, "..") == 0);

	assert(path.name_array[3].size == 3);
	assert(strcmp(path.name_array[3].data, "bin") == 0);

	assert(path.name_array[4].size == 4);
	assert(strcmp(path.name_array[4].data, "bash") == 0);

	path_free(&path);
}

void
path_test(void) {
	path_test_parse();
}
