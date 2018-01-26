// Copyright (C) 2018 Taylor Holberton
// 
//  This file is part of Swanson.
// 
//  Swanson is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
// 
//  Swanson is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
// 
//  You should have received a copy of the GNU General Public License
//  along with Swanson.  If not, see <http://www.gnu.org/licenses/>.

#ifndef SWANSON_BAD_INSTRUCTION_HPP
#define SWANSON_BAD_INSTRUCTION_HPP

#include <swanson/exception.hpp>

#include <cstdint>

namespace swanson {

/// An invalid instruction exception.
class BadInstruction final : public Exception {
	/// The ID of the process that the
	/// instruction was found in.
	uint32_t processID;
	/// The ID of the thread that the
	/// instruction was found in.
	uint32_t threadID;
	/// The memory address that the
	/// illegal instruction resides in.
	uint32_t address;
public:
	/// Default constructor
	BadInstruction() noexcept : Exception("Illegal instruction encountered.") {
		processID = 0;
		threadID = 0;
		address = 0;
	}
	/// Default deconstructor
	~BadInstruction() {

	}
	/// Get the process ID that the instruction was found in.
	/// @returns The process ID.
	auto GetProcessID() const noexcept { return processID; }
	/// Get the thread ID that the instruction was found in.
	/// @returns The thread ID.
	auto GetThreadID() const noexcept { return threadID; }
	/// Get the address that the instruction resides in.
	/// @returns The memory address of the instruction.
	auto GetAddress() const noexcept { return address; }
	/// Sets the ID of the process that
	/// the instruction was found in.
	/// @param id The process ID.
	void SetProcessID(uint32_t id) noexcept { processID = id; }
	/// Sets the ID of the thread that
	/// the instruction was found in.
	/// @param id The thread ID.
	void SetThreadID(uint32_t id) noexcept { threadID = id; }
	/// Sets the memory address where
	/// the instruction resides in.
	/// @param addr The memory address
	/// of the instruction.
	void SetAddress(uint32_t addr) noexcept { address = addr; }
};

} // namespace swanson

#endif // SWANSON_BAD_INSTRUCTION_HPP
