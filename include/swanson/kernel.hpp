// Copyright (C) 2017 Taylor Holberton
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

#ifndef SWANSON_KERNEL_HPP
#define SWANSON_KERNEL_HPP

#include "fs/any.h"
#include "fs/ramfs/fs.h"
#include "memmap.h"
#include "partition.h"
#include "sstream.h"

#include <memory>
#include <vector>

#include <cstdint>

namespace swanson {

class Process;

/// The code returned from the kernel entry point.
/// Indicates whether or not the kernel ran without a panic.
/// @see Kernel::Main
enum class ExitCode {
	/// Kernel exited without a panic.
	Success,
	/// Kernel exited after a panic.
	Failure
};

/// The kernel, encapsulated into a single
/// class. There are no global variables, just
/// the structures found in this class.
class Kernel final {
	/// The kernel's memory map.
	memmap mmap;
	/// The disks known by the kernel.
	std::vector<disk*> disks;
	/// The partition containing the
	// operating system.
	partition root_partition;
	/// The root file system.
	any_fs root_fs;
	/// Initial ramfs file system.
	ramfs initramfs;
	/// The array of processes.
	std::vector<std::shared_ptr<Process>> processes;
public:
	/// Default constructor.
	Kernel() noexcept;
	/// Default deconstructor.
	~Kernel();
	/// Adds a disk to the kernel.
	void AddDisk(disk *disk);
	/// Adds memory that may be used by the kernel.
	void AddMemory(void *addr, uintmax_t size);
	/// Loads the initial ramdisk.
	void LoadInitRamfs(const void *addr, uintmax_t size);
	/// The kernel's entry point.
	ExitCode Main();
protected:
	/// Add a process to the kernel.
	/// The process should be loaded
	/// with an ELF file before calling
	/// this function.
	/// @param process The process to add.
	void AddProcess(std::shared_ptr<Process> &process);
};

} // namespace swanson

#endif /* SWANSON_KERNEL_HPP */
