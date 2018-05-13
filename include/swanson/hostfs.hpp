/* Copyright (C) 2017 - 2018 Taylor Holberton
 *
 * This file is part of Swanson.
 *
 * Swanson is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Swanson is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Swanson.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SWANSON_FS_HOSTFS_HPP
#define SWANSON_FS_HOSTFS_HPP

#include <swanson/vfs.hpp>

#include <experimental/filesystem>

namespace std_fs = std::experimental::filesystem;

namespace swanson::hostfs {

/// This class represents a directory on the host system.
class Directory final : public vfs::Directory {
public:
	///. Default constructor
	Directory() noexcept { }
	/// Default deconstructor
	~Directory() { }
	/// Creates a file on the host system.
	/// @param name The name of the file.
	/// This parameter should not include
	/// forward or backslashes.
	/// @returns The exit code that most
	/// closely represents the exit code
	/// returned by the system call.
	ExitCode AddFile(const std::string &name) override;
	/// Creates a directory on the host system.
	/// @param name The name of the directory.
	/// This parameter should not include forward
	/// or back slashes.
	/// @returns The exit code that most
	/// closely represents the exit code
	/// returned by the system call.
	ExitCode AddDirectory(const std::string &name) override;
	/// Gets info related to the host system
	/// directory.
	std::shared_ptr<vfs::Info> GetInfo() override;
	/// This function does nothing, it
	/// is just a stub. This is because
	/// the host system directory does
	/// not need to be serialized.
	void Export(Disk &) { }
	/// This function does nothing, it
	/// is just a stub. This is because
	/// the host system directory does not
	/// need to be deserialized.
	void Import(Disk &) { }
};

/// Represents part of the file system on the
/// host. It works by 
class FS final : public vfs::FS {
	/// The root path of the file system.
	/// This is prepended to paths that
	/// are passed to the class functions.
	std_fs::path root;
public:
	/// Create a virtual file system from
	/// the host file system.
	/// @param root The root path of the
	/// file system on the host.
	/// @returns A shared pointer to the
	/// new virtual file system.
	static std::shared_ptr<vfs::FS> Create(const std::string &root);
	/// Default constructor
	FS() noexcept { };
	/// This constructor initializes a host
	/// file system class with a specified
	/// root directory.
	/// @param path The path to the root directory,
	/// contained on the host file system.
	FS(const std::string &path);
	/// Default deconstructor
	~FS() { }
	/// Creates a regular file on the host system.
	/// @param path The path of the file to create.
	/// @returns The exit code most similar to that
	/// of the system call.
	ExitCode CreateFile(const std::string &path) override;
	/// Creates a directory on the host system.
	/// @param path The path of the file to create.
	/// @returns The exit code most similar to that
	/// of the system call.
	ExitCode CreateDirectory(const std::string &path) override;
	/// Get a pointer to the root directory.
	/// @returns A pointer to the root directory.
	std::shared_ptr<vfs::Directory> GetRoot() override;
	/// Open a file on the host system.
	/// @param path The path of the file on the host system.
	/// @param stream The pointer that will be assigned the stream
	/// that will write to and read from the file on the host system.
	/// @returns The exit code most similar to that
	/// of the system call.
	ExitCode OpenFile(const std::string &path, std::shared_ptr<Stream> &stream) override;
	/// @param path The path on the host system of
	/// the new root directory.
	void SetRoot(const std::string &path);
	/// @param path The path on the host system of
	/// the new root directory.
	void SetRoot(const std_fs::path &path);
};

} // namespace swanson::hostfs

#endif /* SWANSON_FS_HOSTFS_HPP */
