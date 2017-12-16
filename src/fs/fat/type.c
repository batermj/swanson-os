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

#include "type.h"

#ifndef NULL
#define NULL ((void *) 0x00)
#endif

const char *
fat_type_to_string(enum fat_type type) {
	if (type == FAT_TYPE_16)
		return "FAT16";
	else if (type == FAT_TYPE_32)
		return "FAT32";
	else
		return "Unknown";
}

enum fat_type
fat_type_from_string(const char *type_string) {

	if (type_string == NULL)
		return FAT_TYPE_UNKNOWN;

	if (((type_string[0] == 'f') || (type_string[0] == 'F'))
	 && ((type_string[1] == 'a') || (type_string[1] == 'A'))
	 && ((type_string[2] == 't') || (type_string[2] == 'T'))) {
		if ((type_string[3] == '1')
		 && (type_string[4] == '6')) {
			return FAT_TYPE_16;
		} else if ((type_string[3] == '3')
		        && (type_string[4] == '2')) {
			return FAT_TYPE_32;
		} else {
			return FAT_TYPE_UNKNOWN;
		}
	} else {
		return FAT_TYPE_UNKNOWN;
	}
}
