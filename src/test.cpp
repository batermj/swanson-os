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

#include "test.hpp"

#include "elf-test.hpp"

#include "cpu-test.h"
#include "crc32-test.h"
#include "gpt-test.h"
#include "memmap-test.h"
#include "options-test.h"
#include "path-test.h"

#include <stdlib.h>

int
main(void) {
	swanson::tests::RunTests();
	return EXIT_SUCCESS;
}

namespace swanson::tests {

void RunTests() {
	// C++ tests
	TestELF();
	// Standard C tests
	cpu_test();
	crc32_test();
	gpt_test();
	memmap_test();
	options_test();
	path_test();
}

} // namespace swanson::tests
