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
	/// The input buffer.
	std::vector<unsigned char> input;
	/// The output buffer.
	std::vector<unsigned char> output;
public:
	/// Default constructor
	Syscall() { }
	/// Default deconstructor
	~Syscall() { }
};

} // namespace swanson

#endif // SWANSON_SYSCALL_HPP
