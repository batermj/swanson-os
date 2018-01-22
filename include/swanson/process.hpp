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

#ifndef SWANSON_PROCESS_HPP
#define SWANSON_PROCESS_HPP

#include "cpu.h"
#include "path.h"

#include <memory>
#include <vector>

#include <cstdint>

namespace swanson {

class Thread;
class MemoryBus;
class Path;
struct elf;

class Process final {
	/// Used to read from and write to memory.
	std::shared_ptr<MemoryBus> memoryBus;
	/// The array of threads currently used
	/// by the process.
	std::vector<std::shared_ptr<Thread>> threads;
	/// The array of process IDs that were
	/// started by this process.
	std::vector<int> childProcesses;
	/// The root path of the process.
	std::shared_ptr<Path> root;
	/// The current working directory
	/// of the process.
	std::shared_ptr<Path> cwd;
	/// Process identification number.
	int pid;
public:
	/// Default constructor
	Process() noexcept : pid(0) { }
	/// Default deconstructor
	~Process() { }
	/// Kill the process.
	void Kill();
	/// Load an ELF file into the process.
	/// @param elfFile The ELF file to load.
	void Load(const elf &elfFile);
	/// Allow each of the threads in the
	/// process to run for a specified number
	/// of instructions.
	/// @param steps The number of instructions
	/// to execute on each thread.
	void Step(uintmax_t steps);
};

} // namespace swanson

#endif /* SWANSON_PROCESS_HPP */
