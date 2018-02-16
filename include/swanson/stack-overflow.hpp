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

#ifndef SWANSON_STACK_OVERFLOW_HPP
#define SWANSON_STACK_OVERFLOW_HPP

#include <swanson/exception.hpp>

#include <cstdint>

namespace swanson {

/// The base class of any exception
/// that may occur from the Swanson library.
class StackOverflow final : public Exception {
	/// The address that the segmentation
	/// fault occured at.
	uint32_t addr;
	/// The address of the instruction that
	/// caused the stack overflow.
	uint32_t ip;
	/// The ID of the thread that caused the
	/// stack overflow.
	uint32_t threadID;
	/// The ID of the process that caused the
	/// stack overflow.
	uint32_t processID;
public:
	/// Default constructor.
	StackOverflow() : Exception("Stack overflow detected."), addr(0), ip(0), threadID(0), processID(0) { }
	/// Default deconstructor.
	~StackOverflow() { }
	/// Set the address of the stack overflow.
	/// @param addr_ The address of the fault.
	void SetAddress(uint32_t addr_) noexcept { addr = addr_; }
	/// Set the address of the instruction that caused the fault.
	/// @param ip_ The address of the instruction.
	void SetInstructionPointer(uint32_t ip_) noexcept { ip = ip_; }
	/// Set the ID of the thread that caused the fault.
	/// @param threadID_ The ID of the thread.
	void SetThreadID(uint32_t threadID_) noexcept { threadID = threadID_; }
	/// Set the ID of the process that caused the fault.
	/// @param processID_ The ID of the process.
	void SetProcessID(uint32_t processID_) noexcept { processID = processID_; }
	/// Get the address that the stack overflow occured.
	/// @returns The address that the stack overflow
	/// occured at.
	uint32_t GetAddress() const noexcept { return addr; }
	/// Get the address of the instruction that caused the fault.
	/// @returns The address of the instruction that caused the fault.
	uint32_t GetInstructionPointer() const noexcept { return ip; }
};

} // namespace swanson

#endif /* SWANSON_STACK_OVERFLOW_HPP */
