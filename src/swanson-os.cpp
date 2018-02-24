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
#include <swanson/disk.hpp>
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

int Help() {
	std::cout << "Usage: swanson-os [options] [cmd]" << std::endl;
	std::cout << std::endl;
	std::cout << "Options:" << std::endl;
	std::cout << "\t-h, --help : Print this help message." << std::endl;
	std::cout << std::endl;
	std::cout << "Commands:" << std::endl;
	std::cout << "\tinit : Initialize a Swanson system." << std::endl;
	std::cout << "\trun  : Run the system. This is the default command." << std::endl;
	return EXIT_FAILURE;
}

int HelpInit() {
	std::cout << "Options:" << std::endl;
	std::cout << "\t-d, --disk PATH : Specify the path of the disk to use." << std::endl;
	std::cout << "\t-s, --size SIZE : Specify the max size of the disk." << std::endl;
	return EXIT_FAILURE;
}

int Init(int argc, const char **argv) {

	auto diskSize = 512ULL * 1024ULL;

	std::string diskPath = "disk.img";

	for (decltype(argc) i = 0; i < argc; i++) {
		if ((std::strcmp(argv[i], "--disk") == 0)
		 || (std::strcmp(argv[i], "-d") == 0)) {
			diskPath = argv[i + 1];
			i++;
		} else if ((std::strcmp(argv[i], "--size") == 0)
		        || (std::strcmp(argv[i], "-s") == 0)) {
			if (std::sscanf(argv[i + 1], "%llu", &diskSize) != 1) {
				std::cerr << "Invalid disk size: " << argv[i + 1] << std::endl;
				return EXIT_FAILURE;
			}
		} else if ((std::strcmp(argv[i], "--help") == 0)
		        || (std::strcmp(argv[i], "-h") == 0)) {
			return HelpInit();
		} else {
			std::cerr << "Unknown argument: " << argv[i] << std::endl;
			return EXIT_FAILURE;
		}
	}
	
	swanson::Disk disk;
	disk.Create(diskPath);
	disk.Format(diskSize);

	return EXIT_SUCCESS;
}

int Run(int, const char **) {

	swanson::Kernel kernel;

	kernel.LoadInitRamfs(initramfs_data, initramfs_data_size);

	auto exitCode = kernel.Main();

	if (exitCode == swanson::ExitCode::Success)
		return EXIT_SUCCESS;
	else
		return EXIT_FAILURE;
}

/// The entry point of the program.
/// The entry point is put here so that
/// exception handling could be implemented
/// by the caller.
int Main(int argc, const char **argv) {

	// This is the argument index
	auto i = 1;

	while (i < argc) {

		if (argv[i][0] != '-') {
			// Not an option
			break;
		} else if ((std::strcmp(argv[i], "--help") == 0)
		        || (std::strcmp(argv[i], "-h") == 0)) {
			return Help();
		} else {
			// Unknown option
			std::cerr << "Unknown option: " << argv[i] << std::endl;
			return EXIT_FAILURE;
		}

		i++;
	}

	auto cmd = argv[i];
	if (cmd == NULL)
		cmd = "run";

	if (std::strcmp(cmd, "init") == 0) {
		return Init(argc - (i + 1), &argv[i + 1]);
	} else if (std::strcmp(cmd, "run") == 0) {
		return Run(argc - (i + 1), &argv[i + 1]);
	} else {
		std::cerr << "Unknown command: " << cmd << std::endl;
		return EXIT_FAILURE;
	}
}

} // namespace

int main(int argc, const char **argv) {
	try {
		return Main(argc, argv);
	} catch (const swanson::Segfault &segfault) {
		std::cerr << "An uncaught segmentation fault occured." << std::endl;
		std::cerr << std::hex << std::setw(8) << std::setfill('0');
		std::cerr << "Instruction Pointer: " << segfault.GetInstructionPointer() << std::endl;
		std::cerr << "Fault Address:       " << segfault.GetAddress() << std::endl;
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
