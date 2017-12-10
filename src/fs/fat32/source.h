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

#ifndef SWANSON_FS_FAT32_SOURCE_H
#define SWANSON_FS_FAT32_SOURCE_H

#ifdef __cplusplus
extern "C" {
#endif

/** Represents a source from which
 * a FAT32 file system may be read from
 * or written to.
 * */

struct fat32_source {
	/** Implementation data. */
	void *data;
};

/** Initializes a FAT32 source structure.
 * @param source An uninitialized FAT32 source.
 * */

void
fat32_source_init(struct fat32_source *source);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_FS_FAT32_SOURCE_H */
