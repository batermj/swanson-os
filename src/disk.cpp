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

namespace {

constexpr uint64_t diskSignature = 0x6e6f736e6177537f;

} // namespace

namespace swanson {

void Disk::Format(uint64_t maxSize) {

	// Maximum size is at least 1KiB
	if (maxSize < 0x1000)
		maxSize = 0x1000;

	SetPosition(0x00);
	// Signature
	EncodeLE(diskSignature);
	// Version
	EncodeLE((uint64_t) 0x00);
	// Max Size
	EncodeLE(maxSize);
	// Current Size
	EncodeLE((uint64_t) 0x00);
}

void Disk::Create(const std::string &path) {

	std::ios_base::openmode mode = std::ios_base::out;
	mode |= std::ios_base::binary;

	file.open(path, mode);
	if (!file.good())
		throw Exception("Failed to open disk file for writing.");
}

void Disk::Open(const std::string &path) {

	std::ios_base::openmode mode = std::ios_base::in;
	mode |= std::ios_base::out;
	mode |= std::ios_base::binary;

	file.open(path, mode);
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
