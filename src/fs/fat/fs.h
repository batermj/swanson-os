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

#ifndef SWANSON_FS_FAT_FS_H
#define SWANSON_FS_FAT_FS_H

#include <stdint.h>

#include "disk.h"
#include "fat_defs.h"
#include "fat_opts.h"

#define FAT_INIT_OK 0

#define FAT_INIT_MEDIA_ACCESS_ERROR (-1)

#define FAT_INIT_INVALID_SECTOR_SIZE (-2)

#define FAT_INIT_INVALID_SIGNATURE (-3)

#define FAT_INIT_ENDIAN_ERROR (-4)

#define FAT_INIT_WRONG_FILESYS_TYPE (-5)

#define FAT_INIT_WRONG_PARTITION_TYPE (-6)

#define FAT_INIT_STRUCT_PACKING (-7)

#define FAT_DIR_ENTRIES_PER_SECTOR (FAT_SECTOR_SIZE / FAT_DIR_ENTRY_SIZE)

#ifdef __cplusplus
extern "C" {
#endif

struct fat_buffer;

struct fat_buffer {
	uint8_t sector[FAT_SECTOR_SIZE * FAT_BUFFER_SECTORS];
	uint32_t address;
	int dirty;
	uint8_t *ptr;
	// Next in chain of sector buffers
	struct fat_buffer *next;
};

typedef enum eFatType {
	FAT_TYPE_16,
	FAT_TYPE_32
} tFatType;

struct fatfs {
	/* Filesystem globals */
	uint8_t sectors_per_cluster;
	uint32_t cluster_begin_lba;
	uint32_t rootdir_first_cluster;
	uint32_t rootdir_first_sector;
	uint32_t rootdir_sectors;
	uint32_t fat_begin_lba;
	uint16_t fs_info_sector;
	uint32_t lba_begin;
	uint32_t fat_sectors;
	uint32_t next_free_cluster;
	uint16_t root_entry_count;
	uint16_t reserved_sectors;
	uint8_t num_of_fats;
	enum eFatType fat_type;
	/** Source of FAT data. */
	struct fat_disk disk;
	/* [Optional] Thread Safety */
	void (*fl_lock)(void);
	void (*fl_unlock)(void);
	/* Working buffer */
	struct fat_buffer currentsector;
	/* FAT Buffer */
	struct fat_buffer *fat_buffer_head;
	struct fat_buffer fat_buffers[FAT_BUFFERS];
};

struct fs_dir_list_status {
	uint32_t sector;
	uint32_t cluster;
	uint8_t offset;
};

struct fs_dir_ent {
	char filename[FATFS_MAX_LONG_FILENAME];
	uint8_t is_dir;
	uint32_t cluster;
	uint32_t size;
#if FATFS_INC_TIME_DATE_SUPPORT
	uint16_t access_date;
	uint16_t write_time;
	uint16_t write_date;
	uint16_t create_date;
	uint16_t create_time;
#endif
};

int
fatfs_init(struct fatfs *fs);

uint32_t
fatfs_lba_of_cluster(struct fatfs *fs,
                     uint32_t Cluster_Number);

int
fatfs_sector_reader(struct fatfs *fs,
                    uint32_t Startcluster,
                    uint32_t offset,
                    uint8_t *target);

int
fatfs_sector_read(struct fatfs *fs,
                  uint32_t lba,
                  uint8_t *target,
                  uint32_t count);

int
fatfs_sector_write(struct fatfs *fs,
                   uint32_t lba,
                   uint8_t *target,
                   uint32_t count);

int
fatfs_read_sector(struct fatfs *fs,
                  uint32_t cluster,
                  uint32_t sector,
                  uint8_t *target);

int
fatfs_write_sector(struct fatfs *fs,
                   uint32_t cluster,
                   uint32_t sector,
                   uint8_t *target);

void
fatfs_show_details(struct fatfs *fs);

uint32_t
fatfs_get_root_cluster(struct fatfs *fs);

uint32_t
fatfs_get_file_entry(struct fatfs *fs,
                     uint32_t Cluster,
                     char *nametofind,
                     struct fat_dir_entry *sfEntry);

int
fatfs_sfn_exists(struct fatfs *fs,
                 uint32_t Cluster,
                 char *shortname);

int
fatfs_update_file_length(struct fatfs *fs,
                         uint32_t Cluster,
                         char *shortname,
                         uint32_t fileLength);

int
fatfs_mark_file_deleted(struct fatfs *fs,
                        uint32_t Cluster,
                        char *shortname);

void
fatfs_list_directory_start(struct fatfs *fs,
                           struct fs_dir_list_status *dirls,
                           uint32_t StartCluster);

int
fatfs_list_directory_next(struct fatfs *fs,
                          struct fs_dir_list_status *dirls,
                          struct fs_dir_ent *entry);

int
fatfs_update_timestamps(struct fat_dir_entry *directoryEntry,
                        int create,
                        int modify,
                        int access);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_FS_FAT_FS_H */
