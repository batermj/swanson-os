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

#include <swanson/thread.hpp>

#include <swanson/cpu.hpp>

namespace swanson {

Thread::Thread() noexcept : cpu(new CPU()) {

}

void Thread::SetInstructionPointer(uint32_t addr) noexcept {
	cpu->SetInstructionPointer(addr);
}

void Thread::SetStackPointer(uint32_t addr) noexcept {
	cpu->SetStackPointer(addr);
}

void Thread::SetFramePointer(uint32_t addr) noexcept {
	cpu->SetFramePointer(addr);
}

void Thread::SetMemoryBus(std::shared_ptr<MemoryBus> &memoryBus) noexcept {
	cpu->SetMemoryBus(memoryBus);
}

void Thread::Step(uint32_t steps) {
	cpu->Step(steps);
}

} // namespace swanson
