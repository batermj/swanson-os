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

#include <swanson/memory-section.hpp>

#include <swanson/segfault.hpp>

#include <cstring>

namespace swanson {

void MemorySection::CopyData(const void *data, uint32_t size) {
	bytes.resize(size);
	std::memcpy(bytes.data(), data, size);
}

bool MemorySection::Exists(uint32_t addr) const noexcept {
	if (addr < address)
		return false;
	else if (addr >= (address + bytes.size()))
		return false;
	else
		return true;
}

uint32_t MemorySection::Exec32(uint32_t addr) const {

	if (!executePermission)
		throw Segfault(addr);

	return Read32(addr);
}

uint16_t MemorySection::Exec16(uint32_t addr) const {

	if (!executePermission)
		throw Segfault(addr);

	return Read16(addr);
}

uint32_t MemorySection::Read32(uint32_t addr) const {

	if ((addr < address) || (!readPermission))
		throw Segfault(addr);

	uint32_t offset = addr - address;

	if ((offset + 4) > bytes.size())
		throw Segfault(addr);

	uint32_t value = 0;
	value |= ((uint32_t) bytes[offset + 0]) << 0x18;
	value |= ((uint32_t) bytes[offset + 1]) << 0x10;
	value |= ((uint32_t) bytes[offset + 2]) << 0x08;
	value |= ((uint32_t) bytes[offset + 3]) << 0x00;

	return value;
}

uint16_t MemorySection::Read16(uint32_t addr) const {

	if ((addr < address) || (!readPermission))
		throw Segfault(addr);

	uint32_t offset = addr - address;

	if ((offset + 2) > bytes.size())
		throw Segfault(addr);

	uint16_t value = 0;
	value |= ((uint16_t) bytes[offset + 0]) << 0x08;
	value |= ((uint16_t) bytes[offset + 1]) << 0x00;

	return value;
}

uint8_t MemorySection::Read8(uint32_t addr) const {

	if ((addr < address) || (!readPermission))
		throw Segfault(addr);

	uint32_t offset = addr - address;

	if ((offset + 1) > bytes.size())
		throw Segfault(addr);

	return (uint8_t) bytes[offset];
}

void MemorySection::Write32(uint32_t addr, uint32_t value) {

	if ((addr < address) || (!writePermission))
		throw Segfault(addr);

	uint32_t offset = addr - address;

	if ((offset + 4) > bytes.size())
		throw Segfault(addr);

	bytes[offset + 0] = (value >> 0x18) & 0xff;
	bytes[offset + 1] = (value >> 0x10) & 0xff;
	bytes[offset + 2] = (value >> 0x08) & 0xff;
	bytes[offset + 3] = (value >> 0x00) & 0xff;
}

void MemorySection::Write16(uint32_t addr, uint16_t value) {

	if ((addr < address) || (!writePermission))
		throw Segfault(addr);

	uint32_t offset = addr - address;

	if ((offset + 2) > bytes.size())
		throw Segfault(addr);

	bytes[offset + 0] = (value >> 0x08) & 0xff;
	bytes[offset + 1] = (value >> 0x00) & 0xff;
}

void MemorySection::Write8(uint32_t addr, uint8_t value) {

	if ((addr < address) || (!writePermission))
		throw Segfault(addr);

	uint32_t offset = addr - address;

	if ((offset + 1) > bytes.size())
		throw Segfault(addr);

	bytes[offset] = value;
}

void MemorySection::Resize(uint32_t size) {
	bytes.resize(size);
}

void MemorySection::SetAddress(uint32_t addr) noexcept {
	address = addr;
}

} // namespace swanson
