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

#include <swanson/exception.hpp>

namespace swanson {

bool MemorySection::Exists(uint32_t addr) const noexcept {
	if (addr < address)
		return false;
	else if (addr >= (address + bytes.size()))
		return false;
	else
		return true;
}

uint32_t MemorySection::Read32(uint32_t addr) const {

	if ((addr < address) || ((addr + 3) < (address + bytes.size())))
		throw Exception("Invalid read operation detected.");

	uint32_t value = 0;
	value |= ((uint32_t) bytes[addr + 3]) << 0x18;
	value |= ((uint32_t) bytes[addr + 2]) << 0x10;
	value |= ((uint32_t) bytes[addr + 1]) << 0x08;
	value |= ((uint32_t) bytes[addr + 0]) << 0x00;

	return value;
}

void MemorySection::Resize(uint32_t size) {
	bytes.resize(size);
}

void MemorySection::SetAddress(uint32_t addr) noexcept {
	address = addr;
}

} // namespace swanson
