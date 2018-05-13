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

#include <swanson/hostfs.hpp>

#include <swanson/exception.hpp>
#include <swanson/stream.hpp>

#include <fstream>
#include <iostream>

namespace {

class FileStream : public swanson::Stream {
	std::fstream file;
public:
	FileStream() { }
	~FileStream() { }
	bool Open(const std::string &path) {
		file.open(path);
		return file.good();
	}
	void Write(const void *buf, uint64_t bufSize) override {
		file.write((const char *) buf, bufSize);
	}
	void Read(void *buf, uint64_t bufSize) override {
		file.read((char *) buf, bufSize);
	}
	void SetPosition(uint64_t position) override {
		file.seekp(position);
	}
};

} // namespace

namespace swanson::hostfs {

std::shared_ptr<vfs::FS> FS::Create(const std::string &path) {

	return std::shared_ptr<vfs::FS>(new FS(path));
}

FS::FS(const std::string &path) : FS() {
	SetRoot(path);
}

ExitCode FS::CreateFile(const std::string &path) {
	(void) path;
	return ExitCode::Success;
}

ExitCode FS::CreateDirectory(const std::string &path) {
	(void) path;
	return ExitCode::Success;
}

std::shared_ptr<vfs::Directory> FS::GetRoot() {
#if 0
	auto dir = std::shared_ptr<vfs::Directory>(new Directory(root));
	return dir;
#else
	throw Exception("Not supported yet");
#endif
}

ExitCode FS::OpenFile(const std::string &path_string, std::shared_ptr<Stream> &stream) {

	std_fs::path path = path_string;

	std::string abs_path_string(root);

	auto root_skipped = false;

	for (auto name : path) {
		if (!root_skipped) {
			root_skipped = true;
			continue;
		}
		abs_path_string += "/";
		abs_path_string += name;
	}

	auto fileStream = new ::FileStream();

	if (!fileStream->Open(abs_path_string)) {
		return ExitCode::EntryMissing;
	}

	stream = std::shared_ptr<Stream>(fileStream);

	return ExitCode::Success;
}

void FS::SetRoot(const std::string &path) {
	root = path;
}

} // namespace swanson::hostfs
