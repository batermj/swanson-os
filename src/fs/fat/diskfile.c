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

#include "diskfile.h"

#ifndef NULL
#define NULL ((void *) 0x00)
#endif

static int
diskfile_read(void *data,
              uint32_t sector_index,
              void *buf,
              uint32_t sector_count) {

	struct fat32_diskfile *diskfile;

	diskfile = (struct fat32_diskfile *) data;

	if (diskfile->file == NULL)
		return 0;

	fseek(diskfile->file, sector_index * 512, SEEK_SET);

	fread(buf, 1, sector_count * 512, diskfile->file);

	return 1;
}

static int
diskfile_write(void *data,
               uint32_t sector_index,
               const void *buf,
               uint32_t sector_count) {

	struct fat32_diskfile *diskfile;

	diskfile = (struct fat32_diskfile *) data;

	if (diskfile->file == NULL)
		return 0;

	fseek(diskfile->file, sector_index * 512, SEEK_SET);

	fwrite(buf, 1, sector_count * 512, diskfile->file);

	return 1;
}

void
fat32_diskfile_init(struct fat32_diskfile *diskfile) {
	fat32_disk_init(&diskfile->disk);
	diskfile->disk.data = diskfile;
	diskfile->disk.read = diskfile_read;
	diskfile->disk.write = diskfile_write;
	diskfile->file = NULL;
}

void
fat32_diskfile_done(struct fat32_diskfile *diskfile) {
	if (diskfile->file != NULL) {
		fclose(diskfile->file);
		diskfile->file = NULL;
	}
}

enum fat32_error
fat32_diskfile_open(struct fat32_diskfile *diskfile,
                    const char *path,
                    const char *mode) {

	diskfile->file = fopen(path, mode);
	if (diskfile->file == NULL)
		return FAT32_ERROR_INVALID_PATH;

	return FAT32_ERROR_NONE;
}

struct fat32_disk *
fat32_diskfile_to_disk(struct fat32_diskfile *diskfile) {
	return &diskfile->disk;
}

