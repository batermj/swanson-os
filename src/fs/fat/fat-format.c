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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "diskfile.h"

#include "filelib.h"

static int
create_zero_img(const char *imagepath,
                size_t imagesize) {

	FILE *image;
	unsigned char zerobuffer[512];
	size_t sector_size = 512;
	size_t sector_count;
	size_t write_count;
	size_t i;

	sector_count = (imagesize + sector_size) / sector_size;

	memset(zerobuffer, 0, sizeof(zerobuffer));

	image = fopen(imagepath, "wb");
	if (image == NULL) {
		return EXIT_FAILURE;
	}

	write_count = 0;

	while (write_count < (sector_count * sector_size)) {
		write_count += fwrite(zerobuffer, 1, sizeof(zerobuffer), image);
	}

	fclose(image);

	return EXIT_SUCCESS;
}

int
main(int argc, const char **argv) {

	const char *diskpath;
	enum fat_error error;
	struct fat_disk *disk;
	struct fat_diskfile diskfile;
	size_t disksize;
	size_t sector_size;
	int zero_img;

	if (argc >= 2)
		diskpath = argv[1];
	else
		diskpath = "fat.img";

	sector_size = 512;

	/* 256MiB */
	disksize = 256 * 1024 * 1024;

	zero_img = 0;

	if (zero_img && (create_zero_img(diskpath, disksize) != EXIT_SUCCESS)) {
		fprintf(stderr, "Failed to zero '%s'.\n", diskpath);
		return EXIT_FAILURE;
	}

	fat_diskfile_init(&diskfile);

	error = fat_diskfile_open(&diskfile, diskpath, "r+");
	if (error != FAT_ERROR_NONE) {
		fprintf(stderr, "Failed to open '%s': %s.\n", diskpath, fat_strerror(error));
		return EXIT_FAILURE;
	}

	disk = fat_diskfile_to_disk(&diskfile);

	struct fat_file *file;

	// Initialise File IO Library
	fl_init();

	// Attach media access functions to library
	if (fl_attach_media(disk) != FAT_INIT_OK) {
		printf("Failed to assign disk file to library.\n");
		return EXIT_FAILURE;
	}

	fl_format((disksize + sector_size) / sector_size, "Swanson FAT");

	fl_shutdown();

	fat_diskfile_done(&diskfile);

	return EXIT_SUCCESS;
}
