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

struct stream;

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

/** Decodes a file from a serialized stream.
 * @param file An initialized file structure.
 * @param stream The stream to read the file from.
 * @returns The number of bytes read from the memory block.
 * */

unsigned long int
ramfs_file_decode(struct ramfs_file *file,
                  struct stream *stream);

/** Encodes a ramfs file to a stream.
 * @param file An initialized ramfs file.
 * @param stream The stream to encode the file to.
 * @returns The number of bytes written to the stream.
 * */

unsigned long int
ramfs_file_encode(const struct ramfs_file *file,
                  struct stream *stream);

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

/** Set the name of the file.
 * @param file An initialized file structure.
 * @param name A null-terminated file name.
 * @returns Zero on success, non-zero on failure.
 * */

int
ramfs_file_set_name(struct ramfs_file *file,
                    const char *name);

/** Set the file data.
 * @param file An initialized file structure
 * @param data The new file data.
 * @param data_size The number of bytes occupied
 * by the new file data.
 * @returns Zero on success, a negative number on failure.
 * */

int
ramfs_file_set_data(struct ramfs_file *file,
                    const void *data,
                    unsigned long int data_size);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_FS_RAMFS_FILE_H */
