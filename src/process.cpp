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

#include <swanson/elf.hpp>
#include <swanson/memory-map.hpp>
#include <swanson/memory-section.hpp>
#include <swanson/thread.hpp>

#include <debug.h>

namespace swanson {

Process::Process() noexcept : memoryMap(std::make_shared<MemoryMap>()), entryPoint(0x00) {

}

void Process::Load(const elf::File &file) {

	for (auto &segment : file)
		Load(*segment);

	auto mainThread = std::make_shared<Thread>();
	mainThread->SetInstructionPointer(file.GetEntryPoint());
	// TODO : stack pointer?
	// TODO : frame poitner?
	AddThread(mainThread);
}

void Process::Load(const elf::Segment &segment) {

	auto memorySection = std::make_shared<MemorySection>();
	memorySection->CopyData(segment.GetData(), segment.GetSize());
	memorySection->AllowRead(segment.ReadAllowed());
	memorySection->AllowWrite(segment.WriteAllowed());
	memorySection->AllowExecute(segment.ExecuteAllowed());

	memoryMap->AddSection(memorySection);
}

void Process::Step(uint32_t steps) {
	for (auto &thread : threads)
		thread->Step(steps);
}

void Process::AddThread(std::shared_ptr<Thread> &thread) {
	threads.emplace_back(thread);
}

} // namespace swanson
