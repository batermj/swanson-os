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

#ifndef SWANSON_EXT4_H
#define SWANSON_EXT4_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** A superblock in the ext4 file system.
 * This is the first data structure in a
 * block group.
 * */

struct ext4_superblock {
	/** The number of inodes in the file system. */
	uint32_t inode_count;
	/** The number of blocks in the file system. */
	uint32_t block_count;
	/** The number of blocks in the file system,
	 * available to the root user. */
	uint32_t root_block_count;
	/** The number of free blocks in the file system. */
	uint32_t free_block_count;
	/** The number of free inodes in the file system. */
	uint32_t free_inode_count;
	/** The first data block. This must be at least
	 * one for 1KiB-block file systems and typically
	 * zero for all other block sizes. */
	uint32_t first_data_block;
	/** The logarithmic size of a single block. Zero
	 * means one kibibyte, one means two kibibytes and
	 * so one. */
	uint32_t logarithmic_block_size;
	/** The logarithmic size of a cluster. */
	uint32_t logarithmic_cluster_size;
	/** The number of blocks per group. */
	uint32_t blocks_per_group;
	/** The number of clusters per group. */
	uint32_t clusters_per_group;
	/** The number of inodes per group. */
	uint32_t inodes_per_group;
	/** Mount time, in seconds since epoch. */
	uint32_t mount_time;
	/** Write time, in seconds since epoch. */
	uint32_t write_time;
	/** The number of mounts since a file
	 * system check. */
	uint16_t mount_count;
	/** The number of mounts beyond which
	 * a file system check is required. */
	uint16_t max_mount_count;
	/** The magic number (0xEF53) for the
	 * superblock. This is used by the kernel
	 * to help locating the superblock. */
	uint16_t signature;
	/** File system state. 0x01 means that
	 * the file system was cleanly unmounted,
	 * 0x02 means error were detected, 0x04
	 * means that orphans are being recovered. */
	uint16_t state;
	/** Behavior when errors are detected. 0x01
	 * means continue normally, 0x02 means mount
	 * read-only and 0x03 means panic. */
	uint16_t error_behavior;
	/** Minor revision level. */
	uint16_t minor_version;
	/** The time of the last check, in seconds
	 * since epoch. */
	uint32_t last_check;
	/** The maximum time between checks, in seconds. */
	uint32_t check_interval;
	/** The operating system that created the file
	 * system. */
	uint32_t creator_os;
	/** The major verson of ext4. 0x00 means the
	 * original format. 0x01 means version 2, which
	 * supports dynamic inode sizes. */
	uint32_t version;
	/** Default UID for reserved blocks. */
	uint16_t reserved_uid;
	/** Default GID for reserved blocks. */
	uint16_t reserved_gid;
};

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_EXT4_H */
