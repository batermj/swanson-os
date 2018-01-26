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
	/// Get the total number of bytes that may
	/// be contained by the memory map.
	/// @returns The number of bytes that may
	/// be contained by the memory map.
	uint32_t GetSize() const noexcept;
	/// Read a 32-bit value from memory.
	/// @param addr The address to read from.
	/// @returns The value from memory.
	uint32_t Read32(uint32_t addr) const;
	/// Read a 16-bit value from memory.
	/// @param addr The address to read from.
	/// @returns The value from memory.
	uint16_t Read16(uint32_t addr) const;
	/// Read an 8-bit value from memory.
	/// @param addr The address to read from.
	/// @returns The value from memory.
	uint8_t Read8(uint32_t addr) const;
	/// Fetch a 32-bit instruction from memory.
	/// The section of memory must have execute
	/// permission for this to work without an exception.
	/// @param addr The address of the 32-bit portion
	/// of the instruction.
	/// @returns The 32-bit instruction component.
	uint32_t Exec32(uint32_t addr) const;
	/// Fetch a 16-bit instruction from memory.
	/// The section of memory must have execute
	/// permission for this to work without an exception.
	/// @param addr The address of the 16-bit portion
	/// of the instruction.
	/// @returns The 16-bit instruction component.
	uint16_t Exec16(uint32_t addr) const;
	/// Write a 32-bit value to the memory map.
	/// The section that contains this address must
	/// have write permissions for this function to
	/// work without causing an exception.
	/// @param addr The address to write the value at.
	/// @param value The value to write to the memory map.
	void Write32(uint32_t addr, uint32_t value);
	/// Write a 16-bit value to the memory map.
	/// The section that contains this address must
	/// have write permissions for this function to
	/// work without causing an exception.
	/// @param addr The address to write the value at.
	/// @param value The value to write to the memory map.
	void Write16(uint32_t addr, uint16_t value);
	/// Write a 8-bit value to the memory map.
	/// The section that contains this address must
	/// have write permissions for this function to
	/// work without causing an exception.
	/// @param addr The address to write the value at.
	/// @param value The value to write to the memory map.
	void Write8(uint32_t addr, uint8_t value);
	/// Add a memory section to the memory map.
	/// @param memorySection The memory section to add.
	void AddSection(std::shared_ptr<MemorySection> &memorySection);
	/// Find an available address to accommodate a
	/// specified amount of memory and create a new
	/// section.
	/// @param size The size that the memory section
	/// should accommodate.
	/// @returns A pointer to the newly formed memory section.
	std::shared_ptr<MemorySection> AddSection(uint32_t size);
};

} // namespace swanson

#endif /* SWANSON_MEMORY_MAP_HPP */
