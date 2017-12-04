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

#include "ext4.h"

#include "stream.h"

#ifndef NULL
#define NULL ((void *) 0x00)
#endif

void
ext4_superblock_init(struct ext4_superblock *superblock) {
	superblock->inode_count = 0;
	superblock->block_count = 0;
	superblock->root_block_count = 0;
	superblock->free_block_count = 0;
	superblock->free_inode_count = 0;
	superblock->first_data_block = 0;
	superblock->logarithmic_block_size = 0;
	superblock->logarithmic_cluster_size = 0;
	superblock->blocks_per_group = 0;
	superblock->clusters_per_group = 0;
	superblock->inodes_per_group = 0;
	superblock->mount_time = 0;
	superblock->write_time = 0;
	superblock->mount_count = 0;
	superblock->max_mount_count = 14;
	superblock->signature = EXT4_SIGNATURE;
	/* 0x01 means that the file system
	 * was unmounted cleanly. */
	superblock->state = 0x01;
	/* 0x03 means panic if there are
	 * errors. */
	superblock->error_behavior = 0x03;
	superblock->minor_version = 0;
	superblock->last_check = 0;
	/* two weeks */
	superblock->check_interval = 1209600;
	/* zero means Linux, there's no
	 * actual 'null' value. */
	superblock->creator_os = 0x00;
	superblock->version = 0;
	superblock->reserved_uid = 0;
	superblock->reserved_gid = 0;
}

enum ext4_error
ext4_superblock_read(struct ext4_superblock *superblock,
                     struct stream *stream) {

	uint64_t read_count;

	read_count = 0;
	read_count += stream_decode_uint32le(stream, &superblock->inode_count);
	read_count += stream_decode_uint32le(stream, &superblock->block_count);
	read_count += stream_decode_uint32le(stream, &superblock->root_block_count);
	read_count += stream_decode_uint32le(stream, &superblock->free_block_count);
	read_count += stream_decode_uint32le(stream, &superblock->free_inode_count);
	read_count += stream_decode_uint32le(stream, &superblock->first_data_block);
	read_count += stream_decode_uint32le(stream, &superblock->logarithmic_block_size);
	read_count += stream_decode_uint32le(stream, &superblock->logarithmic_cluster_size);
	read_count += stream_decode_uint32le(stream, &superblock->blocks_per_group);
	read_count += stream_decode_uint32le(stream, &superblock->clusters_per_group);
	read_count += stream_decode_uint32le(stream, &superblock->inodes_per_group);
	read_count += stream_decode_uint32le(stream, &superblock->mount_time);
	read_count += stream_decode_uint32le(stream, &superblock->write_time);
	read_count += stream_decode_uint16le(stream, &superblock->mount_count);
	read_count += stream_decode_uint16le(stream, &superblock->max_mount_count);
	read_count += stream_decode_uint16le(stream, &superblock->signature);
	read_count += stream_decode_uint16le(stream, &superblock->state);
	read_count += stream_decode_uint16le(stream, &superblock->error_behavior);
	read_count += stream_decode_uint16le(stream, &superblock->minor_version);
	read_count += stream_decode_uint32le(stream, &superblock->last_check);
	read_count += stream_decode_uint32le(stream, &superblock->check_interval);
	read_count += stream_decode_uint32le(stream, &superblock->creator_os);
	read_count += stream_decode_uint32le(stream, &superblock->version);
	read_count += stream_decode_uint16le(stream, &superblock->reserved_uid);
	read_count += stream_decode_uint16le(stream, &superblock->reserved_gid);

	if (read_count == 0)
		return EXT4_ERROR_PARTIAL_READ;
	else if (superblock->signature != EXT4_SIGNATURE)
		return EXT4_ERROR_BAD_SIGNATURE;

	return EXT4_ERROR_NONE;
}

enum ext4_error
ext4_superblock_write(const struct ext4_superblock *superblock,
                      struct stream *stream) {

	uint64_t write_count;

	write_count = 0;
	write_count += stream_encode_uint32le(stream, superblock->inode_count);
	write_count += stream_encode_uint32le(stream, superblock->block_count);
	write_count += stream_encode_uint32le(stream, superblock->root_block_count);
	write_count += stream_encode_uint32le(stream, superblock->free_block_count);
	write_count += stream_encode_uint32le(stream, superblock->free_inode_count);
	write_count += stream_encode_uint32le(stream, superblock->first_data_block);
	write_count += stream_encode_uint32le(stream, superblock->logarithmic_block_size);
	write_count += stream_encode_uint32le(stream, superblock->logarithmic_cluster_size);
	write_count += stream_encode_uint32le(stream, superblock->blocks_per_group);
	write_count += stream_encode_uint32le(stream, superblock->clusters_per_group);
	write_count += stream_encode_uint32le(stream, superblock->inodes_per_group);
	write_count += stream_encode_uint32le(stream, superblock->mount_time);
	write_count += stream_encode_uint32le(stream, superblock->write_time);
	write_count += stream_encode_uint16le(stream, superblock->mount_count);
	write_count += stream_encode_uint16le(stream, superblock->max_mount_count);
	write_count += stream_encode_uint16le(stream, superblock->signature);
	write_count += stream_encode_uint16le(stream, superblock->state);
	write_count += stream_encode_uint16le(stream, superblock->error_behavior);
	write_count += stream_encode_uint16le(stream, superblock->minor_version);
	write_count += stream_encode_uint32le(stream, superblock->last_check);
	write_count += stream_encode_uint32le(stream, superblock->check_interval);
	write_count += stream_encode_uint32le(stream, superblock->creator_os);
	write_count += stream_encode_uint32le(stream, superblock->version);
	write_count += stream_encode_uint16le(stream, superblock->reserved_uid);
	write_count += stream_encode_uint16le(stream, superblock->reserved_gid);

	if (write_count == 0)
		return EXT4_ERROR_PARTIAL_WRITE;

	return EXT4_ERROR_NONE;
}

enum ext4_error
ext4_format(struct stream *stream) {

	int err;
	struct ext4_superblock superblock;

	ext4_superblock_init(&superblock);

	err = stream_setpos(stream, 1024);
	if (err != 0)
		return err;

	err = ext4_superblock_write(&superblock, stream);
	if (err != EXT4_ERROR_NONE)
		return err;

	return EXT4_ERROR_NONE;
}

void
ext4_accessor_init(struct ext4_accessor *accessor) {
	accessor->data = NULL;
	accessor->superblock = NULL;
}

int
ext4_access(struct ext4_accessor *accessor,
            struct stream *stream) {

	int err;
	struct ext4_superblock superblock;

	err = stream_setpos(stream, 1024);
	if (err != 0)
		return -1;

	ext4_superblock_init(&superblock);

	err = ext4_superblock_read(&superblock, stream);
	if (err != EXT4_ERROR_NONE)
		return -1;

	if (accessor->superblock != NULL) {
		err = accessor->superblock(accessor->data, &superblock);
		if (err != 0)
			return err;
	}

	return 0;
}

void
ext4_fs_init(struct ext4_fs *fs) {
	stream_init(&fs->stream);
	vfs_init(&fs->vfs);
}
