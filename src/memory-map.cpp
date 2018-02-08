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

#include <swanson/memory-map.hpp>

#include <swanson/memory-section.hpp>
#include <swanson/segfault.hpp>

#include "debug.h"

namespace swanson {

uint32_t MemoryMap::GetSize() const noexcept {

	uint32_t size = 0;

	for (auto &section : sections)
		size += section->GetSize();

	return size;
}

uint32_t MemoryMap::Exec32(uint32_t addr) const {

	for (const auto &section : sections) {
		if (section->Exists(addr))
			return section->Exec32(addr);
	}

	throw Segfault(addr);
}

uint16_t MemoryMap::Exec16(uint32_t addr) const {

	for (const auto &section : sections) {
		if (section->Exists(addr))
			return section->Exec16(addr);
	}

	throw Segfault(addr);
}

uint32_t MemoryMap::Read32(uint32_t addr) const {

	for (const auto &section : sections) {
		if (section->Exists(addr))
			return section->Read32(addr);
	}

	throw Segfault(addr);
}

uint16_t MemoryMap::Read16(uint32_t addr) const {

	for (const auto &section : sections) {
		if (section->Exists(addr))
			return section->Read16(addr);
	}

	throw Segfault(addr);
}

uint8_t MemoryMap::Read8(uint32_t addr) const {

	for (const auto &section : sections) {
		if (section->Exists(addr))
			return section->Read8(addr);
	}

	throw Segfault(addr);
}

void MemoryMap::Write32(uint32_t addr, uint32_t value) {

	debug("write op\n");
	debug("  addr  : %08x\n", addr);
	debug("  value : %08x\n", value);

	for (auto &section : sections) {
		if (section->Exists(addr))
			return section->Write32(addr, value);
	}

	throw Segfault(addr);
}

void MemoryMap::Write16(uint32_t addr, uint16_t value) {

	for (auto &section : sections) {
		if (section->Exists(addr))
			return section->Write16(addr, value);
	}

	throw Segfault(addr);
}

void MemoryMap::Write8(uint32_t addr, uint8_t value) {

	for (auto &section : sections) {
		if (section->Exists(addr))
			return section->Write8(addr, value);
	}

	throw Segfault(addr);
}

void MemoryMap::AddSection(std::shared_ptr<MemorySection> &section) {

	// TODO : ensure that the section does not
	// overlap with another section.

	sections.emplace_back(section);
}

std::shared_ptr<MemorySection> MemoryMap::AddSection(uint32_t size) {

	uint32_t address = 0;

	for (auto &existingSection : sections) {
		if ((address + size) > existingSection->GetAddress()) {
			address = existingSection->GetAddress() + existingSection->GetSize();
			// align to 0x2000
			address += 0x2000 - (address % 0x2000);
		}
	}

	auto section = std::make_shared<MemorySection>();
	section->Resize(size);
	section->SetAddress(address);

	AddSection(section);

	return section;
}

} // namespace swanson
