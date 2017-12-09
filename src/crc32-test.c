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

#include "crc32-test.h"

#include "assert.h"
#include "crc32.h"

void
crc32_test(void) {
	crc32_test_main();
}

void
crc32_test_main(void) {

	uint32_t checksum;
	const char sentence[] = "The quick brown fox jumps over the lazy dog.\n";

	checksum = crc32(sentence, sizeof(sentence) - 1);
	assert(checksum == 0xeb50cc6a);
}
