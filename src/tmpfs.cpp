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

#include <swanson/tmpfs.hpp>

#include <swanson/stream.hpp>

namespace {

/// A stream used for reading and
/// writing to an open temporary file.
class TmpFileStream final : public swanson::Stream {
	std::vector<unsigned char> &data;
	uint64_t offset;
	uint32_t mode;
public:
	TmpFileStream(std::vector<unsigned char> &data_, uint32_t mode_) : data(data_), mode(mode_) {
		offset = 0;
	}
	~TmpFileStream() {

	}
	void Read(void *buf, uint64_t bufSize) override {

		// TODO : this should indicate
		// that the read failed.
		if (!(mode & swanson::vfs::modes::read))
			return;

		auto buf8 = (unsigned char *) buf;

		auto i = offset;

		while ((i < data.size()) && ((i - offset) < bufSize)) {
			buf8[i - offset] = data[i];
			i++;
		}

		offset = i;
	}
	void SetPosition(uint64_t pos) override {

		if (pos >= data.size())
			pos = data.size();

		offset = pos;
	}
	void Write(const void *buf, uint64_t bufSize) override {

		if (!(mode & swanson::vfs::modes::write))
			return;

		auto buf8 = (const unsigned char *) buf;

		auto i = offset;

		while ((i < data.size()) && ((i - offset) < bufSize)) {
			data[i] = buf8[i - offset];
			i++;
		}

		offset = i;
	}
};

} // namespace

namespace swanson::tmpfs {

std::shared_ptr<Stream> File::Open(uint32_t mode) {

	return std::shared_ptr<Stream>(new TmpFileStream(data, mode));
}

ExitCode Directory::AddFile(const std::string &name) {

	if (EntryExists(name))
		return ExitCode::EntryExists;

	auto file = std::shared_ptr<vfs::File>(new tmpfs::File);

	auto fileInfo = file->GetInfo();

	fileInfo->SetName(name);

	files.emplace_back(file);

	return ExitCode::Success;
}

ExitCode Directory::AddDirectory(const std::string &name) {

	if (EntryExists(name))
		return ExitCode::EntryExists;

	auto dir = std::shared_ptr<vfs::Directory>(new tmpfs::Directory);

	auto dirInfo = dir->GetInfo();

	dirInfo->SetName(name);

	subdirectories.emplace_back(dir);

	return ExitCode::Success;
}

bool Directory::EntryExists(const std::string &name) noexcept {

	for (auto file : files) {
		auto fileInfo = file->GetInfo();
		if (fileInfo->GetName() == name)
			return true;
	}

	for (auto dir : subdirectories) {
		auto dirInfo = dir->GetInfo();
		if (info->GetName() == name)
			return true;
	}

	return false;
}

} // namespace swanson::tmpfs
