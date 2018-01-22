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

#ifndef SWANSON_MEMORY_MAP_HPP
#define SWANSON_MEMORY_MAP_HPP

#include <swanson/memory-bus.hpp>

#include <memory>
#include <vector>

namespace swanson {

class MemorySection;

/// The memory map of a process.
class MemoryMap final : public MemoryBus {
	/// Sections of the memory map,
	/// since it is not continuous.
	std::vector<std::shared_ptr<MemorySection>> sections;
public:
	/// Default deconstructor
	~MemoryMap() { }
	/// Read 32-bits from memory.
	/// @param addr The address to read from.
	uint32_t Read32(uint32_t addr) const;
};

} // namespace swanson

#endif /* SWANSON_MEMORY_MAP_HPP */
