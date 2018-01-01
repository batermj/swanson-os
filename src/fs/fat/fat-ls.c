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
list_directory(const char *path) {

	fl_listdirectory(path);

	return EXIT_SUCCESS;
}

int
main(int argc, const char **argv) {

	int i;
	int exitcode;
	const char *diskpath;
	const char *path;
	struct fat_disk *disk;
	struct fat_diskfile diskfile;

	diskpath = "fat.img";

	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--disk") == 0) {
			diskpath = argv[i + 1];
			i++;
		} else if (argv[i][0] != '-') {
			break;
		} else {
			fprintf(stderr, "Unknown option '%s'.\n", argv[i]);
			return EXIT_FAILURE;
		}
	}

	if (diskpath == NULL) {
		fprintf(stderr, "No disk file specified.\n");
		return EXIT_FAILURE;
	}

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

	path = NULL;

	while (i < argc) {
		if (argv[i][0] != '-') {
			if (path != NULL)
				fprintf(stderr, "Only one path is allowed.\n");
			else
				path = argv[i];
		} else {
			fprintf(stderr, "Unknown option '%s'.\n", argv[i]);
			return EXIT_FAILURE;
		}
		i++;
	}

	if (path == NULL)
		path = "/";

	exitcode = list_directory(path);

	fl_shutdown();

	fat_diskfile_done(&diskfile);

	return exitcode;
}
