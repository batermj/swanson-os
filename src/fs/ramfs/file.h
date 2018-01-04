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

#ifndef SWANSON_FS_RAMFS_FILE_H
#define SWANSON_FS_RAMFS_FILE_H

#ifdef __cplusplus
extern "C" {
#endif

/** A ramfs file.
 * */

struct ramfs_file {
	/** The number of bytes occupied by the file name. */
	unsigned long int name_size;
	/** The number of bytes occupied by the file data. */
	unsigned long int data_size;
	/** The file name. */
	char *name;
	/** The file data. */
	void *data;
};

/** Initializes a ramfs file.
 * @param file An uninitialized file structure.
 * */

void
ramfs_file_init(struct ramfs_file *file);

/** Frees a ramfs file.
 * @param file An initialized file structure.
 * */

void
ramfs_file_free(struct ramfs_file *file);

/** Imports a file from a serialized
 * buffer.
 * @param file An initialized file structure.
 * @param data The memory containing the serialized file.
 * @param data_size The size of the memory block containing
 * the serialized file.
 * @returns The number of bytes read from the memory block.
 * */

unsigned long int
ramfs_file_import(struct ramfs_file *file,
                  const void *data,
                  unsigned long int data_size);

/** Returns the name of the file.
 * @param file An initialized file structure.
 * @returns The name of the file.
 * */

const char *
ramfs_file_get_name(const struct ramfs_file *file);

/** Returns the size of the file.
 * @param file An initialized file structure.
 * @returns The number of bytes occupied by the
 * file name.
 * */

unsigned long int
ramfs_file_get_name_size(const struct ramfs_file *file);

/** Returns the data associated with the file.
 * @param file An initialized file structure.
 * @returns A pointer to the data associated
 * with the file data.
 * */

void *
ramfs_file_get_data(struct ramfs_file *file);

/** Returns the number of bytes associated with
 * the file data.
 * @param file An initialized file structure.
 * @returns The number of bytes occupied by
 * the file data.
 * */

unsigned long int
ramfs_file_get_size(const struct ramfs_file *file);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_FS_RAMFS_FILE_H */
