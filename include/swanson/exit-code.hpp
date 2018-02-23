// Copyright (C) 2017 - 2018 Taylor Holberton
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

#ifndef SWANSON_EXIT_CODE_HPP
#define SWANSON_EXIT_CODE_HPP

namespace swanson {

/// Enumerates a list of return codes
/// that may be returned from library
/// functions.
enum class ExitCode {
	/// The function ran successfully.
	Success,
	/// The function failed because the
	/// entry already exists.
	EntryExists,
	/// The function failed because there
	/// is no such file or directory.
	EntryMissing,
	/// The parameter passed to a library
	/// function was invalid.
	InvalidArgument
};

} // namespace swanson

#endif /* SWANSON_EXIT_CODE_HPP */
