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

#include <swanson/process.hpp>

#include <swanson/bad-instruction.hpp>
#include <swanson/elf.hpp>
#include <swanson/interrupt-handler.hpp>
#include <swanson/memory-map.hpp>
#include <swanson/memory-section.hpp>
#include <swanson/syscall.hpp>
#include <swanson/thread.hpp>

namespace {

class InterruptHandler final : public swanson::InterruptHandler {
	swanson::Process &process;
public:
	InterruptHandler(swanson::Process &process_) noexcept : process(process_) {
	}
	~InterruptHandler() {
	}
	void HandleSyscall(const swanson::Syscall &syscall) {

		auto memoryMap = process.GetMemoryMap();

		auto inputAddress = syscall.GetInput();

		auto syscallType = memoryMap->Read32(inputAddress);

		if (syscallType == 0x60) {
			HandleExit(inputAddress + 4);
		} else {
			throw swanson::Exception("System call type is unknown.");
		}
	}
protected:
	void HandleExit(uint32_t inputAddress) {

		auto memoryMap = process.GetMemoryMap();

		auto exitCode = memoryMap->Read32(inputAddress);

		process.Exit(exitCode);
	}
};

} // namespace

namespace swanson {

Process::Process() {
	memoryMap = std::make_shared<MemoryMap>();
	interruptHandler = std::make_shared<::InterruptHandler>(*this);
	entryPoint = 0;
	exited = false;
	exitCode = 0;
	// default stack size is 8MiB
	defaultStackSize = 8 * 1024 * 1024;
}

std::shared_ptr<MemoryMap> Process::GetMemoryMap() {
	return memoryMap;
}

void Process::Exit(int exitCode_) {
	exited = true;
	exitCode = exitCode_;
}

void Process::Load(const elf::File &file) {

	for (auto &segment : file)
		Load(*segment);

	auto mainThread = std::make_shared<Thread>();

	mainThread->SetInstructionPointer(file.GetEntryPoint());

	AddThread(mainThread);
}

void Process::Load(const elf::Segment &segment) {

	auto memorySection = std::make_shared<MemorySection>();
	memorySection->SetAddress(segment.GetAddress());
	memorySection->CopyData(segment.GetData(), segment.GetSize());
	memorySection->AllowRead(segment.ReadAllowed());
	memorySection->AllowWrite(segment.WriteAllowed());
	memorySection->AllowExecute(segment.ExecuteAllowed());

	memoryMap->AddSection(memorySection);
}

void Process::Step(uint32_t steps) {
	auto threadID = 0UL;
	for (auto &thread : threads) {
		try {
			thread->Step(steps);
		} catch (BadInstruction &badInstruction) {
			badInstruction.SetThreadID(threadID);
			throw;
		}
		threadID++;
		/// The exit call may have occured
		/// while executing the last thread.
		/// Check before continuing.
		if (Exited())
			break;
	}
}

void Process::AddThread(std::shared_ptr<Thread> &thread) {

	auto stack = memoryMap->AddSection(GetDefaultStackSize());
	stack->AllowRead(true);
	stack->AllowWrite(true);
	stack->AllowExecute(false);

	thread->SetMemoryBus(memoryMap);
	// TODO frame pointer?
	thread->SetStackPointer(stack->GetAddress() + stack->GetSize());
	thread->SetInterruptHandler(interruptHandler);

	threads.emplace_back(thread);
}

} // namespace swanson
