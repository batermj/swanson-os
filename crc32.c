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

#include "crc32.h"

uint32_t crc32(const void *buf, uint64_t buf_size) {

	uint64_t i;
	uint64_t j;
	uint32_t byte;
	uint32_t crc;
	uint32_t mask;
	const uint8_t *buf8;

	crc = 0xFFFFFFFF;

	buf8 = (const uint8_t *) buf;

	for (i = 0; i < buf_size; i++) {
		byte = buf8[i];
		crc = crc ^ byte;
		for (j = 0; j < 8; j++) {
			mask = -(crc & 1);
			crc = (crc >> 1) ^ (0xEDB88320 & mask);
		}
	}

	return ~crc;
}
