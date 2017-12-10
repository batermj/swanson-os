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

#ifndef SWANSON_FS_FAT32_ERROR_H
#define SWANSON_FS_FAT32_ERROR_H

#ifdef __cplusplus
extern "C" {
#endif

/** A list of errors that may
 * occur in a FAT32 function.
 * */

enum fat32_error {
	/** No error occurred. */
	FAT32_ERROR_NONE,
	/** An unknown error occurred. */
	FAT32_ERROR_UNKNOWN,
	/** An error occurred in a disk function. */
	FAT32_ERROR_DISK_ERROR,
	/** The disk sector size is invalid. */
	FAT32_ERROR_INVALID_SECTOR_SIZE,
	/** The disk does not contain a valid FAT32 signature. */
	FAT32_ERROR_INVALID_SIGNATURE,
	/** An incorrect endian value encountered.
	 * TODO : can this be taken out? */
	FAT32_ERROR_ENDIAN,
	/** Bad file system type.
	 * TODO : how is this different from
	 * @ref FAT32_ERROR_INVALID_SIGNATURE */
	FAT32_ERROR_INVALID_FILESYS_TYPE,
	/** Bad partition type.
	 * TODO : what does this mean exactly?
	 * What is considered a 'good' partition
	 * type? */
	FAT32_ERROR_INVALID_PARTITION_TYPE
};

/** Returns a string representation
 * of an error code to present to a
 * user.
 * @param error The error code to get
 * the string representation for.
 * @returns A string representation of
 * the error code. If the error code is
 * invalid, it is considered @ref FAT32_ERROR_UNKNOWN.
 * The value returned is never @ref NULL.
 * */

const char *
fat32_strerror(enum fat32_error error);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_FS_FAT32_ERROR_H */
