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
	vfs::Info info;
	/// The file data.
	std::vector<unsigned char> data;
public:
	/// Default constructor
	File() { }
	/// Default deconstructor
	~File() { }
	/// Get information regarding the file.
	/// @returns An information class describing the file.
	vfs::Info GetInfo() const override { return info; }
	/// Set information regarding the file.
	/// @param info_ Information regarding the file.
	void SetInfo(const vfs::Info &info_) override { info = info_; }
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
	vfs::Info info;
public:
	/// Default constructor
	Directory() { }
	/// Default deconstructor
	~Directory() { }
	/// Add a file to the temporary directory.
	/// @param name The name of the file.
	void AddFile(const std::string &name);
	/// Add a subdirectory to the temporary directory.
	/// @param name The name of the subdirectory.
	void AddDirectory(const std::string &name);
	/// Get information regarding this directory.
	/// @returns Information regarding the directory.
	vfs::Info GetInfo() const { return info; }
	/// Set information regarding this directory.
	/// @param info_ The new information describing the directory.
	void SetInfo(const vfs::Info &info_) { info = info_; }
	/// This function is a stub and does nothing,
	/// since temporary directories are not saved to disk.
	void Export(Disk &) { }
	/// This function is a stub and does nothing,
	/// since temporary directories are not saved to disk.
	void Import(Disk &) { }
};

} // namespace swanson::tmpfs

#endif /* SWANSON_TMPFS_HPP */
