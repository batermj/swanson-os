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

#ifndef SWANSON_INTERRUPT_HANDLER_HPP
#define SWANSON_INTERRUPT_HANDLER_HPP

namespace swanson {

class Syscall;

/// Handles interrupts from processes.
class InterruptHandler {
public:
	/// Default deconstructor
	virtual ~InterruptHandler() { }
	/// Handle a system call.
	/// @param syscall The system call to handle.
	virtual void HandleSyscall(const Syscall &syscall) = 0;
};

} // namespace swanson

#endif // SWANSON_INTERRUPT_HANDLER_HPP