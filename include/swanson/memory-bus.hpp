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
	/// Read a 32-bit value from memory.
	/// @param addr The address of the value.
	/// @returns The value from memory.
	virtual uint32_t Read32(uint32_t addr) const = 0;
	/// Read a 16-bit value from memory.
	/// @param addr The address of the value.
	/// @returns The value from memory.
	virtual uint16_t Read16(uint32_t addr) const = 0;
	/// Read a 8-bit value from memory.
	/// @param addr The address of the value.
	/// @returns The value from memory.
	virtual uint8_t Read8(uint32_t addr) const = 0;
	/// Fetch a 16-bit executable code from memory.
	/// This function checks that the memory is executable.
	/// @param addr The address of the instruction.
	/// @returns The executable code from memory.
	virtual uint16_t Exec16(uint32_t addr) const = 0;
	/// Fetch a 32-bit executable code from memory.
	/// This function checks that the memory is executable.
	/// @param addr The address of the instruction.
	/// @returns The executable code from memory.
	virtual uint32_t Exec32(uint32_t addr) const = 0;
	/// Write a 32-bit value to the memory bus.
	/// @param addr The address to write the 32-bit value to.
	/// @param value The value to write to the memory bus.
	virtual void Write32(uint32_t addr, uint32_t value) = 0;
	/// Write a 16-bit value to the memory bus.
	/// @param addr The address to write the 32-bit value to.
	/// @param value The value to write to the memory bus.
	virtual void Write16(uint32_t addr, uint16_t value) = 0;
	/// Write an 8-bit value to the memory bus.
	/// @param addr The address to write the 32-bit value to.
	/// @param value The value to write to the memory bus.
	virtual void Write8(uint32_t addr, uint8_t value) = 0;
};

} // namespace swanson

#endif /* SWANSON_MEMORY_BUS_HPP */
