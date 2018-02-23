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

#ifndef SWANSON_VFS_HPP
#define SWANSON_VFS_HPP

#include <swanson/stream.hpp>

#include <string>

namespace swanson {

class Disk;

namespace vfs {

/// Information regarding a file
/// or directory.
class Info final {
	/// The name of the entry.
	std::string name;
public:
	/// Default constructor
	Info() { }
	/// Default deconstructor
	~Info() { }
	/// Get the name of an entry.
	/// @return The name of the entry.
	const std::string &GetName() const noexcept { return name; }
	/// Set the name of an entry.
	/// @param name_ The new name of the entry.
	void SetName(const std::string &name_) { name = name_; }
	/// Set the name of an entry, without
	/// copying the string data.
	/// @param name_ The name to set the entry to.
	void SetName(std::string &&name_) noexcept { name = std::move(name_); }
};

/// A virtual file system file.
class File {
public:
	/// Default deconstructor
	virtual ~File() { }
	/// Get an information
	/// class that describes the file.
	/// @returns An information class
	/// that describes the file.
	virtual Info GetInfo() const = 0;
	/// Set the information of the file.
	/// @param info The new file information.
	virtual void SetInfo(const Info &info) = 0;
	/// Import the file from disk.
	/// @param disk The disk to import
	/// the file from.
	virtual void Import(Disk &disk) = 0;
	/// Export the file to disk.
	/// @param disk The disk to export
	/// the file to.
	virtual void Export(Disk &disk) = 0;
};

/// A virtual file system directory.
class Directory {
public:
	/// Default deconstructor
	virtual ~Directory() { }
	/// Add a file to the directory, by name.
	/// @param name The name of the file to add.
	virtual void AddFile(const std::string &name) = 0;
	/// Add a subdirectory to the directory, by name.
	/// @param name The name of the directory to add.
	virtual void AddDirectory(const std::string &name) = 0;
	/// Get information regarding the directory.
	/// @returns An information class that describes
	/// the directory.
	virtual Info GetInfo() const = 0;
	/// Set information about the directory.
	/// @param info The new directory information.
	virtual void SetInfo(const Info &info) = 0;
	/// Export the directory, including the subdirectories
	/// and files, to a disk.
	/// @param disk The disk to export the directory to.
	virtual void Export(Disk &disk) = 0;
	/// Import the directory, including the subdirectories
	/// and files, from a disk.
	/// @param disk The disk to import the directory from.
	virtual void Import(Disk &disk) = 0;
};

} // namespace vfs

} // namespace swanson

#endif /* SWANSON_VFS_HPP */
