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

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "gpt.h"
#include "sstream.h"
#include "test.h"

struct mock_source {
	struct gpt_source source;
};

static int
mock_write_header(void *source_ptr,
                  const struct gpt_header *header) {

	(void) source_ptr;
	(void) header;

	return 0;
}

static int
mock_write_header_backup(void *source_ptr,
                         const struct gpt_header *header) {

	(void) source_ptr;
	(void) header;

	return 0;
}

static void
mock_source_init(struct mock_source *source) {
	gpt_source_init(&source->source);
	source->source.data = source;
	source->source.write_header = mock_write_header;
	source->source.write_header_backup = mock_write_header_backup;
}

TEST_F(test_format) {

	enum gpt_error error;
	struct mock_source source;

	mock_source_init(&source);

	error = gpt_source_format(&source.source);
	ASSERT_EQ(error, GPT_ERROR_NONE);

	return TEST_SUCCESS;
}

TEST_F(gpt_test) {

	enum test_exitcode exitcode;

	exitcode = test_format(driver);
	ASSERT_EQ(exitcode, TEST_SUCCESS);

	return TEST_SUCCESS;
}
