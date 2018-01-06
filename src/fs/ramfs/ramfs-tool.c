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

#include "fs.h"
#include "file.h"

#include "fdisk.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
help(const char *argv0) {
	printf("Usage: %s [options] <command>\n", argv0);
	printf("\n");
	printf("Options:\n");
	printf("\t--disk PATH : The path of the file containing the ramfs.\n");
	printf("\t--help      : Print this help message.\n");
	printf("\n");
	printf("Commands:\n");
	printf("\tcat\n");
	printf("\tcp\n");
	printf("\tformat\n");
	printf("\tmkdir\n");
	printf("\tls\n");
}

static int
ramfs_cat(struct ramfs *fs,
          const char *path) {
	(void) fs;
	(void) path;
	return EXIT_SUCCESS;
}

#if 0

static int
ramfs_cp(struct ramfs *fs,
         const char *source_path,
         const char *destination_path);

#endif

static int
ramfs_ls(struct ramfs *fs,
         const char *path) {

	unsigned int i;
	struct ramfs_dir *dir;

	dir = ramfs_open_dir(fs, path);
	if (dir == NULL) {
		return EXIT_FAILURE;
	}

	for (i = 0; i < dir->file_count; i++)
		printf("file      : %s\n", dir->file_array[i].name);

	for (i = 0; i < dir->subdir_count; i++)
		printf("directory : %s\n", dir->subdir_array[i].name);

	return EXIT_SUCCESS;
}

static int
ramfs_load(struct ramfs *fs,
           const char *path) {

	int err;
	struct stream *stream;
	struct disk *disk;
	struct fdisk fdisk;

	fdisk_init(&fdisk);

	err = fdisk_open(&fdisk, path, "rb");
	if (err != 0) {
		fprintf(stderr, "Failed to open disk '%s'.\n", path);
		fdisk_close(&fdisk);
		return EXIT_FAILURE;
	}

	disk = fdisk_to_disk(&fdisk);

	stream = disk_to_stream(disk);

	ramfs_decode(fs, stream);

	fdisk_close(&fdisk);

	return EXIT_SUCCESS;
}

static int
ramfs_unload(struct ramfs *fs,
             const char *diskpath) {

	int err;
	struct stream *stream;
	struct disk *disk;
	struct fdisk fdisk;

	fdisk_init(&fdisk);

	err = fdisk_open(&fdisk, diskpath, "wb");
	if (err != 0) {
		fprintf(stderr, "Failed to open disk '%s'.\n", diskpath);
		fdisk_close(&fdisk);
		return EXIT_FAILURE;
	}

	disk = fdisk_to_disk(&fdisk);

	stream = disk_to_stream(disk);

	ramfs_encode(fs, stream);

	fdisk_close(&fdisk);

	return EXIT_SUCCESS;
}

int
main(int argc, const char **argv) {

	int i;
	int err;
	int create_flag;
	const char *diskpath;
	struct ramfs ramfs;

	create_flag = 0;
	diskpath = NULL;

	ramfs_init(&ramfs);

	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--disk") == 0) {
			diskpath = argv[++i];
		} else if (strcmp(argv[i], "--help") == 0) {
			help(argv[0]);
			return EXIT_FAILURE;
		} else if ((strcmp(argv[i], "--create") == 0) || (strcmp(argv[i], "-C") == 0)) {
			create_flag = 1;
		} else if (argv[i][0] != '-') {
			break;
		} else {
			fprintf(stderr, "Unknown option '%s'.\n", argv[i]);
			return EXIT_FAILURE;
		}
	}

	if (i >= argc) {
		fprintf(stderr, "No command specified, see --help.\n");
		return EXIT_FAILURE;
	}

	if (diskpath == NULL)
		diskpath = "ramfs.img";

	if (!create_flag) {
		err = ramfs_load(&ramfs, diskpath);
		if (err != 0) {
			fprintf(stderr, "Failed to load ramfs '%s'.\n", diskpath);
			ramfs_free(&ramfs);
			return EXIT_FAILURE;
		}
	}

	if (strcmp(argv[i], "cat") == 0) {
		i++;
		while (i < argc) {
			err = ramfs_cat(&ramfs, argv[i]);
			if (err != 0) {
				ramfs_free(&ramfs);
				return EXIT_FAILURE;
			}
			i++;
		}
	} else if (strcmp(argv[i], "cp") == 0) {

	} else if (strcmp(argv[i], "mkdir") == 0) {
		i++;
		while (i < argc) {
			err = ramfs_mkdir(&ramfs, argv[i]);
			if (err != 0) {
				fprintf(stderr, "Failed to create directory '%s'.\n", argv[i]);
				ramfs_free(&ramfs);
				return EXIT_FAILURE;
			}
			i++;
		}
	} else if (strcmp(argv[i], "ls") == 0) {
		i++;
		while (i < argc) {
			err = ramfs_ls(&ramfs, argv[i]);
			if (err != EXIT_SUCCESS) {
				ramfs_free(&ramfs);
				return EXIT_FAILURE;
			}
			i++;
		}
	} else if (strcmp(argv[i], "format") == 0) {
		/* not currently anything to do here */
	} else {
		fprintf(stderr, "Unknown command '%s'.\n", argv[i]);
		ramfs_free(&ramfs);
		return EXIT_FAILURE;
	}

	err = ramfs_unload(&ramfs, diskpath);
	if (err != EXIT_SUCCESS) {
		ramfs_free(&ramfs);
		return EXIT_FAILURE;
	}

	ramfs_free(&ramfs);

	return EXIT_SUCCESS;
}
