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

#ifndef SWANSON_DISK_HPP
#define SWANSON_DISK_HPP

#include <swanson/stream.hpp>

#include <fstream>
#include <string>

namespace swanson {

/// Used to contain disk data
/// in a file on the host system.
class Disk final : public Stream {
	/// The file associated with the disk.
	std::fstream file;
public:
	/// Default constructor
	Disk() noexcept { }
	/// Default deconstructor
	~Disk() { }
	/// Open a disk file for reading
	/// and writing.
	/// @param path The path of the file.
	void Open(const std::string &path);
	/// Read data from the disk, at
	/// the current disk position.
	/// @param buf The buffer to fill
	/// with the disk data.
	/// @param bufSize The number of
	/// bytes to read from the disk.
	void Read(void *buf, uint64_t bufSize);
	/// Set the position of the next
	/// read or write operation.
	/// @param pos The position of
	/// the next IO operation.
	void SetPosition(uint64_t pos);
	/// Write data to the disk, at the
	/// current disk position.
	/// @param buf The buffer containing
	/// the data to write to disk.
	/// @param bufSize The number of bytes
	/// to write to the disk.
	void Write(const void *buf, uint64_t bufSize);
};

} // namespace swanson

#endif /* SWANSON_DISK_HPP */
