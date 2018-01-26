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
#include <swanson/memory-map.hpp>
#include <swanson/memory-section.hpp>
#include <swanson/thread.hpp>

namespace swanson {

Process::Process() {
	memoryMap = std::make_shared<MemoryMap>();
	entryPoint = 0;
	exited = false;
	exitCode = 0;
	// default stack size is 8MiB
	defaultStackSize = 8 * 1024 * 1024;
}

void Process::HandleSyscall(Syscall &syscall) {
	// TODO
	exited = true;
	exitCode = 0;
	(void) syscall;
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
	}
}

void Process::AddThread(std::shared_ptr<Thread> &thread) {

	auto stack = memoryMap->AddSection(GetDefaultStackSize());
	stack->AllowRead(true);
	stack->AllowWrite(true);
	stack->AllowExecute(false);

	thread->SetMemoryBus(memoryMap);
	// TODO : frame pointer?
	thread->SetStackPointer(stack->GetAddress());

	threads.emplace_back(thread);
}

} // namespace swanson
