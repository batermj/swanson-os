#ifndef __FAT_WRITE_H__
#define __FAT_WRITE_H__

#include "fat_defs.h"
#include "fat_opts.h"

//-----------------------------------------------------------------------------
// Prototypes
//-----------------------------------------------------------------------------
int fatfs_add_file_entry(struct fatfs *fs, uint32_t dirCluster, char *filename, char *shortfilename, uint32_t startCluster, uint32_t size, int dir);
int fatfs_add_free_space(struct fatfs *fs, uint32_t *startCluster, uint32_t clusters);
int fatfs_allocate_free_space(struct fatfs *fs, int newFile, uint32_t *startCluster, uint32_t size);

#endif
