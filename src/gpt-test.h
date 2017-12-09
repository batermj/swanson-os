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

#ifndef SWANSON_GPT_TEST_H
#define SWANSON_GPT_TEST_H

#ifdef __cplusplus
extern "C" {
#endif

/** Run the GPT tests.
 * */

void
gpt_test(void);

/** Verifies the calculation
 * of checksums is implemented
 * correctly.
 * */

void
gpt_test_do_checksums(void);

/** Run the 'add_partition' test.
 * */

void
gpt_test_add_partition(void);

/** Run the 'test_format' test.
 * */

void
gpt_test_add_format(void);

/** Test allocation of a GPT source.
 * */

void
gpt_test_allocate(void);

/* TODO:
 *  - gpt_test_write_header
 *  - gpt_test_read_header
 *  - gpt_test_write_partition
 *  - gpt_test_read_partition
 */

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_GPT_TEST_H */
