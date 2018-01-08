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

#ifndef SWANSON_FS_RAMFS_DIR_H
#define SWANSON_FS_RAMFS_DIR_H

#ifdef __cplusplus
extern "C" {
#endif

struct stream;

struct ramfs_dir {
	/** The number of bytes occupied
	 * by the directory name. */
	unsigned long int name_size;
	/** The number of files in the
	 * directory. */
	unsigned long int file_count;
	/** The number of sub directories
	 * in the directory. */
	unsigned long int subdir_count;
	/** The name of the directory. */
	char *name;
	/** The file array. */
	struct ramfs_file *file_array;
	/** The subdirectory array. */
	struct ramfs_dir *subdir_array;
};

/** Initializes the directory.
 * @param dir An uninitialized directory
 * structure.
 * */

void
ramfs_dir_init(struct ramfs_dir *dir);

/** Releases memory allocated by a ramfs directory.
 * @param dir An initialized ramfs directory.
 * */

void
ramfs_dir_free(struct ramfs_dir *dir);

/** Adds a file to the directory.
 * @param dir An initialized directory structure.
 * @param name The name of the file to create.
 * @returns Zero on success, non-zero on failure.
 * */

int
ramfs_dir_add_file(struct ramfs_dir *dir,
                   const char *name);

/** Adds a subdirectory to the directory.
 * @param dir An initialized directory structure.
 * @param name The name of the subdirectory to create.
 * @returns Zero on success, non-zero on failure.
 * */

int
ramfs_dir_add_subdir(struct ramfs_dir *dir,
                     const char *name);

/** Checks whether or not a name exists
 * within a directory, as either a file
 * or a directory.
 * @param dir An initialized directory structure.
 * @param name The name to check for.
 * @returns Zero if the name is not in the directory,
 * one if it is.
 * */

int
ramfs_dir_name_exists(const struct ramfs_dir *dir,
                      const char *name);

/** Deserializes a directory structure.
 * @param dir An initialized directory structure.
 * @param stream The stream to read the directory from.
 * @returns The number of bytes read from the directory.
 * */

unsigned long int
ramfs_dir_decode(struct ramfs_dir *dir,
                 struct stream *stream);

/** Exports a ramfs directory to a stream.
 * @param dir An initialized directory structure.
 * @param stream The stream to export the directory to.
 * @returns The number of bytes written to the stream.
 * */

unsigned long int
ramfs_dir_encode(const struct ramfs_dir *dir,
                 struct stream *stream);

/** Sets the name of the directory.
 * @param dir An initialized directory structure.
 * @param name The new name of the directory.
 * @returns Zero on success, non-zero on failure.
 * */

int
ramfs_dir_set_name(struct ramfs_dir *dir,
                   const char *name);

struct ramfs_file *
ramfs_dir_get_file(struct ramfs_file *file,
                   const char *file_name);

const char *
ramfs_dir_get_name(struct ramfs_dir *dir);

struct ramfs_dir *
ramfs_dir_get_subdir(struct ramfs_dir *dir,
                     const char *subdir_name);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_FS_RAMFS_DIR_H */
