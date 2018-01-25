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

#include <swanson/exception.hpp>
#include <swanson/memory-section.hpp>

namespace swanson {

uint32_t MemoryMap::Read32(uint32_t addr) const {

	for (const auto &section : sections) {
		if (section->Exists(addr))
			return section->Read32(addr);
	}

	/// TODO : standard exception class for this.
	throw Exception("Invalid read operation detected.");
}

} // namespace swanson