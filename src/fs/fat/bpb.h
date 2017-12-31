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

#ifndef SWANSON_FS_FAT_BPB_H
#define SWANSON_FS_FAT_BPB_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct stream;

struct fat_bpb {
	/** Bytes per sector. */
	uint16_t bytes_per_sector;
	/** Sectors per cluster. */
	uint8_t sectors_per_cluster;
	uint16_t reserved_sector_count;
	/** Number of FATs on the volume. This
	 * usually two but can also be one. */
	uint8_t fat_count;
	/** For FAT12/FAT16 volumes, this
	 * field contains the number of 32-byte
	 * directory entries in the root directory.
	 * For FAT32, this is set to zero. */
	uint16_t root_entry_count;
	/** For FAT12/FAT16, this is the sector count
	 * of all four regions of the volume. For FAT32,
	 * this field is zero. */
	uint16_t sector_count16;
	/** Describes whether or not the media is removable.
	 * 0xF8 means that the media is non-removable, 0xF0 means
	 * that the storage is removable. */
	uint8_t media_type;
	/** For FAT12/FAT16 this field is the size of one FAT.
	 * On FAT32, this field must be zero. */
	uint16_t fat_size16;
	/** The number of sectors per track, if the storage medium
	 * has a geometry. */
	uint16_t sectors_per_track;
	/** The number of heads in the storage medium. */
	uint16_t head_count;
	/** The number of hidden sectors that precede this FAT
	 * volume. This is only relevant for partitioned volumes. */
	uint32_t hidden_sector_count;
	/** The number of sectors on all four regions of the volume.
	 * This is only relevant for FAT32. */
	uint32_t sector_count32;
	union {
		struct {
			uint8_t drive_index;
			uint8_t reserved;
			uint8_t boot_signature;
			uint32_t volume_serial_number;
			char volume_label[11];
			char fs_type[8];
		} fat16_info;
		struct {

		} fat32_info;
	} u;
};

void
fat_bpb_init(struct fat_bpb *bpb);

unsigned int
fat_bpb_write(const struct fat_bpb *bpb,
              struct stream *stream);

unsigned int
fat_bpb_read(struct fat_bpb *bpb,
             struct stream *stream);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_FS_FAT_BPB_H */
