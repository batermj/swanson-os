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

#ifndef SWANSON_ASSERT_H
#define SWANSON_ASSERT_H

#ifdef __cplusplus
extern "C" {
#endif

/** Used for detected programming
 * errors.
 * */

#define assert(expr) \
	if (!(expr)) \
		assert_failure(__FILE__, __LINE__, #expr)

/** Called when a call to
 * @ref assert evaluates to be false.
 * @param filename The filename of code
 * containing the assertion.
 * @param line The line within the file
 * that the assertion is placed.
 * @param expr The expression that evaluated
 * to be false.
 * */

void
assert_failure(const char *filename,
               unsigned long int line,
               const char *expr);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_ASSERT_H */
