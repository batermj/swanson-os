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

#ifndef SWANSON_FS_FAT_FILELIB_H
#define SWANSON_FS_FAT_FILELIB_H

#include "fat_opts.h"
#include "fat_access.h"
#include "fat_list.h"

#ifndef SEEK_CUR
    #define SEEK_CUR    1
#endif

#ifndef SEEK_END
    #define SEEK_END    2
#endif

#ifndef SEEK_SET
    #define SEEK_SET    0
#endif

#ifndef EOF
    #define EOF         (-1)
#endif

struct cluster_lookup {
	uint32_t ClusterIdx;
	uint32_t CurrentCluster;
};

typedef struct fat_file {
	uint32_t parentcluster;
	uint32_t startcluster;
	uint32_t bytenum;
	uint32_t filelength;
	int filelength_changed;
	char path[FATFS_MAX_LONG_FILENAME];
	char filename[FATFS_MAX_LONG_FILENAME];
	uint8_t shortfilename[11];
#ifdef FAT_CLUSTER_CACHE_ENTRIES
	uint32_t cluster_cache_idx[FAT_CLUSTER_CACHE_ENTRIES];
	uint32_t cluster_cache_data[FAT_CLUSTER_CACHE_ENTRIES];
#endif
	/// Cluster Lookup
	struct cluster_lookup last_fat_lookup;
	// Read/Write sector buffer
	uint8_t file_data_sector[FAT_SECTOR_SIZE];
	uint32_t file_data_address;
	int file_data_dirty;
	// File fopen flags
	uint8_t flags;
#define FILE_READ        (1 << 0)
#define FILE_WRITE       (1 << 1)
#define FILE_APPEND      (1 << 2)
#define FILE_BINARY      (1 << 3)
#define FILE_ERASE       (1 << 4)
#define FILE_CREATE      (1 << 5)
	struct fat_node list_node;
};

void
fl_init(void);

void
fl_attach_locks(void (*lock)(void), void (*unlock)(void));

int
fl_attach_media(struct fat_disk *disk);

void
fl_shutdown(void);

void*
fl_fopen(const char *path, const char *modifiers);

void
fl_fclose(void *file);

int
fl_fflush(void *file);

int
fl_fgetc(void *file);

char *
fl_fgets(char *s, int n, void *f);

int
fl_fputc(int c, void *file);

int
fl_fputs(const char *str, void *file);

int
fl_fwrite(const void *data, int size, int count, void *file);

int
fl_fread(void *data, int size, int count, void *file);

int
fl_fseek(void *file , long offset , int origin);

int
fl_fgetpos(void *file , uint32_t *position);

long
fl_ftell(void *f);

int
fl_feof(void *f);

int
fl_remove(const char *filename);

// Equivelant dirent.h
typedef struct fs_dir_list_status FL_DIR;
typedef struct fs_dir_ent fl_dirent;

FL_DIR*
fl_opendir(const char* path, FL_DIR *dir);

int
fl_readdir(FL_DIR *dirls, fl_dirent *entry);

int
fl_closedir(FL_DIR* dir);

void
fl_listdirectory(const char *path);

int
fl_createdirectory(const char *path);

int
fl_is_dir(const char *path);

int
fl_format(uint32_t volume_sectors, const char *name);

#ifdef FATFS_INC_TEST_HOOKS
struct fatfs*
fl_get_fs(void);
#endif

#endif /* SWANSON_FS_FAT_FILELIB_H */
