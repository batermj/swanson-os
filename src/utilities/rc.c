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

struct plan {
	FILE *in;
	FILE *header;
	FILE *source;
	const char *name;
};

static void
write_header(struct plan *plan) {

	FILE *header;

	header = plan->header;

	fprintf(header, "\n");
}

static void
write_source(struct plan *plan) {

	FILE *source;

	source = plan->source;

	fprintf(source, "\n");
}

static void
execute_plan(struct plan *plan) {
	write_header(plan);
	write_source(plan);
}

int
main(int argc, const char **argv) {

	int i;
	int err;
	FILE *source;
	FILE *header;
	FILE *input;
	const char *field_name;
	const char *source_path;
	const char *header_path;
	const char *input_path;
	struct plan plan;

	input = NULL;
	source = NULL;
	header = NULL;
	input_path = NULL;
	source_path = NULL;
	header_path = NULL;
	field_name = NULL;

	for (i = 1; i < argc; i += 2) {
		if (strcmp(argv[i], "--header") == 0) {
			header_path = argv[i + 1];
		} else if (strcmp(argv[i], "--source") == 0) {
			source_path = argv[i + 1];
		} else if (strcmp(argv[i], "--input") == 0) {
			input_path = argv[i + 1];
		} else if (strcmp(argv[i], "--name") == 0) {
			field_name = argv[i + 1];
		} else if (strcmp(argv[i], "--help") == 0) {
			printf("Usage: %s [Options]\n", argv[0]);
			printf("\n");
			printf("Options:\n");
			printf("\t--header PATH : The header that contains the forward declarations of the variable.\n");
			printf("\t--source PATH : The source file to put the data in.\n");
			printf("\t--input PATH  : The file containing the data to put in source code.\n");
			printf("\t--name PATH   : The name of the variable containing the data.\n");
			printf("\t--help        : Print this help message.\n");
			return EXIT_FAILURE;
		} else {
			fprintf(stderr, "Unknown option '%s'.\n", argv[i]);
			return EXIT_FAILURE;
		}
	}

	if (header_path == NULL) {
		fprintf(stderr, "Must specify a header path with '--header PATH'.\n");
		return EXIT_FAILURE;
	}

	if (source_path == NULL) {
		fprintf(stderr, "Must specify source file with '--source PATH'.\n");
		return EXIT_FAILURE;
	}

	if (input_path == NULL) {
		fprintf(stderr, "Must specify input file with '--input PATH'.\n");
		return EXIT_FAILURE;
	}

	if (field_name == NULL) {
		fprintf(stderr, "Must specify variable name with '--name NAME'.\n");
		return EXIT_FAILURE;
	}

	source = fopen(source_path, "wb");
	if (source == NULL) {
		fprintf(stderr, "Failed to open source file '%s'.\n", source_path);
		return EXIT_FAILURE;
	}

	header = fopen(header_path, "wb");
	if (header == NULL) {
		fprintf(stderr, "Failed to open header file '%s'.\n", header_path);
		fclose(source);
		return EXIT_FAILURE;
	}

	input = fopen(input_path, "rb");
	if (input == NULL) {
		fprintf(stderr, "Failed to open input file '%s'.\n", input_path);
		fclose(header);
		fclose(source);
		return EXIT_FAILURE;
	}

	plan.in = input;
	plan.source = source;
	plan.header = header;
	plan.name = field_name;

	execute_plan(&plan);

	if (input != NULL)
		fclose(input);

	if (header != NULL)
		fclose(header);

	if (source != NULL)
		fclose(source);

	if (err != EXIT_SUCCESS)
		return EXIT_FAILURE;
	else
		return EXIT_SUCCESS;
}
