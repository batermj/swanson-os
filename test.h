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

/** Runs the tests.
 * Used for logging and displaying
 * test results.
 * */

struct test_driver {
	/** The file to write the errors to. */
	FILE *errlog;
};

void
test_driver_init(struct test_driver *driver);

void
test_driver_log_error(struct test_driver *driver,
                      const char *fmt, ...);

void
test_driver_dump(struct test_driver *driver,
                 const char *variable_name,
                 const void *buf,
                 const void *buf_size);

/** A unit test.
 * This will typically be
 * useful when declared as
 * an array.
 * */

struct test {
	/** The name of the unit test. */
	const char *name;
	/** The callback function. This
	 * will run the unit test. */
	enum test_exitcode (*run)(struct test_driver *driver);
};

/** Tests if to values are equal.
 * Must be called in a function defined
 * with @ref TEST_F.
 * */

#define ASSERT_EQ(a,b) \
	if (a != b) { \
		fprintf(driver->errlog, "%s:%u assertion failed.\n", __FILE__, __LINE__); \
		return TEST_FAILURE; \
	}

/** Defines a test function.
 * A test function is a function used
 * for running a unit test.
 * */

#define TEST_F(function) \
	enum test_exitcode \
	function(struct test_driver *driver)

/** Used as a convient way to
 * add a @ref test structure to
 * a test array when the name of
 * the test and the name of the
 * function that runs it are the
 * same.
 * */

#define UNIT_TEST(function) \
	{ #function, function }

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_TEST_H */
