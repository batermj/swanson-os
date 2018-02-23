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

namespace swanson::tmpfs {

void Directory::AddFile(const std::string &name) {

	vfs::Info fileInfo;
	fileInfo.SetName(name);

	auto file = std::shared_ptr<vfs::File>(new tmpfs::File);
	file->SetInfo(fileInfo);

	files.emplace_back(file);
}

void Directory::AddDirectory(const std::string &name) {

	vfs::Info dirInfo;
	dirInfo.SetName(name);

	auto dir = std::shared_ptr<vfs::Directory>(new tmpfs::Directory);
	dir->SetInfo(dirInfo);

	subdirectories.emplace_back(dir);
}

} // namespace swanson::tmpfs
