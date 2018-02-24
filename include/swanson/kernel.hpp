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

#include <swanson/disk.hpp>
#include <swanson/exit-code.hpp>

#include "fs/ramfs/fs.h"

#include <memory>
#include <vector>

#include <cstdint>

namespace swanson {

class Process;

/// The kernel, encapsulated into a single
/// class. There are no global variables, just
/// the structures found in this class.
class Kernel final {
	/// The disks known by the kernel.
	std::vector<std::shared_ptr<Disk>> disks;
	/// The array of processes.
	std::vector<std::shared_ptr<Process>> processes;
	/// The initial ram file system.
	ramfs initramfs;
public:
	/// Default constructor.
	Kernel() noexcept;
	/// Default deconstructor.
	~Kernel();
	/// Add a disk to the kernel's disk array.
	/// @param disk The disk to add to the kernel.
	void AddDisk(std::shared_ptr<Disk> disk);
	/// Loads the initial ramdisk.
	/// @param addr The address of the ramdisk data.
	/// @param size The number of bytes contained by
	/// the ramdisk.
	void LoadInitRamfs(const void *addr, uintmax_t size);
	/// The kernel's entry point.
	/// @returns The exit code of the kernel.
	/// This value is used to determine whether
	/// the host program should return succesfully
	/// or not.
	ExitCode Main();
	/// Run the processes for a specified
	/// number of instructions per thread.
	/// @param steps The instructions per
	/// thread to run in each process.
	void Step(uint32_t steps);
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
