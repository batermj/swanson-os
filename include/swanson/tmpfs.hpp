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

#ifndef SWANSON_TMPFS_HPP
#define SWANSON_TMPFS_HPP

#include <swanson/vfs.hpp>

#include <memory>
#include <vector>

namespace swanson::tmpfs {

/// A temporary file.
class File final : public vfs::File {
	/// Information regarding the file.
	std::shared_ptr<vfs::Info> info;
	/// The file data.
	std::vector<unsigned char> data;
public:
	/// Default constructor
	File() : info(std::make_shared<vfs::Info>()) { }
	/// Default deconstructor
	~File() { }
	/// Get information regarding the file.
	/// @returns An information class describing the file.
	std::shared_ptr<vfs::Info> GetInfo() override { return info; }
	/// Open the temporary file.
	/// @param mode The input/output mode to
	/// open the file in.
	/// @returns A stream class used to read
	/// and write data.
	std::shared_ptr<Stream> Open(uint32_t mode);
	/// This function is a stub and does nothing,
	/// since temporary files are not saved to disk.
	void Import(Disk &) override { }
	/// This function is a stub and does nothing,
	/// since temporary files are not saved to disk.
	void Export(Disk &) override { }
};

/// A temporary directory.
class Directory final : public vfs::Directory {
	/// Files contained by the directory.
	std::vector<std::shared_ptr<vfs::File>> files;
	/// Subdirectories contained by the directory.
	std::vector<std::shared_ptr<vfs::Directory>> subdirectories;
	/// Information regarding this directory.
	std::shared_ptr<vfs::Info> info;
public:
	/// Default constructor
	Directory() : info(std::make_shared<vfs::Info>()) { }
	/// Default deconstructor
	~Directory() { }
	/// Add a file to the temporary directory.
	/// @param name The name of the file.
	/// @returns ExitCode::Success on a successfull run.
	/// If the entry already exists, then ExitCode::EntryExists
	/// is returned.
	ExitCode AddFile(const std::string &name) override;
	/// Add a subdirectory to the temporary directory.
	/// @param name The name of the subdirectory.
	/// @returns ExitCode::Success on a successfull run.
	/// If the entry already exists, then ExitCode::EntryExists
	/// is returned.
	ExitCode AddDirectory(const std::string &name) override;
	/// Checks if an entry exists or not.
	/// @param name The name of the entry.
	/// @returns True if the entry exists,
	/// false if it does not.
	bool EntryExists(const std::string &name) noexcept;
	/// Get information regarding this directory.
	/// @returns Information regarding the directory.
	std::shared_ptr<vfs::Info> GetInfo() override { return info; }
	/// This function is a stub and does nothing,
	/// since temporary directories are not saved to disk.
	void Export(Disk &) override { }
	/// This function is a stub and does nothing,
	/// since temporary directories are not saved to disk.
	void Import(Disk &) override { }
};

class FS final : public vfs::FS {
	/// The root directory of the
	/// temporary file system.
	std::shared_ptr<vfs::Directory> root;
public:
	/// Default constructor
	FS() : root(std::shared_ptr<vfs::Directory>(new Directory)) { }
	/// Default deconstructor
	~FS() { }
	/// Get the root directory of
	/// the temporary file system.
	/// @returns The root directory
	/// of the temporary file system.
	std::shared_ptr<vfs::Directory> GetRoot() { return root; }
	/// Create a file in the temporary file system.
	/// @param path The path of the file to create.
	/// @returns If the function creates the file
	/// successfully, then @ref ExitCode::Success is
	/// returned. If the file exists already, then
	/// @ref ExitCode::FileExists is returned. If
	/// the parent directory does not exist,
	/// then @ref ExitCode::MissingEntry is returned.
	ExitCode CreateFile(const std::string &path);
	/// Create a directory in the temporary file system.
	/// @param path The path of the directory to create.
	/// @returns If the function creates the directory
	/// successfully, then @ref ExitCode::Success is
	/// returned. If the file exists already, then
	/// @ref ExitCode::FileExists is returned. If
	/// the parent directory does not exist,
	/// then @ref ExitCode::MissingEntry is returned.
	ExitCode CreateDirectory(const std::string &path);
};

} // namespace swanson::tmpfs

#endif /* SWANSON_TMPFS_HPP */
