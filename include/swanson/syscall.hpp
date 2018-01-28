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

#ifndef SWANSON_SYSCALL_HPP
#define SWANSON_SYSCALL_HPP

namespace swanson {

/// A system call. It consists
/// of an input and output buffer.
/// The interpretation of these buffers
/// is handled by the interrupt handler
/// and standard C library.
class Syscall final {
	/// The address of the input data.
	uint32_t inputAddress;
	/// The address of where to put the
	/// output data.
	uint32_t outputAddress;
public:
	/// Default constructor
	Syscall() noexcept : inputAddress(0), outputAddress(0) { }
	/// Default deconstructor
	~Syscall() { }
	/// Get the input address of the system call.
	/// @return The input address of the system call.
	auto GetInput() const noexcept { return inputAddress; }
	/// Get the output address of the system call.
	/// @return The output address of the system call.
	auto GetOutput() const noexcept { return outputAddress; }
	/// Set the input buffer address.
	/// @param input_ The new location
	/// of the input data.
	void SetInput(uint32_t input) noexcept { inputAddress = input; }
	/// Set the output buffer address.
	/// @param output The new address of
	/// where the output data will be put.
	void SetOutput(uint32_t output) noexcept { outputAddress = output; }
};

} // namespace swanson

#endif // SWANSON_SYSCALL_HPP
