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

	char *buf;
	char *ptr;
	enum gpt_error error;
	struct sstream sstream;

	buf = malloc(GPT_MINIMUM_DISK_SIZE);
	assert(buf != NULL);

	memset(buf, 0, GPT_MINIMUM_DISK_SIZE);

	sstream_init(&sstream);

	sstream_setbuf(&sstream, buf, GPT_MINIMUM_DISK_SIZE);

	error = gpt_format(&sstream.stream);
	assert(error == GPT_ERROR_NONE);

	/* move pointer to start of
	 * GPT header */
	ptr = &buf[GPT_START_OFFSET];
	/* signature */
	assert(ptr[0] == 'E');
	assert(ptr[1] == 'F');
	assert(ptr[2] == 'I');
	assert(ptr[3] == ' ');
	assert(ptr[4] == 'P');
	assert(ptr[5] == 'A');
	assert(ptr[6] == 'R');
	assert(ptr[7] == 'T');
	/* version */
	assert(ptr[8] == 0x00);
	assert(ptr[9] == 0x00);
	assert(ptr[10] == 0x01);
	assert(ptr[11] == 0x00);
	/* header size */
	assert(ptr[12] == 0x5c);
	assert(ptr[13] == 0x00);
	assert(ptr[14] == 0x00);
	assert(ptr[15] == 0x00);
	/* TODO : header crc32 */
	/* reserved */
	assert(ptr[20] == 0x00);
	assert(ptr[21] == 0x00);
	assert(ptr[22] == 0x00);
	assert(ptr[23] == 0x00);
	/* current LBA */
	assert(ptr[24] == 0x01);
	assert(ptr[25] == 0x00);
	assert(ptr[26] == 0x00);
	assert(ptr[27] == 0x00);
	assert(ptr[28] == 0x00);
	assert(ptr[29] == 0x00);
	assert(ptr[30] == 0x00);
	assert(ptr[31] == 0x00);
	/* backup LBA (should be 0x4400) */
	assert(ptr[32] == 0x00);
	assert(ptr[33] == 0x44);
	assert(ptr[34] == 0x00);
	assert(ptr[35] == 0x00);
	assert(ptr[36] == 0x00);
	assert(ptr[37] == 0x00);
	assert(ptr[38] == 0x00);
	assert(ptr[39] == 0x00);
	/* TODO : the rest */
}

int main(void) {
	test_format();
	return EXIT_SUCCESS;
}
