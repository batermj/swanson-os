#ifndef SWANSON_FS_FAT_FORMAT_H
#define SWANSON_FS_FAT_FORMAT_H

#include "fat_defs.h"
#include "fat_opts.h"

#include "fs.h"

#ifdef __cplusplus
extern "C" {
#endif

int
fatfs_format(struct fatfs *fs,
             uint32_t volume_sectors,
             const char *name);

int
fatfs_format_fat16(struct fatfs *fs,
                   uint32_t volume_sectors,
                   const char *name);

int
fatfs_format_fat32(struct fatfs *fs,
                   uint32_t volume_sectors,
                   const char *name);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_FS_FAT_FORMAT_H */
