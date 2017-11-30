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

#include "fdisk.h"
#include "gpt.h"

int main(int argc, const char **argv) {

	int i = 1;
	int err = 0;
	unsigned long int size = 1 * 1024 * 1024;
	struct fdisk image;
	const char *image_path = NULL;
	char zero_buffer[512];
	size_t write_count;

	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--mebibytes") == 0) {
			if (sscanf(argv[i + 1], "%lu", &size) != 1) {
				fprintf(stderr, "Malformed image size '%s'\n", argv[i + 1]);
				return EXIT_FAILURE;
			}
			size *= 1024 * 1024;
		} else if (strcmp(argv[i], "--image-path") == 0) {
			image_path = argv[i + 1];
			i++;
		} else if (strcmp(argv[i], "--help") == 0) {
			printf("Usage: %s [options]\n", argv[0]);
			printf("\n");
			printf("Options:\n");
			printf("\t--image-path PATH : Specify path of image.\n");
			printf("\t--mebibytes  N    : Specify the size of the image (in mebibytes).\n");
			printf("\t--help            : Print help and exit.\n");
		} else {
			fprintf(stderr, "Unknown option '%s' (see --help)\n", argv[i]);
			return EXIT_FAILURE;
		}
	}

	if (image_path == NULL)
		image_path = "swanson.img";

	fdisk_init(&image);

	err = fdisk_open(&image, image_path, "wb");
	if (err != 0) {
		fprintf(stderr, "Failed to open image '%s' for writing.\n", image_path);
		fdisk_close(&image);
		return EXIT_FAILURE;
	}

	/* zero the image contents */

	memset(zero_buffer, 0, sizeof(zero_buffer));
	write_count = 0;
	while ((write_count < size)) {
		write_count += image.base.stream.write(&image, zero_buffer, sizeof(zero_buffer));
	}

	/* format the disk with a GUID partition table */

	err = gpt_format(&image.base.stream);
	if (err != GPT_ERROR_NONE) {
		fprintf(stderr, "Failed to format partition table: %s\n", gpt_strerror(err));
		fdisk_close(&image);
		return EXIT_FAILURE;
	}

	fdisk_close(&image);

	return EXIT_SUCCESS;
}

