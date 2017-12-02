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

void test_format(void) {

	char buf[1024];

	struct sstream sstream;

	sstream_init(&sstream);

	sstream_setbuf(&sstream, &buf, sizeof(buf));

	gpt_format(&sstream.stream);

	/* signature */
	assert(buf[512 + 0] == 'E');
	assert(buf[512 + 1] == 'F');
	assert(buf[512 + 2] == 'I');
	assert(buf[512 + 3] == ' ');
	assert(buf[512 + 4] == 'P');
	assert(buf[512 + 5] == 'A');
	assert(buf[512 + 6] == 'R');
	assert(buf[512 + 7] == 'T');
	/* version */
	assert(buf[512 + 8] == 0x00);
	assert(buf[512 + 9] == 0x00);
	assert(buf[512 + 10] == 0x01);
	assert(buf[512 + 11] == 0x00);
	/* header size */
	assert(buf[512 + 12] == 0x5c);
	assert(buf[512 + 13] == 0x00);
	assert(buf[512 + 14] == 0x00);
	assert(buf[512 + 15] == 0x00);
	/* TODO : header crc32 */
	/* reserved */
	assert(buf[512 + 20] == 0x00);
	assert(buf[512 + 21] == 0x00);
	assert(buf[512 + 22] == 0x00);
	assert(buf[512 + 23] == 0x00);
	/* current LBA */
	assert(buf[512 + 24] == 0x01);
	assert(buf[512 + 25] == 0x00);
	assert(buf[512 + 26] == 0x00);
	assert(buf[512 + 27] == 0x00);
	/* TODO : the rest */
}

int main(void) {
	test_format();
	return EXIT_SUCCESS;
}
