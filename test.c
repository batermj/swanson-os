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

#include "test.h"

#include <stdio.h>
#include <stdlib.h>

#include "memmap-test.h"

int
main(void) {

	enum test_exitcode exitcode;

	struct test_driver driver;

	const struct test test_array[] = {
		UNIT_TEST(test_alloc)
	};

	unsigned int i;
	unsigned int test_count;

	driver.errlog = stderr;

	test_count = 0;
	test_count += sizeof(test_array);
	test_count /= sizeof(test_array[0]);

	for (i = 0; i < test_count; i++) {
		printf("Running test '%s'.\n", test_array[i].name);
		exitcode = test_array[i].run(&driver);
		if (exitcode == TEST_FAILURE)
			return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
