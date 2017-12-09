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

#ifndef SWANSON_DEBUG_H
#define SWANSON_DEBUG_H

#ifdef __cplusplus
extern "C" {
#endif

/** Prints a kernel debug message.
 * This function prints to the standard
 * output file.
 * @param fmt The printf-format for
 * the debug statement.
 * */

void debug(const char *fmt, ...);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* SWANSON_DEBUG_H */
