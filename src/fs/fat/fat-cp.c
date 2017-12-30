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
copyfile(const char *source_path,
         const char *destination_path) {

	FILE *source;
	struct fat_file *destination;
	char *buffer;
	size_t buffer_size;
	size_t read_size;

	source = fopen(source_path, "rb");
	if (source == NULL) {
		return EXIT_FAILURE;
	}

	destination = fl_fopen(destination_path, "wb");
	if (destination == NULL) {
		fclose(source);
		return EXIT_FAILURE;
	}

	buffer_size = 512;

	buffer = malloc(buffer_size);
	if (buffer == NULL) {
		fclose(source);
		fl_fclose(destination);
		return EXIT_FAILURE;
	}

	while (!feof(source)) {

		read_size = fread(buffer, 1, buffer_size, source);

		fl_fwrite(buffer, 1, read_size, destination);
	}

	free(buffer);

	fclose(source);

	fl_fclose(destination);

	return EXIT_SUCCESS;
}

int
main(int argc, const char **argv) {

	int i;
	int exitcode;
	const char *diskpath;
	const char *source;
	const char *destination;
	struct fat_disk *disk;
	struct fat_diskfile diskfile;

	if (argc >= 2)
		diskpath = argv[1];
	else
		diskpath = "fat.img";

	fat_diskfile_init(&diskfile);

	exitcode = fat_diskfile_open(&diskfile, diskpath, "r+");
	if (exitcode != FAT_ERROR_NONE) {
		fprintf(stderr, "Failed to open '%s': %s.\n", diskpath, fat_strerror(exitcode));
		return EXIT_FAILURE;
	}

	disk = fat_diskfile_to_disk(&diskfile);

	struct fat_file *file;

	// Initialise File IO Library
	fl_init();

	// Attach media access functions to library
	fl_attach_media(disk);

	source = NULL;
	destination = NULL;

	for (i = 1; i < argc; i++) {
		if (argv[i][0] != '-') {
			if (source == NULL)
				source = argv[i];
			else if (destination == NULL)
				destination = argv[i];
			else {
				fprintf(stderr, "Trailing file '%s'.\n", argv[i]);
				return EXIT_FAILURE;
			}
		} else {
			fprintf(stderr, "Unknown option '%s'.\n", argv[i]);
			return EXIT_FAILURE;
		}
	}

	if (source == NULL) {
		fprintf(stderr, "Source file not specified.\n");
		return EXIT_FAILURE;
	} else if (destination == NULL) {
		fprintf(stderr, "Destination file not specified.\n");
		return EXIT_FAILURE;
	}

	exitcode = copyfile(source, destination);

	fl_shutdown();

	fat_diskfile_done(&diskfile);

	return exitcode;
}
