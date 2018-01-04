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

/** Deserializes a directory structure.
 * @param dir An initialized directory structure.
 * @param data The data to read the directory from.
 * @param data_size The size of the data containing
 * the directory.
 * @returns The number of bytes read from the directory.
 * */

unsigned long int
ramfs_dir_import(struct ramfs_dir *dir,
                 const void *data,
                 unsigned long int data_size);

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
