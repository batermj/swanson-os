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

#include <swanson/disk.hpp>

#include <swanson/exception.hpp>

namespace swanson {

void Disk::Open(const std::string &path) {
	file.open(path);
	if (!file.good())
		throw Exception("Failed to open disk file.");
}

void Disk::Read(void *buf, uint64_t bufSize) {
	file.read((char *) buf, bufSize);
}

void Disk::SetPosition(uint64_t pos) {
	file.seekp(pos);
}

void Disk::Write(const void *buf, uint64_t bufSize) {
	file.write((const char *) buf, bufSize);
}

} // namespace swanson
