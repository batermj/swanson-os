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

#include "guid.h"

#include "stream.h"

void guid_init(struct guid *guid) {
	guid->buffer[0] = 0;
	guid->buffer[1] = 0;
	guid->buffer[2] = 0;
	guid->buffer[3] = 0;
	guid->buffer[4] = 0;
	guid->buffer[5] = 0;
	guid->buffer[6] = 0;
	guid->buffer[7] = 0;
}

int guid_cmp(const struct guid *a,
             const struct guid *b) {
	unsigned int i;
	for (i = 0; i < GUID_SIZE; i++) {
		if (a->buffer[i] < b->buffer[i])
			return -1;
		else if (a->buffer[i] > b->buffer[i])
			return 1;
	}
	return 0;
}

void guid_copy(struct guid *dst,
               const struct guid *src) {
	unsigned int i;
	for (i = 0; i < GUID_SIZE; i++)
		dst->buffer[i] = src->buffer[i];
}

uint64_t guid_read(struct stream *stream,
                   struct guid *guid) {
	return stream_read(stream, guid->buffer, GUID_SIZE);
}

uint64_t guid_write(struct stream *stream,
                    const struct guid *guid) {
	return stream_write(stream, guid->buffer, GUID_SIZE);
}
