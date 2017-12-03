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

#include "ext4.h"
#include "fdisk.h"

static int format(const char *image_path, int argc, const char **argv) {

	struct fdisk image;

	fdisk_init(&image);

	fdisk_open(&image, image_path, "w+");

	fdisk_close(&image);

	(void) argc;
	(void) argv;

	return EXIT_SUCCESS;
}

static void help(const char *argv0) {
	printf("Usage: %s [options] <command>\n", argv0);
	printf("\n");
	printf("Options:\n");
	printf("\t--image-path PATH : Path to the ext4 image.\n");
	printf("\n");
	printf("Commands:\n");
	printf("\tformat : Format an ext4 image.\n");
	printf("\thelp   : Print this help message and exit.\n");
}

int main(int argc, const char **argv) {

	int i;
	const char *image_path;

	image_path = "swanson-root.img";

	for (i = 1; i < argc; i++) {
		if (argv[i][0] != '-') {
			break;
		} else if (strcmp(argv[i], "--image-path") == 0) {
			image_path = argv[i + 1];
			i++;
		} else {
			fprintf(stderr, "Unknown option '%s'.\n", argv[i]);
			return EXIT_FAILURE;
		}
	}

	if (image_path == NULL) {
		fprintf(stderr, "Image path not specified.\n");
		return EXIT_FAILURE;
	}

	if (i >= argc) {
		help(argv[0]);
		return EXIT_FAILURE;
	}

	/* Parse command. */
	if (strcmp(argv[i], "format") == 0) {
		return format(image_path, argc - (i + 1), &argv[i + 1]);
	} else if (strcmp(argv[i], "help") == 0) {
		help(argv[0]);
	} else if (strcmp(argv[i], "cp") == 0) {

	} else if (strcmp(argv[i], "cat") == 0) {

	} else if (strcmp(argv[i], "mkdir") == 0) {

	} else if (strcmp(argv[i], "xxd") == 0) {

	} else if (strcmp(argv[i], "info") == 0) {

	} else {
		fprintf(stderr, "Unknown command '%s'.\n", argv[i]);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
