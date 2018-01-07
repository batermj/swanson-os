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

#ifndef SWANSON_PATH_H
#define SWANSON_PATH_H

#ifdef __cplusplus
extern "C" {
#endif

/** A file or directory name that
 * is part of a path structure.
 * */

struct path_name {
	/** The name characters */
	char *data;
	/** The number of characters
	 * in the name. */
	unsigned long int size;
} *name_array;

/** A file system path.
 * */

struct path {
	/** The directory name array */
	struct path_name *name_array;
	/** The number of names in the
	 * directory name array. */
	unsigned long int name_count;
};

/** Initializes a path structure.
 * @param path An uninitialized
 * path structure.
 * */

void
path_init(struct path *path);

/** Releases resources allocated
 * by the path.
 * @param path An initialized path
 * structure.
 * */

void
path_free(struct path *path);

/** Get a directory name from the path.
 * @param path An initialized path structure.
 * @param index The index of the directory
 * name within the path to get.
 * @returns The name of the directory
 * within the path. If the index
 * is out of bounds, null is returned.
 * */

const char *
path_get_name(const struct path *path,
              unsigned int index);

/** Get the number of directories specified in the path.
 * @param path An initialized path structure.
 * @returns The number of directories in the path.
 * */

unsigned int
path_get_name_count(const struct path *path);

/** Resolve '.' and '..' references.
 * If there are more '..' references than
 * there are real directory names, the root
 * directory will be leftover.
 * @param path An initialized path structure
 * @returns Zero on success, non-zero on failure.
 * */

int
path_normalize(struct path *path);

/** Parses a path string.
 * @param path An initialized path string.
 * @param path_string The string to parse
 * path from.
 * @returns Zero on success, non-zero on
 * failure.
 * */

int
path_parse(struct path *path,
           const char *path_string);

/** Add a file or directory name to the
 * end of the path.
 * @param path An initialized path structure.
 * @param name The file or directory name to push.
 * @returns Zero on success, non-zero otherwise.
 * */

int
path_push_child(struct path *path,
                const char *name);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_PATH_H */
