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

#ifndef SWANSON_TEST_H
#define SWANSON_TEST_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Indicates whether or not
 * the test passed.
 * */

enum test_exitcode {
	/** The test passed. */
	TEST_SUCCESS,
	/** The test failed. */
	TEST_FAILURE
};

struct test_options {
	/** File for output logging. */
	FILE *output_log;
	/** File for error logging. */
	FILE *error_log;
};

/** Initializes the test options
 * to their default values.
 * */

void
test_options_init(struct test_options *options);

/** Runs all tests.
 * @param options An options structure
 * that gets passed to all tests.
 * @returns See @ref test_exitcode.
 * */

enum test_exitcode
test_run(const struct test_options *options);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_TEST_H */
