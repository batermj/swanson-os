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

#include <swanson/cpu.hpp>

#include <swanson/bad-instruction.hpp>
#include <swanson/exception.hpp>
#include <swanson/interrupt-handler.hpp>
#include <swanson/memory-bus.hpp>
#include <swanson/syscall.hpp>

#include <cstring>
#include <csignal>

/* Since most instructions encode
 * a register number (or multiple register
 * numbers) within their 32-bit space, these
 * macros are available to easily decode them.
 * */

#define get_a(inst) ((inst & 0x00f0) >> 4)

#define get_b(inst) (inst & 0x000f)

#define get_v(inst) ((((int16_t)((inst & ((1 << 10) - 1)) << 6)) >> 6) << 1)

// Equal to.
#define CPU_CONDITION_EQ 0x00

// Greater than or equal to.
#define CPU_CONDITION_GE 0x06

// Greater than or equal to (unsigned).
#define CPU_CONDITION_GEU 0x08

// Greater than.
#define CPU_CONDITION_GT 0x03

// Greater than (unsigned).
#define CPU_CONDITION_GTU 0x05

// Less than or equal to.
#define CPU_CONDITION_LE 0x07

// Less than or equal to (unsigned).
#define CPU_CONDITION_LEU 0x09

// Less than.
#define CPU_CONDITION_LT 0x02

// Less than (unsigned).
#define CPU_CONDITION_LTU 0x04

// Not equal to.
#define CPU_CONDITION_NE 0x01

namespace {

uint32_t CalculateOffset(uint32_t addr, int16_t offset) {
	if (offset < 0) {
		if (((uint32_t) (offset * -1)) > addr)
			throw swanson::Exception("Integer underflow detected.");
	} else if (offset > 0) {
		if ((UINT32_MAX - ((uint32_t) offset)) < addr)
			throw swanson::Exception("Integer overflow detected.");
	}
	return (uint32_t)(((int32_t) addr) + offset);
}

} // namespace

