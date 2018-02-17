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

#include <swanson/exception.hpp>
#include <swanson/elf.hpp>
#include <swanson/process.hpp>

#include <fstream>
#include <iomanip>
#include <iostream>

#include <cstdlib>
#include <cstring>

namespace {

void PrintHelp() {
	std::cout << "Usage: sandbox [options] <executable> [args]" << std::endl;
	std::cout << std::endl;
	std::cout << "Options:" << std::endl;
	std::cout << "\t-h, --help : Print this help message." << std::endl;
}

void Run(int argc, const char **argv) {

	if (argc < 1) {
		throw swanson::Exception("No executable specified.");
	}

	std::fstream file(argv[0], std::ios::in | std::ios::binary);
	if (!file.good()) {
		throw swanson::Exception("Failed to open executable.");
	}

	swanson::elf::File elfFile;

	if (elfFile.Decode(file) != 0) {
		throw swanson::Exception("Failed to decode ELF file.");
	}

	swanson::Process process;

	process.Load(elfFile);

	while (!process.Exited()) {
		process.Step(100);
	}

	std::cout << "Process exited with code, ";
	std::cout << std::hex << std::setfill('0') << std::setw(8);
	std::cout << process.GetExitCode();
	std::cout << std::endl;
}

} // namespace

int main(int argc, const char **argv) {

	int argi;

	if (argi == 1) {
		PrintHelp();
		return EXIT_FAILURE;
	}

	for (argi = 1; argi < argc; argi++) {
		if ((std::strcmp(argv[argi], "--help") == 0)
		 || (std::strcmp(argv[argi], "-h") == 0)) {
			PrintHelp();
			return EXIT_FAILURE;
		} else if (argv[argi][0] == '-') {
			std::cerr << "Unknown option '" << argv[argi] << "'" << std::endl;
			return EXIT_FAILURE;
		} else {
			break;
		}
	}

	try {
		Run(argc - argi, &argv[argi]);
	} catch (const swanson::Exception &exception) {
		std::cerr << exception.What() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
