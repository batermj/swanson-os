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

#include <swanson/exit-code.hpp>

#include <string>
#include <memory>

namespace swanson {

class Disk;
class Stream;

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

namespace modes {

/// Allow for read operations
/// on an open file.
constexpr uint32_t read = 0x01;

/// Allow for write operations
/// on an open file.
constexpr uint32_t write = 0x02;

/// When opening the file, write
/// operations will take place at
/// the end of the file.
constexpr uint32_t append = 0x04;

/// When opening the file in writing
/// mode, do not truncate the data in
/// the file.
constexpr uint32_t notrunc = 0x08;

} // namespace modes

/// A virtual file system file.
class File {
public:
	/// Default deconstructor
	virtual ~File() { }
	/// Get an information
	/// class that describes the file.
	/// @returns An information class
	/// that describes the file.
	virtual std::shared_ptr<Info> GetInfo() = 0;
	/// Open the file for reading, writing, or both.
	/// @param mode The mode to open the file in.
	/// @returns A stream structure that
	/// can be used to read the file data.
	virtual std::shared_ptr<Stream> Open(uint32_t mode) = 0;
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
	/// @returns ExitCode::Success on a good run. If
	/// the name of the directory already exists, then
	/// ExitCode::EntryExists is returned. If the name
	/// is an empty string, then ExitCode::InvalidArgument
	/// is returned.
	virtual ExitCode AddFile(const std::string &name) = 0;
	/// Add a subdirectory to the directory, by name.
	/// @param name The name of the directory to add.
	/// @returns ExitCode::Success on a good run. If
	/// the name of the directory already exists, then
	/// ExitCode::EntryExists is returned. If the name
	/// is an empty string, then ExitCode::InvalidArgument
	/// is returned.
	virtual ExitCode AddDirectory(const std::string &name) = 0;
	/// Get information regarding the directory.
	/// @returns An information class that describes
	/// the directory.
	virtual std::shared_ptr<Info> GetInfo() = 0;
	/// Export the directory, including the subdirectories
	/// and files, to a disk.
	/// @param disk The disk to export the directory to.
	virtual void Export(Disk &disk) = 0;
	/// Import the directory, including the subdirectories
	/// and files, from a disk.
	/// @param disk The disk to import the directory from.
	virtual void Import(Disk &disk) = 0;
};

/// File system class. Used to
/// contain the root directory as
/// well as information found in
/// the file system header.
class FS {
public:
	/// Default deconstructor
	virtual ~FS() { }
	/// Get a pointer to the root directory.
	/// @returns A pointer to the root directory.
	virtual std::shared_ptr<Directory> GetRoot() = 0;
	/// Create a regular file.
	/// @param path The path of the file.
	/// @returns @ref ExitCode::Success on a successful run.
	/// If the directory already exists, then @ref ExitCode::EntryExists
	/// is returned. If the parent directory does not exist,
	/// then @ref ExitCode::MissingEntry is returned.
	virtual ExitCode CreateFile(const std::string &path) = 0;
	/// Create a directory.
	/// @param path The path of the directory.
	/// @returns @ref ExitCode::Success on a successful run.
	/// If the directory already exists, then @ref ExitCode::EntryExists
	/// is returned. If the parent directory does not exist,
	/// then @ref ExitCode::MissingEntry is returned.
	virtual ExitCode CreateDirectory(const std::string &path) = 0;
	/// Open a file from the file system.
	/// @param path The path of the file to open.
	/// @param stream If the file can be opened, this pointer
	/// will be assigned a stream class for read and write
	/// operations for the file.
	/// @returns The exit code most similar to that
	/// of the system call.
	virtual ExitCode OpenFile(const std::string &path, std::shared_ptr<Stream> &stream) = 0;
};

} // namespace vfs

} // namespace swanson

#endif /* SWANSON_VFS_HPP */
