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

#ifndef SWANSON_MEMORY_SECTION_HPP
#define SWANSON_MEMORY_SECTION_HPP

#include <vector>

#include <cstdint>

namespace swanson {

/// A section of memory in the
/// memory map.
class MemorySection final {
	/// The virtual address of the
	/// memory section.
	uint32_t address;
	/// The bytes of memory associated
	/// with the memory section.
	std::vector<unsigned char> bytes;
public:
	/// Default constructor.
	MemorySection() noexcept : address(0x00) { }
	/// Default deconstructor.
	~MemorySection() { }
	/// Determine if an address exists
	/// within this memory section.
	/// @param addr The address to check for.
	/// @returns True if the address
	/// exists in the section, false if
	/// it does not.
	bool Exists(uint32_t addr) const noexcept;
	/// Read 32 bits from memory.
	/// @param addr The address to read from.
	/// @returns The 32-bit value at the
	/// specified address.
	uint32_t Read32(uint32_t addr) const;
	/// Resize the section of memory.
	/// Care should be taken that it
	/// does not overlap with the other
	/// sections of memory in the memory map.
	/// @param size The new size of the
	/// memory section.
	void Resize(uint32_t size);
	/// Set the virtual address of the
	/// memory section. Care should be
	/// taken that this address does not
	/// cause the memory section to overlap
	/// with another memory section in the
	/// memory map.
	/// @param addr The new virtual address
	/// of the memory section.
	void SetAddress(uint32_t addr) noexcept;
};

} // namespace swanson

#endif /* SWANSON_MEMORY_SECTION_HPP */
