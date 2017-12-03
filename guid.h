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

#ifndef SWANSON_GUID_H
#define SWANSON_GUID_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct stream;

/** Defines the number of bytes in
 * a GUID.
 * */

#define GUID_SIZE 0x10

/** A globally unique identifier.
 * Used mainly for the GUID partition
 * table defined by the UEFI standard.
 * */

struct guid {
	/** The bytes of the GUID. */
	uint8_t buffer[GUID_SIZE];
};

/** Zero initialzies the GUID structure.
 * @param guid The structure to initialize.
 * */

void guid_init(struct guid *guid);

/** Reads a GUID from a stream.
 * @param stream An initialized stream structure.
 * @param guid The GUID structure that will receive
 * the data.
 * @returns The number of bytes read from the stream.
 * This should be equal to @ref GPT_GUID_PARTITION_SIZE
 * on success.
 * */

uint64_t guid_read(struct stream *stream,
                   struct guid *guid);

/** Writes a GUID to a stream.
 * @param stream An initialized stream structure.
 * @param guid The GUID to write.
 * @returns The number of bytes written to the
 * stream. This should be equal to @ref GPT_GUID_PARTITION_SIZE
 * on success.
 * */

uint64_t guid_write(struct stream *stream,
                    const struct guid *guid);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_GUID_H */
