// Copyright (C) 2017 - 2018 Taylor Holberton
//
// This file is part of Swanson.
//
// Swanson is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Swanson is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Swanson.  If not, see <http://www.gnu.org/licenses/>.

#ifndef SWANSON_CPU_HPP
#define SWANSON_CPU_HPP

#include <memory>

#include <cstdint>

namespace swanson {

class MemoryBus;

/// A Moxie CPU simulator.
/// This is used for executing application code.
class CPU final {
	/// Used for reading and writing memory.
	std::shared_ptr<MemoryBus> memoryBus;
	/// General-purpose registers.
	uint32_t regs[18];
	/// Special-purpose registers.
	uint32_t sregs[256];
	/// Condition register
	uint32_t condition;
	/// Instruction counter.
	uintmax_t instructionCount;
public:
	/// Default constructor
	CPU() noexcept;
	/// Default deconstructor
	~CPU() { }
	/// Get the value of a register by specifying
	/// the index of the register. There are eighteen
	/// registers. The first register has an index of
	/// zero. If an index above seventeen is passed,
	/// zero is returned.
	/// @param index The index of the register.
	/// @returns The value of the specified register.
	uint32_t GetRegister(uint32_t index) const noexcept;
	/// Get the current frame pointer address.
	/// @returns The current frame pointer address.
	auto GetFramePointer() const noexcept { return regs[0]; }
	/// Get the current stack pointer address.
	/// @returns The current stack pointer address.
	auto GetStackPointer() const noexcept { return regs[1]; }
	/// Get the current instruction pointer address.
	/// @returns The current instruction pointer address.
	auto GetInstructionPointer() const noexcept { return regs[16]; }
	/// Set the condition state of the CPU.
	/// Do not use this function unless you have a very
	/// specific reason for it.
	/// @param condition_ The new condition code for the CPU.
	void SetCondition(uint32_t condition_) noexcept { condition = condition_; }
	/// Set the frame pointer address. This should be an address
	/// that may be read from or written to.
	/// @param addr The address for the new frame pointer.
	void SetFramePointer(uint32_t addr) noexcept { regs[0] = addr; }
	/// Set the value of a register by specifying
	/// the index of the register.
	/// @param index The index of the register.
	/// @param value The value to assign the register.
	void SetRegister(uint32_t index, uint32_t value) noexcept;
	/// Set the stack pointer address. This should be an address
	/// that may be read from or written to.
	void SetStackPointer(uint32_t addr) noexcept { regs[1] = addr; }
	/// Set the instruction pointer address. This should be
	/// an address that may be read from and executed.
	void SetInstructionPointer(uint32_t addr) noexcept { regs[16] = addr; }
	/// Assign a new memory bus to the CPU.
	/// This will overwrite the previous one (if any.)
	/// @param memoryBus_ The new memory bus for the CPU.
	void SetMemoryBus(std::shared_ptr<MemoryBus> memoryBus_) noexcept;
	/// Execute a certain number of instructions.
	/// @param steps The number of instructions
	/// to execute.
	void Step(uint32_t steps);
protected:
	/// Handle a bad instruction.
	void HandleBadInstruction();
	/// Handle a break
	void HandleBreak();
	/// Handle a divide-by-zero error.
	void HandleDivideByZero();
	/// Handle a stack overflow.
	void HandleStackOverflow();
	/// Handle a software interrupt, specified by type.
	/// @param type The type of interrupt.
	void HandleInterrupt(uint32_t type);
	/// Execute a single instruction.
	void StepOnce();
	/// Pop a 32-bit value from the stack.
	uint32_t Pop32();
	/// Push a 32-bit value to the stack.
	void Push32(uint32_t value);
	/// Jump to a subroutine. This is
	/// basically a function call.
	/// @param addr The address of the subroutine.
	void JumpToSubroutine(uint32_t addr);
	/// Return from a subroutine.
	void ReturnFromSubroutine();
	/// Get the memory bus, if it exists.
	/// This function will throw an exception
	/// if there has not yet been a memory bus
	/// assigned to the CPU.
	MemoryBus &GetMemoryBus();
};

} // namespace swanson

#endif /* SWANSON_CPU_HPP */