namespace swanson {

CPU::CPU() noexcept {
	std::memset(regs, 0, sizeof(regs));
	std::memset(sregs, 0, sizeof(sregs));
	condition = CPU_CONDITION_EQ;
	instructionCount = 0;
}

uint32_t CPU::GetRegister(uint32_t index) const noexcept {
	if (index <= 17)
		return regs[index];
	else
		return 0;
}

void CPU::SetRegister(uint32_t index, uint32_t value) noexcept {
	if (index <= 17)
		regs[index] = value;
}

void CPU::Step(uint32_t steps) {
	for (decltype(steps) i = 0; i < steps; i++) {
		auto continuationFlag = StepOnce();
		if (!continuationFlag)
			break;
		instructionCount++;
	}
}

void CPU::SetMemoryBus(std::shared_ptr<MemoryBus> memoryBus_) noexcept {
	memoryBus = memoryBus_;
}

void CPU::SetInterruptHandler(std::shared_ptr<InterruptHandler> interruptHandler_) noexcept {
	interruptHandler = interruptHandler_;
}

void CPU::JumpToSubroutine(uint32_t addr, uint32_t ret) {

	/* Save slot in static chain */
	Push32(0x00);

	/* Return address */
	Push32(ret);

	/* Current frame pointer */
	Push32(GetFramePointer());

	SetFramePointer(GetStackPointer());

	SetInstructionPointer(addr);
}

void CPU::ReturnFromSubroutine() {

	// pop frame pointer
	SetFramePointer(Pop32());

	// pop return address
	SetInstructionPointer(Pop32());

	// skip static chain slot
	Pop32();
}

void CPU::StoreOffset32(uint32_t addr, uint32_t value, int16_t offset) {

	addr = CalculateOffset(addr, offset);

	auto &memoryBus = GetMemoryBus();

	memoryBus.Write32(addr, value);
}

void CPU::LoadOffset32(uint8_t a, uint8_t b, int16_t offset) {

	auto addr = regs[b];

	addr = CalculateOffset(addr, offset);

	auto &memoryBus = GetMemoryBus();

	regs[a] = memoryBus.Read32(addr);
}

bool CPU::StepOnce() {

	uint32_t a;
	uint32_t b;
	uint32_t reg_a;
	uint32_t reg_b;
	int16_t offset;

	auto &memoryBus = GetMemoryBus();

	auto instructionPointer = GetInstructionPointer();

	auto inst = memoryBus.Exec16(instructionPointer);

	/* check 4-bit instructions */

	switch ((inst & 0xf000) >> 0x0c) {
	case 0x08: /* inc */
		a = (inst & 0x0f00) >> 0x08;
		regs[a] += inst & 0xff;
		SetInstructionPointer(instructionPointer + 2);
		return true;
	case 0x09: /* dec */
		a = (inst & 0x0f00) >> 0x08;
		regs[a] -= inst & 0xff;
		SetInstructionPointer(instructionPointer + 2);
		return true;
	case 0x0a:
		a = (inst & 0x0f00) >> 0x08;
		regs[a] = sregs[inst & 0xff];
		SetInstructionPointer(instructionPointer + 2);
		return true;
	default:
		break;
	}

	/* check 6-bit instuctions */

	// These variables are used
	// in the branch instructions.
	uint8_t conditionRequired;
	uint8_t conditionPresent;
	// An immediate value that
	// follows an instruction.
	uint32_t immediate;
	switch ((inst & 0xfc00) >> 0x0a) {
	case 0x30: /* beq */
	case 0x36: /* bge */
	case 0x38: /* bgeu */
	case 0x33: /* bgt */
	case 0x35: /* bgtu */
	case 0x37: /* ble */
	case 0x39: /* bleu */
	case 0x32: /* blt */
	case 0x34: /* bltu */
	case 0x31: /* bne */
		/* condition code required */
		conditionRequired = (inst & 0x3c00) >> 0x0a;
		/* actual condition code of the cpu */
		conditionPresent = condition;
		/* get branch offset (if taken) */
		offset = get_v(inst);
		/* check for underflow */
		if (offset < 0) {
			if (((uint16_t)(offset * -1)) > instructionPointer) {
				HandleBadInstruction();
				return false;
			}
		}

		/* check for overflow */
		if (offset > 0) {
			if (instructionPointer > (UINT32_MAX - ((uint32_t) offset))) {
				HandleBadInstruction();
				return false;
			}
		}
		/* check if branch is taken */
		if (conditionRequired == conditionPresent) {

			if (offset > 0)
				instructionPointer += (uint32_t)(offset);
			else
				instructionPointer -= (uint32_t)(offset * -1);

			SetInstructionPointer(instructionPointer);
			return true;
		}

		/* branch not taken */
		instructionPointer += 2;
		SetInstructionPointer(instructionPointer);
		return true;
	default:
		break;
	}

	/* check 8-bit instuctions */

	switch ((inst & 0xff00) >> 8) {
	case 0x26: /* and */
		a = get_a(inst);
		b = get_b(inst);
		regs[a] = regs[a] & regs[b];
		break;
	case 0x05: /* add */
		a = get_a(inst);
		b = get_b(inst);
		regs[a] = regs[a] + regs[b];
		break;
	case 0x28: /* ashl */
		a = get_a(inst);
		b = get_b(inst);
		regs[a] = regs[a] << regs[b];
		break;
	case 0x2d: /* ashr */
		a = get_a(inst);
		b = get_b(inst);
		regs[a] = regs[a] >> regs[b];
		break;
	case 0x35: /* brk */
		HandleBreak();
		return false;
	case 0x0e: /* cmp */

		a = get_a(inst);
		b = get_b(inst);

		reg_a = regs[a];
		reg_b = regs[b];

		if (reg_a > reg_b)
			condition = CPU_CONDITION_GTU;
		else if (reg_a < reg_b)
			condition = CPU_CONDITION_LTU;
		else
			condition = CPU_CONDITION_EQ;

		if (((int32_t) reg_a) > ((int32_t) reg_b))
			condition |= CPU_CONDITION_GT;
		else if (((int32_t) reg_a) < ((int32_t) reg_b))
			condition |= CPU_CONDITION_LT;

		break;

	case 0x31: /* div */
		a = get_a(inst);
		b = get_b(inst);
		if (regs[b] == 0) {
			HandleDivideByZero();
			return false;
		}
		regs[a] /= regs[b];
		break;
	case 0x25: /* jmp */
		a = get_a(inst);
		SetInstructionPointer(regs[a]);
		return true;
	case 0x1a: /* jmpa */
		instructionPointer = memoryBus.Exec32(instructionPointer + 2);
		SetInstructionPointer(instructionPointer);
		return true;
	case 0x19: /* jsr */
		JumpToSubroutine(regs[get_a(inst)], instructionPointer + 2);
		return true;
	case 0x03: /* jsra */
		immediate = memoryBus.Exec32(instructionPointer + 2);
		JumpToSubroutine(immediate, instructionPointer + 6);
		return true;
	case 0x1c: /* ld.b */
		a = get_a(inst);
		b = get_b(inst);
		regs[a] = memoryBus.Read8(regs[b]);
		break;
	case 0x0a: /* ld.l */
		a = get_a(inst);
		b = get_b(inst);
		regs[a] = memoryBus.Read32(regs[b]);
		break;
	case 0x21: /* ld.s */
		a = get_a(inst);
		b = get_b(inst);
		regs[a] = memoryBus.Read16(regs[b]);
		break;
	case 0x01: /* ldi.l */
		a = get_a(inst);
		regs[a] = memoryBus.Exec32(instructionPointer + 2);
		SetInstructionPointer(instructionPointer + 6);
		return true;
	case 0x0c: /* ldo.l */
		a = get_a(inst);
		b = get_b(inst);
		instructionPointer += 2;
		LoadOffset32(a, b, (int16_t) memoryBus.Exec16(instructionPointer));
		break;
	case 0x02: /* mov */
		a = get_a(inst);
		b = get_b(inst);
		regs[a] = regs[b];
		break;
	case 0x0f: /* nop */
		break;
	case 0x04: /* ret */
		ReturnFromSubroutine();
		return true;
	case 0x0d: /* sto.l */
		a = get_a(inst);
		b = get_b(inst);
		instructionPointer += 2;
		StoreOffset32(regs[a], regs[b], (int16_t) memoryBus.Exec16(instructionPointer));
		break;
	case 0x30: /* swi */
		immediate = memoryBus.Exec32(instructionPointer + 2);
		HandleInterrupt(immediate);
		SetInstructionPointer(instructionPointer + 6);
		return false;
	case 0x2e: /* xor */
		a = get_a(inst);
		b = get_b(inst);
		regs[a] ^= regs[b];
		break;
	case 0x12: /* zex.b */
		a = get_a(inst);
		b = get_b(inst);
		regs[a] = regs[b] & 0xff;
		break;
	default:
		/* illegal instruction */
		HandleBadInstruction();
		return false;
	}

	SetInstructionPointer(instructionPointer + 2);

	return true;
}

uint32_t CPU::Pop32() {

	auto stackPointer = GetStackPointer();

	auto &memoryBus = GetMemoryBus();

	auto value = memoryBus.Read32(stackPointer);

	SetStackPointer(stackPointer + 4);

	return value;
}

void CPU::Push32(uint32_t value) {

	auto stackPointer = GetStackPointer();
	if (stackPointer < 4) {
		HandleStackOverflow();
		return;
	}

	stackPointer -= 4;

	auto &memoryBus = GetMemoryBus();

	memoryBus.Write32(stackPointer, value);

	SetStackPointer(stackPointer);
}

MemoryBus &CPU::GetMemoryBus() {
	if (memoryBus == nullptr)
		throw Exception("Memory bus not assigned to CPU.");
	else
		return *memoryBus;
}

void CPU::HandleBadInstruction() {
	BadInstruction badInstruction;
	badInstruction.SetAddress(GetInstructionPointer());
	throw badInstruction;
}

void CPU::HandleBreak() {

}

void CPU::HandleStackOverflow() {
	throw Exception("Stack overflow detected.");
}

void CPU::HandleDivideByZero() {
	throw Exception("Divide by zero detected.");
}

void CPU::HandleInterrupt(uint32_t type) {

	if (interruptHandler == nullptr)
		throw Exception("Interrupt handler is not assigned to the CPU.");

	// check if interrupt is syscall
	if (type == 0x80) {
		Syscall syscall;
		syscall.SetInput(regs[2]);
		syscall.SetOutput(regs[3]);
		interruptHandler->HandleSyscall(syscall);
	} else {
		throw Exception("Interrupt type is unknown.");
	}
}

} // namespace swanson
