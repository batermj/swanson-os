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

#include "stream.h"
#include "vfs.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** The signature for a ext4 file
 * system. This is contained by the
 * superblock.
 * @see ext4_superblock
 * */

#define EXT4_SIGNATURE 0xef53

/** A list of possible errors
 * that may occur from an ext4
 * function. */

enum ext4_error {
	/** No error occured. */
	EXT4_ERROR_NONE,
	/** An unknown error occured. */
	EXT4_ERROR_UNKNOWN,
	/** A read operation did not finish correctly. */
	EXT4_ERROR_PARTIAL_READ,
	/** A write operation did not finish correctly. */
	EXT4_ERROR_PARTIAL_WRITE,
	/** The ext4 superblock does not contain a valid
	 * file system signature. */
	EXT4_ERROR_BAD_SIGNATURE
};

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

/** Initializes an ext4 superblock with
 * reasonable values.
 * @param superblock An uninitialized superblock.
 * */

void
ext4_superblock_init(struct ext4_superblock *superblock);

/** Reads a superblock from a stream.
 * This function does not set the stream position.
 * The caller must ensure that the stream points
 * to where a superblock might be.
 * @param superblock The data structure to
 * receive the data from the stream.
 * @param The stream containing the superblock.
 * @returns See @ref ext4_error.
 * */

enum ext4_error
ext4_superblock_read(struct ext4_superblock *superblock,
                     struct stream *stream);

/** Writes a superblock to a stream.
 * This function does not set the stream position,
 * that must be done by the caller.
 * @param superblock An initialzied superblock.
 * @param stream The stream to write the superblock to.
 * @returns See @ref ext4_error.
 * */

enum ext4_error
ext4_superblock_write(const struct ext4_superblock *superblock,
                      struct stream *stream);

/** Formats a stream with ext4.
 * @param stream An initialized stream
 * structure.
 * @returns See @ref ext4_error.
 * */

enum ext4_error
ext4_format(struct stream *stream);

/** Used for reading ext4 data
 * structures. It abstracts the method
 * by which the data structures are decoded.
 * */

struct ext4_accessor {
	/** Implementation data. */
	void *data;
	/** Called when the accessor finds a superblock. */
	int (*superblock)(void *data, const struct ext4_superblock *superblock);
};

/** Initializes an accessor structure.
 * This function should be called before
 * the accessor is used.
 * @param accessor An uninitialized accessor
 * structure.
 * */

void
ext4_accessor_init(struct ext4_accessor *accessor);

/** Reads ext4 data structures and passes
 * them to the accessor.
 * @param accessor An initialized accessor.
 * @param stream A stream containing an ext4
 * formatted partition or drive.
 * @returns Zero on success, non-zero on failure.
 * */

int
ext4_access(struct ext4_accessor *accessor,
            struct stream *stream);

/** An inode in the ext4 file
 * system. Used to describe information
 * about a file.
 * */

struct ext4_inode {
	/** File mode. */
	uint16_t mode;
	/** Owner's user id. */
	uint16_t owner_uid;
	/** Lower 32-bit of the size. */
	uint32_t size_low;
	/** Last access time, in seconds since epoch. */
	uint32_t access_time;
	/** Last change time of the inode, in seconds
	 * since epoch. */
	uint32_t change_time;
	/** Last modification time of the data, in seconds
	 * since epoch. */
	uint32_t modification_time;
	/** Time that the inode was deleted, in seconds
	 * since epoch. */
	uint32_t deletion_time;
	/** Group ID */
	uint16_t gid;
	/** Hard link count. */
	uint16_t link_count;
	/** Lower 32-bits of the block count. */
	uint32_t block_count_low;
	/** Flags for the inode. */
	uint32_t flags;
	/** Kernel-specific data */
	union {
		/** Linux-specific data */
		struct {
			/** Version of the inode. */
			uint32_t version;
		} linux1;
	} osd1;
	uint8_t block[60];
	/** File version (for NFS) */
	uint32_t generation;
	/** Lower 32-bits of the extended
	 * attribute block. */
	uint32_t file_acl_low;
	/** Upper 32-bit of the size. */
	uint32_t size_high;
	/** Obsolete */
	uint32_t obsolete;
	/** More kernel-specific data. */
	union {
		/** Linux-specific data */
		struct {
			/** Upper 16-bits of the block count. */
			uint16_t block_count_high;
			/** Upper 16-bits of the extended
			 * attribute block. */
			uint16_t file_acl_high;
			/** Upper 16-bits of the owner ID. */
			uint16_t owner_uid_high;
			/** Upper 16-bits of the group ID. */
			uint16_t gid_high;
			/** Lower 16-bits of the checksum. */
			uint16_t checksum_low;
			/** Reserved. */
			uint16_t reserved;
		} linux2;
	} osd2;
	/** Size of this inode, minus 128. The inode
	 * is always at least 128 bytes. */
	uint16_t extra_size;
	/** The upper 16-bits of the inode checksum. */
	uint16_t checksum_high;
	/** Extra change time bits. */
	uint32_t extra_change_time;
	/** Extra modification time bits. */
	uint32_t extra_modification_time;
	/** Extra access time bits. */
	uint32_t extra_access_time;
	/** Creation time, in seconds since epoch. */
	uint32_t creation_time;
	/** Extra creation time bits. */
	uint32_t extra_creation_time;
	/** Upper 32-bits of the inode version. */
	uint32_t version_high;
	/** Project ID. */
	uint32_t project_id;
};

/** An ext4 file system.
 * */

struct ext4_fs {
	/** The stream used for reading from
	 * and writing to the file system. */
	struct stream stream;
	/** The virtual file system structure,
	 * used by the kernel to operate on the
	 * file system in a generic manner. */
	struct vfs vfs;
};

/** Initializes the ext4 file system
 * structure.
 * @param fs An uninitialized file system
 * structure.
 * */

void
ext4_fs_init(struct ext4_fs *fs);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_EXT4_H */
