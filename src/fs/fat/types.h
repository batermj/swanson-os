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

#ifndef SWANSON_FS_FAT_TYPES_H
#define SWANSON_FS_FAT_TYPES_H

#include <stdint.h>

// Detect 64-bit compilation on GCC
#if defined(__GNUC__) && defined(__SIZEOF_LONG__)
    #if __SIZEOF_LONG__ == 8
        #define FATFS_DEF_UINT32_AS_INT
    #endif
#endif

//-------------------------------------------------------------
// Endian Macros
//-------------------------------------------------------------
// FAT is little endian so big endian systems need to swap words

// Little Endian - No swap required
#if FATFS_IS_LITTLE_ENDIAN == 1

    #define FAT_HTONS(n) (n)
    #define FAT_HTONL(n) (n)

// Big Endian - Swap required
#else

#define FAT_HTONS(n) ((((uint16_t)((n) & 0xff)) << 8) | (((n) & 0xff00) >> 8))

#define FAT_HTONL(n) \
               (((((uint32_t)(n) & 0xFF)) << 24) | \
                ((((uint32_t)(n) & 0xFF00)) << 8) | \
                ((((uint32_t)(n) & 0xFF0000)) >> 8) | \
                ((((uint32_t)(n) & 0xFF000000)) >> 24))

#endif

//-------------------------------------------------------------
// Structure Packing Compile Options
//-------------------------------------------------------------
#ifdef __GNUC__
    #define STRUCT_PACK
    #define STRUCT_PACK_BEGIN
    #define STRUCT_PACK_END
    #define STRUCT_PACKED           __attribute__ ((packed))
#else
    // Other compilers may require other methods of packing structures
    #define STRUCT_PACK
    #define STRUCT_PACK_BEGIN
    #define STRUCT_PACK_END
    #define STRUCT_PACKED
#endif

#endif /* SWANSON_FS_FAT_TYPES_H */
