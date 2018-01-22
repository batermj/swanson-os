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

#ifndef SWANSON_STREAM_HPP
#define SWANSON_STREAM_HPP

namespace swanson {

class Stream {
public:
	/// Default deconstructor
	virtual ~Stream() { }
	/// Writes a buffer to the stream.
	/// @param buf The buffer containing the data to write.
	/// @param bufSize The number of bytes to write.
	virtual void Write(const void *buf, uintmax_t bufSize) = 0;
	/// Reads data into a buffer.
	/// @param buf The buffer to put the data into.
	/// @param bufSize The number of bytes to read.
	virtual void Read(void *buf, uintmax_t bufSize) = 0;
};

} // namespace swanson

#endif /* SWANSON_STREAM_HPP */
