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

#include "cache.h"

#include <string.h>

// Per file cluster chain caching used to improve performance.
// This does not have to be enabled for architectures with low
// memory space.

//-----------------------------------------------------------------------------
// fatfs_cache_init:
//-----------------------------------------------------------------------------
int
fatfs_cache_init(struct fatfs *fs,
                 struct fat_file *file) {

#ifdef FAT_CLUSTER_CACHE_ENTRIES
	int i;

	for (i = 0; i < FAT_CLUSTER_CACHE_ENTRIES; i++) {
		file->cluster_cache_idx[i] = 0xFFFFFFFF; // Not used
		file->cluster_cache_data[i] = 0;
	}
#endif

	return 1;
}
//-----------------------------------------------------------------------------
// fatfs_cache_get_next_cluster:
//-----------------------------------------------------------------------------
int
fatfs_cache_get_next_cluster(struct fatfs *fs,
                             struct fat_file *file,
                             uint32_t clusterIdx,
                             uint32_t *pNextCluster) {

#ifdef FAT_CLUSTER_CACHE_ENTRIES
	uint32_t slot = clusterIdx % FAT_CLUSTER_CACHE_ENTRIES;

	if (file->cluster_cache_idx[slot] == clusterIdx) {
		*pNextCluster = file->cluster_cache_data[slot];
		return 1;
	}
#endif

	return 0;
}
//-----------------------------------------------------------------------------
// fatfs_cache_set_next_cluster:
//-----------------------------------------------------------------------------
int
fatfs_cache_set_next_cluster(struct fatfs *fs,
                             struct fat_file *file,
                             uint32_t clusterIdx,
                             uint32_t nextCluster) {

#ifdef FAT_CLUSTER_CACHE_ENTRIES
	uint32_t slot = clusterIdx % FAT_CLUSTER_CACHE_ENTRIES;

	if (file->cluster_cache_idx[slot] == clusterIdx) {
		file->cluster_cache_data[slot] = nextCluster;
	} else {
		file->cluster_cache_idx[slot] = clusterIdx;
		file->cluster_cache_data[slot] = nextCluster;
	}
#endif

	return 1;
}
