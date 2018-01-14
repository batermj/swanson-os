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

#include "fs/ext4.h"
#include "fdisk.h"

/** An ext4 accessor that prints
 * information about the file system.
 * */

struct ext4_printer {
	/** Base accessor structure. */
	struct ext4_accessor accessor;
	/** The output file that the information
	 * will be printed to. */
	FILE *output;
};

static int
ext4_printer_superblock(void *printer_ptr,
                        const struct ext4_superblock *superblock) {

	struct ext4_printer *printer;

	printer = (struct ext4_printer *) printer_ptr;

	fprintf(printer->output, "superblock:\n");
	fprintf(printer->output, "\tinode count: %lu\n", (unsigned long int) superblock->inode_count);
	fprintf(printer->output, "\tmount count: %lu\n", (unsigned long int) superblock->mount_count);

	return 0;
}

/** Initializes the ext4 printer.
 * @param printer An uninitialized ext4 printer.
 * */

static void
ext4_printer_init(struct ext4_printer *printer) {
	ext4_accessor_init(&printer->accessor);
	printer->accessor.data = printer;
	printer->accessor.superblock = ext4_printer_superblock;
	printer->output = stdout;
}

static int format(const char *image_path, int argc, const char **argv) {

	int err;
	struct fdisk image;
	char zero_buffer[512];
	unsigned int i;

	fdisk_init(&image);

	err = fdisk_open(&image, image_path, "w+");
	if (err != 0) {
		fprintf(stderr, "Failed to open '%s' for writing.\n", image_path);
		return EXIT_FAILURE;
	}

	/* zero the disk contents */

	memset(zero_buffer, 0, sizeof(zero_buffer));

	for (i = 0; i < 64; i++) {
		stream_write(&image.base.stream, zero_buffer, sizeof(zero_buffer));
	}

	/* format that disk */

	err = ext4_format(&image.base.stream);
	if (err != EXT4_ERROR_NONE) {
		/* TODO error description should go here. */
		fprintf(stderr, "Failed to format image.\n");
		fdisk_close(&image);
		return EXIT_FAILURE;
	}

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

static int info(const char *image_path) {

	int err;
	struct fdisk image;
	struct ext4_printer printer;

	fdisk_init(&image);

	err = fdisk_open(&image, image_path, "r+");
	if (err != 0) {
		fprintf(stderr, "Failed to open ext4 image '%s'\n", image_path);
		return EXIT_FAILURE;
	}

	ext4_printer_init(&printer);

	err = ext4_access(&printer.accessor, &image.base.stream);
	if (err != 0) {
		fdisk_close(&image);
		return EXIT_FAILURE;
	}

	fdisk_close(&image);

	return EXIT_SUCCESS;
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
		return info(image_path);
	} else {
		fprintf(stderr, "Unknown command '%s'.\n", argv[i]);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
