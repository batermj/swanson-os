/* Copyright (C) 2018 Taylor Holberton
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

#ifndef SWANSON_MEMORY_BUS_HPP
#define SWANSON_MEMORY_BUS_HPP

#include <cstdint>

namespace swanson {

/// The interface between the CPU and
/// the memory manager.
class MemoryBus {
public:
	/// Default deconstructor
	virtual ~MemoryBus() { }
	/// Read a 32-bit word from memory.
	/// @param addr The address of the work.
	virtual uint32_t Read32(uint32_t addr) const = 0;
};

} // namespace swanson

#endif /* SWANSON_MEMORY_BUS_HPP */
