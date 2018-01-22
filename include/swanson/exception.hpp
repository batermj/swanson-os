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

#ifndef SWANSON_EXCEPTION_HPP
#define SWANSON_EXCEPTION_HPP

namespace swanson {

/// The base class of any exception
/// that may occur from the Swanson library.
class Exception {
	/// A human-readable description of what happened.
	const char *whatMsg;
public:
	/// The default constructor.
	/// @param whatMsg_ A description of what happened
	Exception(const char *whatMsg_) noexcept {
		if (whatMsg_ != nullptr)
			whatMsg = whatMsg_;
	}
	/// The default deconstructor.
	~Exception() {

	}
	/// Returns a description of what caused
	/// the exception.
	/// @returns A description of what caused
	/// the exception.
	const char *What() const noexcept {
		return whatMsg;
	}
};

} // namespace swanson

#endif /* SWANSON_EXCEPTION_HPP */
