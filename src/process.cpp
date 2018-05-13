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
#include <swanson/cpu.hpp>
#include <swanson/elf.hpp>
#include <swanson/interrupt-handler.hpp>
#include <swanson/memory-map.hpp>
#include <swanson/memory-section.hpp>
#include <swanson/segfault.hpp>
#include <swanson/syscalls.hpp>
#include <swanson/thread.hpp>

#include <iostream>

namespace {

class InterruptHandler final : public swanson::InterruptHandler {
	swanson::Process &process;
public:
	InterruptHandler(swanson::Process &process_) noexcept : process(process_) {
	}
	~InterruptHandler() {
	}
	void HandleSyscall(swanson::CPU &cpu, uint32_t type) {

		auto memoryMap = process.GetMemoryMap();

		if (type == swanson::syscalls::exit) {
			HandleExit(cpu);
		} else if (type == swanson::syscalls::close) {
			HandleClose(cpu);
		} else if (type == swanson::syscalls::write) {
			HandleWrite(cpu);
		} else if (type == swanson::syscalls::execve) {
			throw swanson::Exception("Syscall 'execve' not implemented");
		} else if (type == swanson::syscalls::fork) {
			throw swanson::Exception("Syscall 'fork' not implemented");
		} else {
			throw swanson::Exception("System call type is unknown.");
		}
	}
protected:
	void HandleClose(swanson::CPU &) {
		// nothing to do here, currently.
	}
	void HandleExit(swanson::CPU &cpu) {

		// exit code in r0
		auto exitCode = cpu.GetRegister(2);

		process.Exit(exitCode);
	}
	void HandleWrite(swanson::CPU &cpu) {

		auto memoryMap = process.GetMemoryMap();

		auto fd = cpu.GetRegister(2);
		auto bufAddr = cpu.GetRegister(3);
		auto bufSize = cpu.GetRegister(4);

		if (bufSize > INT32_MAX)
			bufSize = INT32_MAX;

		if (fd == 1) {
			WriteStdout(bufAddr, bufSize);
			// return bytes written
			cpu.SetRegister(2, bufSize);
		} else if (fd == 2) {
			WriteStderr(bufAddr, bufSize);
			// return bytes written
			cpu.SetRegister(2, bufSize);
		} else {
			// not a valid file descriptor
		}
	}
	void WriteStdout(uint32_t addr, uint32_t size) {

		auto memoryMap = process.GetMemoryMap();

		for (decltype(size) i = 0; i < size; i++) {
			std::cout.put((char) memoryMap->Read8(addr + i));
		}
	}
	void WriteStderr(uint32_t addr, uint32_t size) {

		auto memoryMap = process.GetMemoryMap();

		for (decltype(size) i = 0; i < size; i++) {
			std::cerr.put((char) memoryMap->Read8(addr + i));
		}
	}
};

} // namespace

namespace swanson {

Process::Process() {

	/* create memory that will contain the
	 * command line arguments */
	argumentSection = std::make_shared<MemorySection>();
	/* argc, argv, argv[0] */
	argumentSection->Resize(4 + 4 + 4);
	argumentSection->SetAddress(0x04);
	/* argc = 0 */
	argumentSection->Write32(0x04, 0x00);
	/* argv = argv[0] */
	argumentSection->Write32(0x08, 0x0c);
	/* argv[0] = NULL */
	argumentSection->Write32(0x0c, 0x00);
	/* set permissions */
	argumentSection->AllowRead(true);
	argumentSection->AllowWrite(true);
	argumentSection->AllowExecute(false);

	/* create memory map */
	memoryMap = std::make_shared<MemoryMap>();
	/* add command line argument section */
	memoryMap->AddSection(argumentSection);

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

	// TODO : remove this, it's a hack
	// used to reserve enough space for
	// a few changes to the heap from
	// sbrk.
	if (((memorySection->GetSize() % 0x2000) != 0) && segment.WriteAllowed()) {
		memorySection->Resize(memorySection->GetSize() + (0x2000 - (memorySection->GetSize() % 0x2000)));
	}

	memoryMap->AddSection(memorySection);
}

void Process::SetRootFS(std::shared_ptr<vfs::FS> root_fs_) {
	root_fs = root_fs_;
}

void Process::Step(uint32_t steps) {
	auto threadID = 0UL;
	for (auto &thread : threads) {
		try {
			thread->Step(steps);
		} catch (Segfault &segfault) {
			segfault.SetThreadID(threadID);
			throw;
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
	thread->SetFramePointer(0x00);
	thread->SetStackPointer(stack->GetAddress() + stack->GetSize());
	thread->SetInterruptHandler(interruptHandler);

	threads.emplace_back(thread);
}

} // namespace swanson
