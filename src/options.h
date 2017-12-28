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

/** @file */

#ifndef SWANSON_OPTIONS_H
#define SWANSON_OPTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

struct fdisk;

/** Runtime options for the kernel.
 * */

struct options {
	/** The number of bytes that the kernel may use. */
	unsigned long int memory;
	/** An array of files that may be used as disks. */
	struct fdisk *disk_array;
	/** The number of disks in the disk array. */
	unsigned long int disk_count;
};

/** Initializes an options structure.
 * @param options An uninitialized options structure.
 * */

void
options_init(struct options *options);

/** Releases resources allocated by the options
 * structure.
 * */

void
options_free(struct options *options);

/** Adds a disk file to the options.
 * @param options An initialized options structure.
 * @param disk_path The path to the disk file.
 * @returns Zero on success, a negative number on failure.
 * */

int
options_add_disk(struct options *options,
                 const char *disk_path);

/** Get the amount of memory that will be used
 * for the kernel.
 * @param options An initialized options structure.
 * @returns The amount of memory to use for kernel.
 * */

unsigned long int
options_get_memory(const struct options *options);

/** Gets a disk from the disk array in
 * the options structure.
 * @param options An initialized options
 * structure.
 * @param index The index of the disk
 * within the disk array.
 * @returns A pointer to the disk, from
 * the disk array. If the index is out of
 * bounds, this is NULL.
 * @see options_add_disk
 * @see options_get_disk_count
 * */

struct disk *
options_get_disk(struct options *options,
                 unsigned long int index);

/** Returns the number of disks in the disk array.
 * @param options An initialized options structure.
 * @returns The number of disks within the disk array.
 * @see options_get_disk
 * @see options_add_disk
 * */

unsigned long int
options_get_disk_count(const struct options *options);

/** Parses options from the command line.
 * @param options An initialized options structure.
 * @param argc The number of arguments in the
 * argument array.
 * @param argv The array of command line arguments.
 * @returns Zero on success, non-zero on failure.
 * */

int
options_parse_args(struct options *options,
                   int argc, const char **argv);

/** Parses an amount of memory in human-readable
 * format.
 * @param options An initialized options structure.
 * @param memory The string containing the human-readable
 * memory specification.
 * @returns Zero on success, non-zero on failure.
 * */

int
options_parse_memory(struct options *options,
                     const char *memory);

/** Sets the amount of memory to use by the kernel.
 * @param options An initialized options structure.
 * @param memory The amount of memory to assign the kernel.
 * This parameter must be greater than zero.
 * @returns Zero on success, non-zero on failure.
 * */

int
options_set_memory(struct options *options,
                   unsigned long int memory);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_OPTIONS_H */
