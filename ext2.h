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

#ifndef SWANSON_EXT2_H
#define SWANSON_EXT2_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** A superblock in the ext2 file system.
 * This is the first data structure in a
 * block group.
 * */

struct ext2_superblock {
	/** The number of inodes in the file system. */
	uint32_t inode_count;
	/** The number of blocks in the file system. */
	uint32_t block_count;
	/** The number of free blocks in the file system. */
	uint32_t free_block_count;
	/** The number of free inodes in the file system. */
	uint32_t free_inode_count;
	/** The first data block. */
	uint32_t first_data_block;
	/** The logarithmic size of a single block. Zero
	 * means one kibibyte, one means two kibibytes and
	 * so one. */
	uint32_t logarithmic_block_size;
	/** The number of blocks per group. */
	uint32_t blocks_per_group;
	/** The magic number for the superblock.
	 * This is used by the kernel to help
	 * locating the superblock. */
	uint16_t signature;
};

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_EXT2_H */
