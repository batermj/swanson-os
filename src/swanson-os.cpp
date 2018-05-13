/* Copyright (C) 2017 - 2018 Taylor Holberton
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
#include <swanson/hostfs.hpp>
#include <swanson/kernel.hpp>
#include <swanson/segfault.hpp>

#include "debug.h"
#include "options.h"

#ifdef SWANSON_WITH_INITRAMFS_DATA_H
#include "initramfs-data.h"
#endif /* SWANSON_WITH_INITRAMFS_DATA_H */

#include <experimental/filesystem>
#include <iomanip>
#include <iostream>

#include <cstdlib>
#include <cstring>

namespace std_fs = std::experimental::filesystem;

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

int Init(std::vector<std::string>::const_iterator begin,
         std::vector<std::string>::const_iterator end) {

	auto diskSize = 512ULL * 1024ULL;

	std::string diskPath = "disk.img";

	for (auto it = begin; it != end; it++) {

		if ((*it == "--disk") || (*it == "-d")) {

			if ((it + 1) == end)
				throw std::runtime_error("Disk path not given");

			diskPath = *(++it);

		} else if ((*it == "--size") || (*it == "-s")) {

			if ((it + 1) == end)
				throw std::runtime_error("Disk size not given");

			if (std::sscanf((++it)->c_str(), "%llu", &diskSize) != 1) {
				std::cerr << "Invalid disk size: " << *it << std::endl;
				return EXIT_FAILURE;
			}

		} else if ((*it == "--help") || (*it == "-h")) {
			return HelpInit();
		} else {
			std::cerr << "Unknown argument: " << *it << std::endl;
			return EXIT_FAILURE;
		}
	}
	
	swanson::Disk disk;
	disk.Create(diskPath);
	disk.Format(diskSize);

	return EXIT_SUCCESS;
}

int Run(std::vector<std::string>::const_iterator begin,
        std::vector<std::string>::const_iterator end) {

	auto use_hostfs = false;

	std::string hostfs_path = std_fs::current_path();

	for (auto it = begin; it != end; it++) {
		if (*it == "--use-hostfs") {
			use_hostfs = true;
		} else if (*it == "--hostfs-path") {
			if ((it + 1) == end)
				throw std::runtime_error("HostFS path not given");

			hostfs_path = *(++it);
		} else {
			std::cerr << "Unknown option: " << *it << std::endl;
			return EXIT_FAILURE;
		}
	}

	swanson::Kernel kernel;

	if (use_hostfs) {
		auto root_fs = swanson::hostfs::FS::Create(hostfs_path);
		kernel.SetRootFS(root_fs);
	} else {
		kernel.LoadInitRamfs(initramfs_data, initramfs_data_size);
	}

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
int Main(std::vector<std::string>::const_iterator begin,
         std::vector<std::string>::const_iterator end) {

	auto it = begin;

	while (it != end) {

		if ((*it)[0] != '-') {
			break;
		} else if ((*it == "--help") || (*it == "-h")) {
			return Help();
		} else {
			// Unknown option
			std::cerr << "Unknown option: " << *it << std::endl;
			return EXIT_FAILURE;
		}

		it++;
	}

	std::string cmd = "run";

	if (it != end) {
		cmd = *it;
		it++;
	}

	if (cmd == "init") {
		return Init(it, end);
	} else if (cmd == "run") {
		return Run(it, end);
	} else {
		std::cerr << "Unknown command: " << cmd << std::endl;
		return EXIT_FAILURE;
	}
}

} // namespace

int main(int argc, const char **argv) {

	std::vector<std::string> args;

	for (auto i = 1; i < argc; i++)
		args.push_back(argv[i]);

	try {
		return Main(args.begin(), args.end());
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
