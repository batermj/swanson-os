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

#include "guid.h"

void guid_init(struct guid *guid) {
	guid->buffer[0] = 0;
	guid->buffer[1] = 0;
	guid->buffer[2] = 0;
	guid->buffer[3] = 0;
	guid->buffer[4] = 0;
	guid->buffer[5] = 0;
	guid->buffer[6] = 0;
	guid->buffer[7] = 0;
}
