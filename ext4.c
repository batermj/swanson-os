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

void ext4_superblock_init(struct ext4_superblock *superblock) {
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
	superblock->signature = 0xef53;
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
