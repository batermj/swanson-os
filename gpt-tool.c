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

#ifndef MiB
#define MiB (1 * 1024 * 1024)
#endif

/** Used for printing the
 * contents of a GUID partition table.
 * */

struct gpt_printer {
	/** Accessor class that contains the
	 * callback functions. */
	struct gpt_accessor accessor;
	/** The file to print the information
	 * to. */
	FILE *output;
};

static int gpt_printer_header(void *printer_ptr, const struct gpt_header *header) {

	struct gpt_printer *printer;

	printer = (struct gpt_printer *) printer_ptr;

	fprintf(printer->output, "GPT Header:\n");
	fprintf(printer->output, "\tVersion: %08x\n", header->version);
	fprintf(printer->output, "\tHeader Size: %08u\n", header->header_size);
	fprintf(printer->output, "\tHeader CRC32: %08x\n", header->header_crc32);

	return 0;
}

static void gpt_printer_init(struct gpt_printer *printer) {
	gpt_accessor_init(&printer->accessor);
	printer->accessor.data = printer;
	printer->accessor.header = gpt_printer_header;
	printer->output = stdout;
}

static int add_partition(struct fdisk *image, int argc, const char **argv) {

	enum gpt_error err;
	uint32_t partition_index;

	(void) argc;
	(void) argv;

	/* add the partition for the file system. */

	err = gpt_add_partition(&image->base.stream, &partition_index);
	if (err != GPT_ERROR_NONE) {
		fprintf(stderr, "Failed to add partition: %s\n", gpt_strerror(err));
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

static int format(struct fdisk *image, int argc, const char **argv) {

	enum gpt_error err;
	char zero_buffer[512];
	uint64_t write_count;
	uint64_t size = 1 * 1024 * 1024;

	(void) argc;
	(void) argv;

	if (size < GPT_MINIMUM_DISK_SIZE) {
		fprintf(stderr, "Disk must be at least %u MiB.\n", (MiB + GPT_MINIMUM_DISK_SIZE) / MiB);
		return EXIT_FAILURE;
	}

	/* zero the image contents */

	memset(zero_buffer, 0, sizeof(zero_buffer));
	write_count = 0;
	while ((write_count < size)) {
		write_count += stream_write(&image->base.stream, zero_buffer, sizeof(zero_buffer));
	}

	/* format the disk with a GUID partition table */

	err = gpt_format(&image->base.stream);
	if (err != GPT_ERROR_NONE) {
		fprintf(stderr, "Failed to format partition table: %s\n", gpt_strerror(err));
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

static int print(struct fdisk *image, int argc, const char **argv) {

	int err;
	struct gpt_printer gpt_printer;

	(void) argc;
	(void) argv;

	gpt_printer_init(&gpt_printer);

	err = gpt_access(&image->base.stream, &gpt_printer.accessor);
	if (err != 0)
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}

static void help(const char *argv0) {
	printf("Usage: %s [options] <command>\n", argv0);
	printf("\n");
	printf("Commands:\n");
	printf("\tadd-partition : Add a new partition.\n");
	printf("\tformat        : Format an image file.\n");
	printf("\thelp          : Print this help message.\n");
	printf("\tprint         : Print GPT contents.\n");
	printf("\n");
	printf("Options:\n");
	printf("\t--image-path PATH : Path of the image.\n");
}

int main(int argc, const char **argv) {

	int i = 1;
	int err = 0;
	struct fdisk image;
	const char *image_path = "swanson.img";

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

	if (argc <= i) {
		help(argv[0]);
		return EXIT_FAILURE;
	}

	fdisk_init(&image);

	err = fdisk_open(&image, image_path, "r+");
	if (err != 0) {
		fprintf(stderr, "Failed to open image file '%s'\n", image_path);
		return EXIT_FAILURE;
	}

	for (; i < argc; i++) {
		if (strcmp(argv[i], "add-partition") == 0) {
			err = add_partition(&image, argc - 1, &argv[i]);
		} else if (strcmp(argv[i], "format") == 0) {
			err = format(&image, argc - i, &argv[i]);
		} else if (strcmp(argv[i], "print") == 0) {
			err = print(&image, argc - i, &argv[i]);
		} else if (strcmp(argv[i], "help") == 0) {
			help(argv[0]);
			return EXIT_FAILURE;
		} else {
			fprintf(stderr, "Unknown command '%s', see '%s help'.\n", argv[i], argv[0]);
			return EXIT_FAILURE;
		}
	}

	fdisk_close(&image);

	return err;
}

