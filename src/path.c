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

#include "path.h"

#include <stdlib.h>
#include <string.h>

static int
is_separator(char c) {
	if ((c == '/') || (c == '\\'))
		return 1;
	else
		return 0;
}

void
path_init(struct path *path) {
	path->name_array = NULL;
	path->name_count = 0;
}

void
path_free(struct path *path) {

	unsigned int i;

	for (i = 0; i < path->name_count; i++)
		free(path->name_array[i].data);

	free(path->name_array);

	path->name_array = NULL;
	path->name_count = 0;
}

const char *
path_get_name(const struct path *path,
              unsigned int index) {

	if (index >= path->name_count)
		return NULL;

	return path->name_array[index].data;
}

unsigned int
path_get_name_count(const struct path *path) {

	return path->name_count;
}

int
path_parse(struct path *path,
           const char *path_str) {

	int err;
	unsigned int i;
	char *tmp;
	char *tmp2;
	unsigned int tmp_size;
	unsigned int tmp_res;

	tmp = NULL;
	tmp_size = 0;
	tmp_res = 0;

	for (i = 0; path_str[i]; i++) {
		if (is_separator(path_str[i])) {

			if (tmp_size == 0)
				continue;

			err = path_push_child(path, tmp);
			if (err != 0) {
				free(tmp);
				return -1;
			}

			tmp_size = 0;

		} else {

			if ((tmp_size + 1) >= tmp_res) {
				tmp_res += 64;
				tmp2 = realloc(tmp, tmp_res);
				if (tmp2 == NULL) {
					free(tmp);
					return -1;
				}
				tmp = tmp2;
			}

			tmp[tmp_size++] = path_str[i];
			tmp[tmp_size] = 0;
		}
	}

	if (tmp_size > 0) {
		err = path_push_child(path, tmp);
		if (err != 0) {
			free(tmp);
			return -1;
		}
	}

	free(tmp);

	return 0;
}

int
path_push_child(struct path *path,
                const char *name) {

	char *tmp_name;
	unsigned int name_size;
	struct path_name *name_array;
	unsigned int name_array_size;

	name_array = path->name_array;

	name_array_size = path->name_count + 1;
	name_array_size *= sizeof(path->name_array[0]);

	name_array = realloc(name_array, name_array_size);
	if (name_array == NULL)
		return -1;

	path->name_array = name_array;

	name_size = strlen(name);

	tmp_name = malloc(name_size + 1);
	if (tmp_name == NULL)
		return -1;

	memcpy(tmp_name, name, name_size);

	tmp_name[name_size] = 0;

	path->name_array[path->name_count].data = tmp_name;
	path->name_array[path->name_count].size = name_size;

	path->name_count++;

	return 0;
}
