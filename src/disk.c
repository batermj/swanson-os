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

#include "disk.h"

void
disk_init(struct disk *disk) {
	stream_init(&disk->stream);
	disk->sector_size = 512;
}

void
disk_set_sector_size(struct disk *disk,
                     uint64_t sector_size) {
	if (sector_size > 0)
		disk->sector_size = sector_size;
}

uint64_t
disk_read(struct disk *disk,
          void *buf,
          uint64_t sector_count) {

	uint64_t read_count;
	uint64_t buf_size;

	buf_size = sector_count * disk->sector_size;

	read_count = stream_read(&disk->stream, buf, buf_size);

	read_count /= disk->sector_size;

	return 0;
}

int
disk_seek(struct disk *disk,
          uint64_t sector_index) {

	uint64_t offset;

	offset = sector_index * disk->sector_size;

	return stream_setpos(&disk->stream, offset);
}
