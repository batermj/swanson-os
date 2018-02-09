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
#include <swanson/conditions.hpp>
#include <swanson/memory-map.hpp>
#include <swanson/memory-section.hpp>

#include "cpu-test.hpp"

#include <cassert>
#include <csignal>
#include <cstdint>

#include <iostream>

namespace {

class Test final {
	std::shared_ptr<swanson::MemorySection> code;
	std::shared_ptr<swanson::MemorySection> data;
	std::shared_ptr<swanson::MemoryMap> memoryMap;
	std::shared_ptr<swanson::CPU> cpu;
public:
	Test() {
		code = std::make_shared<swanson::MemorySection>();
		code->AllowExecute(true);
		code->SetAddress(0x00);

		data = std::make_shared<swanson::MemorySection>();
		data->AllowWrite(true);
		data->SetAddress(0x1000);

		memoryMap = std::make_shared<swanson::MemoryMap>();
		memoryMap->AddSection(code);
		memoryMap->AddSection(data);

		cpu = std::make_shared<swanson::CPU>();
		cpu->SetMemoryBus(memoryMap);
	}
	~Test() {

	}
	void SetCodeBytes(const std::vector<unsigned char> &bytes) {
		code->CopyData(bytes);
	}
	void SetDataBytes(const std::vector<unsigned char> &bytes) {
		data->CopyData(bytes);
	}
	void SetRegister(uint32_t index, uint32_t value) noexcept {
		cpu->SetRegister(index, value);
	}
	void SetFramePointer(uint32_t addr) noexcept {
		cpu->SetFramePointer(addr);
	}
	void SetStackPointer(uint32_t addr) noexcept {
		cpu->SetStackPointer(addr);
	}
	void SetCondition(uint32_t condition) noexcept {
		cpu->SetCondition(condition);
	}
	bool CheckInstructionPointer(uint32_t addr) const noexcept {
		return addr == cpu->GetInstructionPointer();
	}
	bool CheckRegister(uint32_t index, uint32_t value) const noexcept {
		return cpu->GetRegister(index) == value;
	}
	bool CheckStackPointer(uint32_t value) const noexcept {
		return cpu->GetStackPointer() == value;
	}
	bool CheckFramePointer(uint32_t value) const noexcept {
		return cpu->GetFramePointer() == value;
	}
	bool CheckMemory(uint32_t address, uint32_t value) const {
		return memoryMap->Read32(address) == value;
	}
	void Run(unsigned int steps = 1) {
		cpu->Step(steps);
	}
};

void TestArithmetic() {

	// add
	Test test1;
	test1.SetCodeBytes({ 0x05, 0x34 });
	test1.SetRegister(3, 9);
	test1.SetRegister(4, 22);
	test1.Run();
	assert(test1.CheckRegister(3, 31));

	Test test2;
	test2.SetCodeBytes({ 0x91, 0x07 });
	test2.SetRegister(1, 9);
	test2.Run();
	assert(test2.CheckRegister(1, 2));

	// div
	Test test3;
	test3.SetCodeBytes({ 0x31, 0x56 });
	test3.SetRegister(5, 6);
	test3.SetRegister(6, 2);
	test3.Run();
	assert(test3.CheckRegister(5, 3));

	// inc
	Test test4;
	test4.SetCodeBytes({ 0x82, 0x24 });
	test4.SetRegister(2, 2);
	test4.Run();
	assert(test4.CheckRegister(2, 0x26));
}

void TestBitwise() {

	// and
	Test test1;
	test1.SetCodeBytes({ 0x26, 0x12 });
	test1.SetRegister(1, 6);
	test1.SetRegister(2, 4);
	test1.Run();
	assert(test1.CheckRegister(1, 4));

	// ashl
	Test test2;
	test2.SetCodeBytes({ 0x28, 0x23 });
	test2.SetRegister(2, 2);
	test2.SetRegister(3, 3);
	test2.Run();
	assert(test2.CheckRegister(2, 16));

	// ashr
	Test test3;
	test3.SetCodeBytes({ 0x2d, 0x27 });
	test3.SetRegister(2, 4);
	test3.SetRegister(7, 1);
	test3.Run();
	assert(test3.CheckRegister(2, 2));
}

void TestBranching() {

	// beq - if branch isn't taken
	Test test1;
	test1.SetCodeBytes({ 0xc0, 0x04 });
	test1.SetCondition(swanson::conditions::ne);
	test1.Run();
	assert(test1.CheckInstructionPointer(0x02));

	// beq - if branch is taken
	Test test2;
	test2.SetCodeBytes({ 0xc0, 0x04 });
	test2.SetCondition(swanson::conditions::eq);
	test2.Run();
	assert(test2.CheckInstructionPointer(0x08));
}

void TestJumping() {

	// jmp
	Test test1;
	test1.SetCodeBytes({ 0x25, 0x30 });
	test1.SetRegister(3, 0x27);
	test1.Run();
	assert(test1.CheckInstructionPointer(0x27));

	// jmpa
	Test test2;
	test2.SetCodeBytes({ 0x1a, 0x00, 0x01, 0x02, 0x03, 0x04 });
	test2.Run();
	assert(test2.CheckInstructionPointer(0x01020304));

	// jsr
	Test test3;
	test3.SetFramePointer(0x1010);
	test3.SetStackPointer(0x1010);
	test3.SetCodeBytes({
		0x19, 0x30, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
	});
	test3.SetDataBytes({
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
	});
	test3.SetRegister(3, 0x08);
	test3.Run();
	assert(test3.CheckInstructionPointer(0x08));
	assert(test3.CheckStackPointer(0x1004));
	// check frame pointer
	assert(test3.CheckMemory(0x1004, 0x1010));
	// check return address
	assert(test3.CheckMemory(0x1008, 0x02));
	// check stack chain slot
	assert(test3.CheckMemory(0x100c, 0x00));

	// ret
	Test test4;
	test4.SetFramePointer(0x1004);
	test4.SetStackPointer(0x1004);
	test4.SetCodeBytes({ 0x04, 0x00 });
	test4.SetDataBytes({
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0xff, 0x00,
		0x00, 0x80, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
	});
	test4.Run();
	assert(test4.CheckInstructionPointer(0x00800000));
	assert(test4.CheckStackPointer(0x1010));
	assert(test4.CheckFramePointer(0xff00));

	// jsra
	Test test5;
	test5.SetFramePointer(0x1010);
	test5.SetStackPointer(0x1010);
	test5.SetCodeBytes({
		0x03, 0x00,
		0x01, 0x00, 0x00, 0x22
	});
	test5.SetDataBytes({
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
	});
	test5.Run();
	assert(test5.CheckInstructionPointer(0x01000022));
	assert(test5.CheckStackPointer(0x1004));
	assert(test5.CheckFramePointer(0x1004));
	// check frame pointer
	assert(test5.CheckMemory(0x1004, 0x1010));
	// check return address
	assert(test5.CheckMemory(0x1008, 0x06));
	// check stack chain slot
	assert(test5.CheckMemory(0x100c, 0x00));
}

void TestLoading() {

	// ld.b
	Test test1;
	test1.SetCodeBytes({
		0x1c, 0x12, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x42
	});
	test1.SetRegister(1, 0);
	test1.SetRegister(2, 9);
	test1.Run();
	assert(test1.CheckRegister(1, 0x42));

	// ld.l
	Test test2;
	test2.SetCodeBytes({
		0x0a, 0x34, 0x00, 0x00,
		0x00, 0x01, 0x02, 0x03,
		0x04
	});
	test2.SetRegister(3, 0);
	test2.SetRegister(4, 5);
	test2.Run();
	assert(test2.CheckRegister(3, 0x01020304));

	// ld.s
	Test test3;
	test3.SetCodeBytes({
		0x21, 0x34, 0x00, 0x00,
		0x00, 0xaa, 0xbb, 0x00
	});
	test3.SetRegister(3, 0);
	test3.SetRegister(4, 5);
	test3.Run();
	assert(test3.CheckRegister(3, 0xaabb));
}

void TestStoreOffset() {

	// sto.l
	Test test1;
	test1.SetCodeBytes({
		0x0d, 0x34, 0x00, 0x03
	});
	test1.SetDataBytes({
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
	});
	test1.SetRegister(3, 0x1001);
	test1.SetRegister(4, 0xaabbccdd);
	test1.Run();
	assert(test1.CheckMemory(0x1004, 0xaabbccdd));
}

class LoadImmediateLongTest final {
	/// The index of the register
	/// to move the immediate into.
	uint8_t a;
	/// The immediate value to move
	/// into a register.
	uint32_t immediate;
public:
	/// Default constructor
	LoadImmediateLongTest() noexcept : a(0), immediate(0) { }
	/// Default deconstructor
	~LoadImmediateLongTest() { }
	/// Set the index of the register
	/// to move the immediate value into.
	/// @param index The index of the register.
	void SetRegister(uint32_t index) { a = index; }
	/// Set the immediate value
	/// @param immediate_ The new immediate value.
	void SetImmediate(uint32_t immediate_) { immediate = immediate_; }
	/// Run the test
	void Run() const {
		Test test;
		test.SetCodeBytes({
			0x01,
			(unsigned char)((a << 4) & 0xf0),
			(unsigned char)((immediate >> 0x18) & 0xff),
			(unsigned char)((immediate >> 0x10) & 0xff),
			(unsigned char)((immediate >> 0x08) & 0xff),
			(unsigned char)((immediate >> 0x00) & 0xff),
		});
		test.Run();
		assert(test.CheckRegister(a, immediate));
	}
};

void TestLoadImmediate() {

	LoadImmediateLongTest test1;
	test1.SetRegister(3);
	test1.SetImmediate(1);
	test1.Run();

	LoadImmediateLongTest test2;
	test2.SetRegister(15);
	test2.SetImmediate(0xffffffff);
	test2.Run();
}

class LoadOffsetLongTest final {
	uint8_t a;
	uint8_t b;
	int16_t offset;
	uint32_t baseAddress;
	uint32_t value;
public:
	LoadOffsetLongTest() noexcept {
		a = 0;
		b = 0;
		offset = 0;
		baseAddress = 0;
		value = 0;
	}
	void SetRegisterA(uint8_t a_) noexcept { a = a_; }
	void SetRegisterB(uint8_t b_) noexcept { b = b_; }
	void SetBaseAddress(uint32_t base) noexcept { baseAddress = base; }
	void SetOffset(int16_t offset_) noexcept { offset = offset_; };
	void SetValue(uint32_t value_) noexcept { value = value_; }
	void Run() const {
		Test test;
		test.SetCodeBytes({
			0x0c,
			(unsigned char)(((a << 4) & 0xf0) | (b & 0x0f)),
			(unsigned char)((offset >> 0x08) & 0xff),
			(unsigned char)((offset >> 0x00) & 0xff)
		});
		test.SetRegister(a, 0);
		test.SetRegister(b, baseAddress);
		test.SetDataBytes({
			(unsigned char)((value >> 0x18) & 0xff),
			(unsigned char)((value >> 0x10) & 0xff),
			(unsigned char)((value >> 0x08) & 0xff),
			(unsigned char)((value >> 0x00) & 0xff)
		});
		test.Run();
		assert(test.CheckRegister(a, value));
	}
};

void TestLoadOffset() {

	// In these tests, the base address
	// plus the offset should result in
	// the address 0x1000. This is required
	// due to the test class always loading
	// the value there.

	LoadOffsetLongTest test1;
	test1.SetRegisterA(4);
	test1.SetRegisterB(5);
	test1.SetBaseAddress(0x1000 - 4);
	test1.SetOffset(4);
	test1.SetValue(3141);
	test1.Run();

	LoadOffsetLongTest test2;
	test2.SetRegisterA(5);
	test2.SetRegisterB(6);
	test2.SetBaseAddress(0x1000 + 6);
	test2.SetOffset(-6);
	test2.SetValue(59265);
	test2.Run();
}

#if 0

void TestMisc() {

	/* test breakpoint */
	cpu_set_pc(cpu, 0x00);
	code[0] = 0x35; /* brk */
	code[1] = 0;    /* (ignored) */
	code[2] = 0x0f; /* nop */
	assert(cpu.Step(2) == 1);
	assert(cpu->exception == SIGTRAP);
	assert(cpu_get_pc(cpu) == 2);

	cpu_recover(cpu);

	/* test cmp */
	cpu_set_pc(cpu, 0x00);
	code[0] = 0x0e;
	code[1] = 0x12;
	cpu->regs[1] = 0x01;
	cpu->regs[2] = 0x02;
	cpu->condition = CPU_CONDITION_EQ;
	cpu.Step(1);
	assert(cpu->condition == (CPU_CONDITION_LT | CPU_CONDITION_LTU));
	assert(cpu_get_pc(cpu) == 2);

	/* test gsr (get special register) */
	cpu_set_pc(cpu, 0x00);
	code[0] = 0xa1;
	code[1] = 0xff;
	cpu->regs[1] = 0;
	cpu->sregs[0xff] = 42;
	cpu.Step(1);
	assert(cpu->regs[1] == 42);
	assert(cpu_get_pc(cpu) == 2);
}

#else

void TestMisc() { }

#endif

} // namespace

namespace swanson::tests {

void TestCPU() {
	TestArithmetic();
	TestBitwise();
	TestBranching();
	TestJumping();
	TestLoading();
	TestMisc();
	TestStoreOffset();
	TestLoadImmediate();
	TestLoadOffset();
}

} // namespace swanson::tests
