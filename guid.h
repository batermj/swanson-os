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

#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_GUID_H */
