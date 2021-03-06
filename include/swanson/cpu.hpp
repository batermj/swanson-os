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
class InterruptHandler;

/// A Moxie CPU simulator.
/// This is used for executing application code.
class CPU final {
	/// Used for reading and writing memory.
	std::shared_ptr<MemoryBus> memoryBus;
	/// Used for handling interrupts, like
	/// system calls.
	std::shared_ptr<InterruptHandler> interruptHandler;
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
	/// Get the current condition
	/// state of the cpu.
	/// @returns The current condition
	/// state of the cpu.
	auto GetCondition() const noexcept { return condition; }
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
	/// Pop a 32-bit value from the stack.
	/// @returns The value that was popped
	/// from the stack.
	uint32_t Pop32();
	/// Push a 32-bit value to the stack.
	/// @param value The value to push.
	void Push32(uint32_t value);
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
	/// @param addr The new address of the stack pointer.
	void SetStackPointer(uint32_t addr) noexcept { regs[1] = addr; }
	/// Set the instruction pointer address. This should be
	/// an address that may be read from and executed.
	/// @param addr The new address of the instruction pointer.
	void SetInstructionPointer(uint32_t addr) noexcept { regs[16] = addr; }
	/// Assign a new memory bus to the CPU.
	/// This will overwrite the previous one (if any.)
	/// @param memoryBus_ The new memory bus for the CPU.
	void SetMemoryBus(std::shared_ptr<MemoryBus> memoryBus_) noexcept;
	/// Assign a new interrupt handler to the CPU.
	/// This will overwrite the previous one (if any.)
	/// @param interruptHandler_ The new interrupt handler.
	void SetInterruptHandler(std::shared_ptr<InterruptHandler> interruptHandler_) noexcept;
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
	/// @returns Whether or not the CPU
	/// should continue execution.
	bool StepOnce();
	/// Jump to a subroutine. This is
	/// basically a function call.
	/// @param addr The address of the subroutine.
	/// @param ret The return address.
	void JumpToSubroutine(uint32_t addr, uint32_t ret);
	/// Return from a subroutine.
	void ReturnFromSubroutine();
	/// Store a 32-bit value at the offset of a memory address.
	/// @param addr The base address.
	/// @param value The value to store.
	/// @param offset The offset to add to the base address.
	void StoreOffset32(uint32_t addr, uint32_t value, int16_t offset);
	/// Store a 16-bit value at the offset of a memory address.
	/// @param addr The base address.
	/// @param value The value to store.
	/// @param offset The offset to add to the base address.
	void StoreOffset16(uint32_t addr, uint16_t value, int16_t offset);
	/// Load the 32-bit value at the offset of a memory base address.
	/// @param a The destination register index.
	/// @param b The index of the register containing the base address.
	/// @param offset The offset to add to the base address.
	void LoadOffset32(uint8_t a, uint8_t b, int16_t offset);
	/// Load the 16-bit value at the offset of a memory base address.
	/// @param a The destination register index.
	/// @param b The index of the register containing the base address.
	/// @param offset The offset to add to the base address.
	void LoadOffset16(uint8_t a, uint8_t b, int16_t offset);
	/// Get the memory bus, if it exists.
	/// This function will throw an exception
	/// if there has not yet been a memory bus
	/// assigned to the CPU.
	/// @returns A reference to the memory bus.
	MemoryBus &GetMemoryBus();
};

} // namespace swanson

#endif /* SWANSON_CPU_HPP */
