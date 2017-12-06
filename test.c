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

#include "gpt-test.h"

int
main(void) {

	enum test_exitcode exitcode;
	struct test_options options;

	test_options_init(&options);

	exitcode = test_run(&options);
	if (exitcode == TEST_FAILURE)
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}

void
test_options_init(struct test_options *options) {
	options->output_log = stdout;
	options->error_log = stderr;
}

enum test_exitcode
test_run(const struct test_options *options) {

	enum test_exitcode exitcode;

	exitcode = gpt_test(options);
	if (exitcode == TEST_FAILURE)
		return exitcode;

	return TEST_SUCCESS;
}

