// Copyright (C) 2017 Taylor Holberton
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

#ifndef SWANSON_SEGFAULT_HPP
#define SWANSON_SEGFAULT_HPP

#include <swanson/exception.hpp>

#include <cstdint>

namespace swanson {

/// The base class of any exception
/// that may occur from the Swanson library.
class Segfault final : public Exception {
	/// The address that the segmentation
	/// fault occured at.
	uint32_t addr;
public:
	/// Default constructor.
	/// @param addr_ The address where the segmentation
	/// fault occured at.
	Segfault(uint32_t addr_) : Exception("Segmentation fault occured."), addr(addr_) { }
	/// Default deconstructor.
	~Segfault() { }
	/// Get the address that the segmentation fault occured.
	/// @returns The address that the segmentation fault
	/// occured at.
	uint32_t GetAddress() const noexcept { return addr; }
};

} // namespace swanson

#endif /* SWANSON_SEGFAULT_HPP */
