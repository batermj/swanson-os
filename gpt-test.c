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

#include "gpt-test.h"

#include "gpt.h"
#include "null.h"

#include <assert.h>

void
gpt_test_allocate(void) {

}

void
gpt_test_add_partition(void) {

	enum gpt_error error;
	struct gpt_source source;

	gpt_source_init(&source);

	error = gpt_source_format(&source);
	assert(error == GPT_ERROR_NONE);

	error = gpt_source_add_partition(&source, 0, NULL);
	assert(error == GPT_ERROR_NONE);
}

void
gpt_test_format(void) {

}

void
gpt_test(void) {
	gpt_test_allocate();
	gpt_test_add_partition();
	gpt_test_format();
}
