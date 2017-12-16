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

#ifndef SWANSON_FS_FAT_TYPE_H
#define SWANSON_FS_FAT_TYPE_H

#ifdef __cplusplus
extern "C" {
#endif

/** Indicates the type of a FAT
 * filesystem. */

enum fat_type {
	/** The filesystem type is an
	 * unknown version of FAT. */
	FAT_TYPE_UNKNOWN,
	/** The filesystem is FAT16. */
	FAT_TYPE_16,
	/** The filesystem is FAT32. */
	FAT_TYPE_32,
	/** The first supported FAT type. */
	FAT_TYPE_FIRST = FAT_TYPE_16,
	/** The last supported FAT type. */
	FAT_TYPE_LAST = FAT_TYPE_32
};

/** Returns a string representation
 * of a FAT type.
 * @param type The type to get a string
 * representation for.
 * @returns A string that indicates the
 * type of FAT.
 * */

const char *
fat_type_to_string(enum fat_type type);

/** Parses a string to determine a FAT type.
 * @param type_string A string containing the
 * type of FAT. Currently, this may be 'FAT16'
 * or 'FAT32' (case insensitive).
 * @returns The type of FAT indicated by the
 * string.  If the string is malformed,
 * then @ref FAT_TYPE_UNKNOWN is returned.
 * */

enum fat_type
fat_type_from_string(const char *type_string);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_FS_FAT_TYPE_H */
