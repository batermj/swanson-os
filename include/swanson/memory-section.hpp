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
	/// Whether or not read operations
	/// are allowed in this section.
	bool readPermission;
	/// Whether or not write operations
	/// are allowed in this section.
	bool writePermission;
	/// Whether or not this section
	/// of memory is executable.
	bool executePermission;
	/// The bytes of memory associated
	/// with the memory section.
	std::vector<unsigned char> bytes;
public:
	/// Default constructor.
	MemorySection() noexcept : address(0x00),
	                           readPermission(true),
	                           writePermission(true),
	                           executePermission(false) { }
	/// Default deconstructor.
	~MemorySection() { }
	/// Get the address of the memory section.
	/// @returns The address of the memory section.
	auto GetAddress() const noexcept { return address; }
	/// Get the number of bytes occupied by
	/// the memory section.
	/// @returns The number of bytes occupied
	/// by the memory section.
	auto GetSize() const noexcept { return bytes.size(); }
	/// Indicate whether or not read
	/// operations may occur at this section.
	/// @param state True if read operations are
	/// allowed, false if they are not.
	void AllowRead(bool state) { readPermission = state; }
	/// Indicate whether or not write
	/// operations may occur at this section.
	/// @param state True if write operations are
	/// allowed, false if they are not.
	void AllowWrite(bool state) { writePermission = state; }
	/// Indicate whether or not execute
	/// permissions may occur at this section.
	/// @param state True if memory may be executed
	/// at this section, false if it can not be.
	void AllowExecute(bool state) { executePermission = state; }
	/// Copy data to the memory section.
	/// @param data The data to copy to the section.
	/// @param size The number of bytes to copy.
	void CopyData(const void *data, uint32_t size);
	/// Copy data to the memory section.
	/// @param bytes_ The data to copy to the section.
	void CopyData(const std::vector<unsigned char> &bytes_) { bytes = bytes_; }
	/// Determine if an address exists
	/// within this memory section.
	/// @param addr The address to check for.
	/// @returns True if the address
	/// exists in the section, false if
	/// it does not.
	bool Exists(uint32_t addr) const noexcept;
	/// Read a 32 bit value from memory.
	/// @param addr The address to read from.
	/// @returns The 32-bit value at the
	/// specified address.
	uint32_t Read32(uint32_t addr) const;
	/// Read a 16 bit value from memory.
	/// @param addr The address to read from.
	/// @returns The 16-bit value at the
	/// specified address.
	uint16_t Read16(uint32_t addr) const;
	/// Read an 8 bit value from memory.
	/// @param addr The address to read from.
	/// @returns The 8-bit value at the
	/// specified address.
	uint8_t Read8(uint32_t addr) const;
	/// Fetch a 32-bit component of an instruction.
	/// The section must have execute permission for this
	/// function to work without throwing an exception.
	/// @param addr The address of the instruction component.
	/// @returns The 32-bit component of the instruction.
	uint32_t Exec32(uint32_t addr) const;
	/// Fetch a 16-bit component of an instruction.
	/// The section must have execute permission for this
	/// function to work without throwing an exception.
	/// @param addr The address of the instruction component.
	/// @returns The 16-bit component of the instruction.
	uint16_t Exec16(uint32_t addr) const;
	/// Write a 32-bit value to the memory section.
	/// The section must have write permission for this
	/// function to work without causing an exception.
	/// @param addr The address to write the value at.
	/// @param value The value to write to the section.
	void Write32(uint32_t addr, uint32_t value);
	/// Write a 16-bit value to the memory section.
	/// The section must have write permission for this
	/// function to work without causing an exception.
	/// @param addr The address to write the value at.
	/// @param value The value to write to the section.
	void Write16(uint32_t addr, uint16_t value);
	/// Write an 8-bit value to the memory section.
	/// The section must have write permission for this
	/// function to work without causing an exception.
	/// @param addr The address to write the value at.
	/// @param value The value to write to the section.
	void Write8(uint32_t addr, uint8_t value);
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
