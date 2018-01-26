/* Copyright (C) 2017 Taylor Holberton
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

#include <swanson/bad-instruction.hpp>
#include <swanson/kernel.hpp>
#include <swanson/segfault.hpp>

#include "debug.h"
#include "options.h"

#ifdef SWANSON_WITH_INITRAMFS_DATA_H
#include "initramfs-data.h"
#endif /* SWANSON_WITH_INITRAMFS_DATA_H */

#include <iomanip>
#include <iostream>

#include <cstdlib>
#include <cstring>

namespace {

/// A section of memory, described
/// by an address and size.
struct MemorySection {
	/// The address of the memory section.
	void *addr;
	/// The number of bytes allocated by
	/// the section.
	uintmax_t size;
};

/// The hosted environment for the
/// kernel to run in. Contains the
/// memory and the disks that the
/// kernel will use to run the operating
/// system.
class KernelHost {
	/// The memory sections that will be
	/// used by the kernel.
	std::vector<MemorySection> memorySections;
	/// The kernel that is being hosted.
	swanson::Kernel &kernel;
public:
	/// Default constructor
	KernelHost(swanson::Kernel &kernel_) noexcept : kernel(kernel_) {

	}
	/// Default constructor.
	~KernelHost() {
		for (auto &memorySection : memorySections)
			std::free(memorySection.addr);
	}
	/// Add a section of memory for the kernel to use.
	/// @param size The number of bytes that the memory
	/// section will contain.
	void AddMemorySection(uintmax_t size) {

		MemorySection memorySection;
		memorySection.addr = std::malloc(size);
		memorySection.size = size;
		memorySections.push_back(memorySection);

		kernel.AddMemory(memorySection.addr, memorySection.size);
	}
	/// Read the command line arguments
	/// and adjust the kernel host accordingly.
	/// @param argc The number of arguments in the
	/// argument array.
	/// @param argv The argument array. This does not
	/// include the name of the program, which is typically
	/// the first argument in a standard entry point.
	void ReadArgs(int argc, const char **argv) {

		options opts;

		options_init(&opts);

		auto err = options_parse_args(&opts, argc, argv);
		if (err != 0) {
			options_free(&opts);
			throw swanson::Exception("Failed to parse command line arguments.");
		}

		ReadOptions(opts);

		options_free(&opts);
	}
	/// Read the options that were passed via
	/// the command line.
	/// @param opts The options structure to read.
	void ReadOptions(options &opts) {

		auto memorySize = options_get_memory(&opts);

		AddMemorySection(memorySize);

		auto diskCount = options_get_disk_count(&opts);
		for (decltype(diskCount) i = 0; i < diskCount; i++) {

			auto disk = options_get_disk(&opts, i);
			if (disk == nullptr)
				continue;

			kernel.AddDisk(disk);
		}
	}
};

/// The entry point of the program.
/// The entry point is put here so that
/// exception handling could be implemented
/// by the caller.
int Main(int argc, const char **argv) {

	swanson::Kernel kernel;

	::KernelHost kernelHost(kernel);

	kernelHost.ReadArgs(argc - 1, &argv[1]);

	kernel.LoadInitRamfs(initramfs_data, initramfs_data_size);

	auto exitCode = kernel.Main();

	if (exitCode == swanson::ExitCode::Success)
		return EXIT_SUCCESS;
	else
		return EXIT_FAILURE;
}

} // namespace

int main(int argc, const char **argv) {
	try {
		return Main(argc, argv);
	} catch (const swanson::Segfault &segfault) {
		std::cerr << "An uncaught segmentation fault occured at ";
		std::cerr << std::hex << std::setw(8) << std::setfill('0');
		std::cerr << segfault.GetAddress() << "." << std::endl;
		return EXIT_FAILURE;
	} catch (const swanson::BadInstruction &badInstruction) {
		std::cerr << "An uncaught illegal instruction was encounted." << std::endl;
		std::cerr << std::hex << std::setw(8) << std::setfill('0');
		std::cerr << "Process ID: " << badInstruction.GetProcessID() << std::endl;
		std::cerr << "Thread ID:  " << badInstruction.GetProcessID() << std::endl;
		std::cerr << "Address:    " << badInstruction.GetAddress() << std::endl;
		return EXIT_FAILURE;
	} catch (const swanson::Exception &exception) {
		std::cerr << exception.What() << std::endl;
		return EXIT_FAILURE;
	} catch (...) {
		return EXIT_FAILURE;
	}
}
