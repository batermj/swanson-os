// Copyright (C) 2018 Taylor Holberton
//
// This file is part of Swanson.
//
// Swanson is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Swanson is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Swanson.  If not, see <http://www.gnu.org/licenses/>.

#include "memory-map-test.hpp"

#include <swanson/memory-map.hpp>
#include <swanson/memory-section.hpp>
#include <swanson/segfault.hpp>

#include "assert.h"

#include "debug.h"

namespace swanson::tests {

auto MakeSection() {
	return std::make_shared<MemorySection>();
}

auto MakeMap() {
	return std::make_shared<MemoryMap>();
}

void TestMemoryMap() {

	auto code = MakeSection();
	code->CopyData({
		0x00, 0x11, 0x22, 0x33,
		0x44, 0x55, 0x66, 0x77,
		0x88, 0x99, 0xaa, 0xbb,
		0xcc, 0xdd, 0xee, 0xff
	});
	code->Resize(0x10);
	code->SetAddress(0x00);
	code->AllowRead(true);
	code->AllowExecute(true);
	code->AllowWrite(false);

	auto rodata = MakeSection();
	rodata->CopyData({
		0xff, 0xee, 0xdd, 0xcc,
		0xbb, 0xaa, 0x99, 0x88,
		0x77, 0x66, 0x55, 0x44,
		0x33, 0x22, 0x11, 0x00
	});
	rodata->SetAddress(0x50);
	rodata->AllowRead(true);
	rodata->AllowExecute(false);
	rodata->AllowWrite(false);

	auto data = MakeSection();
	data->CopyData({
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
	});
	data->Resize(0x10);
	data->SetAddress(0x90);
	data->AllowRead(true);
	data->AllowExecute(false);
	data->AllowWrite(true);

	auto map = MakeMap();
	map->AddSection(code);
	map->AddSection(rodata);
	map->AddSection(data);

	/* ensure that exec works */

	assert(map->Exec32(0x00) == 0x00112233);
	assert(map->Exec16(0x00) == 0x0011);

	assert(map->Exec32(0x04) == 0x44556677);
	assert(map->Exec16(0x04) == 0x4455);

	assert(map->Exec32(0x05) == 0x55667788);
	assert(map->Exec16(0x05) == 0x5566);

	/* ensure that read operations work */

	assert(map->Read32(0x00) == 0x00112233);

	/* ensure that write operations will fail */

	auto faultFlag = false;

	try {
		map->Write32(0x00, 0x00);
	} catch (const Segfault &) {
		faultFlag = true;
	}

	assert(faultFlag == true);

	/* check the read-only section */

	assert(map->Read32(0x50) == 0xffeeddcc);
	assert(map->Read16(0x50) == 0xffee);
	assert(map->Read8(0x50) == 0xff);

	assert(map->Read32(0x54) == 0xbbaa9988);
	assert(map->Read16(0x54) == 0xbbaa);
	assert(map->Read8(0x54) == 0xbb);

	assert(map->Read32(0x55) == 0xaa998877);
	assert(map->Read16(0x55) == 0xaa99);
	assert(map->Read8(0x55) == 0xaa);

	faultFlag = false;

	try {
		map->Exec32(0x50);
	} catch (const Segfault &) {
		faultFlag = true;
	}

	assert(faultFlag == true);

	faultFlag = false;

	try {
		map->Write32(0x50, 0x00);
	} catch (const Segfault &) {
		faultFlag = true;
	}

	assert(faultFlag == true);

	/* check the read/write section */

	assert(map->Read32(0x90) == 0);

	map->Write32(0x90, 0x31416000);

	assert(map->Read32(0x90) == 0x31416000);
	assert(map->Read16(0x90) == 0x3141);
	assert(map->Read8(0x90) == 0x31);

	map->Write16(0x92, 0x5927);

	assert(map->Read32(0x90) == 0x31415927);
	assert(map->Read16(0x90) == 0x3141);
	assert(map->Read8(0x90) == 0x31);

	map->Write8(0x93, 0x26);

	assert(map->Read32(0x90) == 0x31415926);
	assert(map->Read16(0x90) == 0x3141);
	assert(map->Read8(0x90) == 0x31);
}

} // namespace swanson::tests
