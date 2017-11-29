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

#include "debug.h"
#include "kernel.h"

struct opts {
	/** The number of bytes to allocate for memory. */
	unsigned long int memory;
};

static void opts_init(struct opts *opts) {
	/* 256 MiB */
	opts->memory = 256 * 1024 * 1024;
}

static int opts_parse(struct opts *opts,
                      int argc,
                      const char **argv) {

	int i;
	const char *memory = NULL;

	for (i = 1; i < argc; i++) {
		if ((strcmp(argv[i], "--memory") == 0)
		 || (strcmp(argv[i], "-m") == 0)) {
			memory = argv[i + 1];
			i++;
		} else {
			fprintf(stderr, "Unknown option '%s'\n", argv[i]);
			return EXIT_FAILURE;
		}
	}

	if (memory != NULL) {
		if (sscanf(memory, "%lu", &opts->memory) != 1) {
			fprintf(stderr, "Malformed memory amount '%s'\n", memory);
			return EXIT_FAILURE;
		}
		/* memory given in MiB, so it should
		 * be multiplied to get the number of bytes. */
		opts->memory *= 1024 * 1024;
	}

	return EXIT_SUCCESS;
}

int main(int argc, const char **argv) {

	struct opts opts;
	enum kernel_exitcode exitcode;
	struct kernel kernel;
	struct memmap_section primary_memmap_section;

	opts_init(&opts);

	opts_parse(&opts, argc, argv);

	primary_memmap_section.addr = malloc(opts.memory);
	primary_memmap_section.size = opts.memory;

	kernel_init(&kernel);

	kernel.memmap.unused_section_array = &primary_memmap_section;
	kernel.memmap.unused_section_count = 1;

	exitcode = kernel_main(&kernel);

	free(primary_memmap_section.addr);

	if (exitcode == KERNEL_SUCCESS)
		return EXIT_SUCCESS;
	else if (exitcode == KERNEL_FAILURE)
		return EXIT_FAILURE;
	else
		return EXIT_FAILURE;
}
