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

#ifndef SWANSON_THREAD_HPP
#define SWANSON_THREAD_HPP

#include <memory>

namespace swanson {

class CPU;
class MemoryBus;

/// A thread in the Swanson kernel.
/// This is not an actual thread
/// running on the host system.
class Thread {
	/// The CPU responsible for
	/// executing the thread
	/// instructions.
	std::shared_ptr<CPU> cpu;
public:
	/// Default constructor
	Thread() noexcept;
	/// Default deconstructor
	~Thread() { }
	/// Set the CPU instruction pointer address.
	/// @param addr The new address of the
	/// instruction pointer.
	void SetInstructionPointer(uint32_t addr) noexcept;
	/// Set the stack pointer address.
	/// @param addr The new address of the
	/// stack pointer.
	void SetStackPointer(uint32_t addr) noexcept;
	/// Set the frame pointer address.
	/// @param addr The new address of the
	/// frame pointer.
	void SetFramePointer(uint32_t addr) noexcept;
	/// Assign a memory bus to the thread.
	/// This will overwrite the one that is
	/// currently present (if any.)
	/// @param memoryBus The new memory bus
	/// for the thread.
	void SetMemoryBus(std::shared_ptr<MemoryBus> memoryBus) noexcept;
	/// Run a specified number of instructions
	/// on the thread.
	/// @param steps The number of instructions
	/// to execute.
	void Step(uint32_t steps);
};

} // namespace swanson

#endif // SWANSON_THREAD_HPP
