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

#include "elf-test.hpp"

#include <swanson/elf.hpp>
#include <swanson/exception.hpp>
#include <swanson/stream.hpp>

#include "assert.h"
#include "elf.h"
#include "elf-data.h"
#include "rstream.h"

#include <cstring>

namespace {

const unsigned char segment_data[] = {
	0x7f, 0x45, 0x4c, 0x46, 0x01, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x02, 0x00, 0xdf, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x34,
	0x00, 0x00, 0x01, 0x68, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 0x00, 0x20, 0x00, 0x01, 0x00, 0x28,
	0x00, 0x06, 0x00, 0x05, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xac,
	0x00, 0x00, 0x0f, 0xac, 0x00, 0x00, 0x00, 0x58, 0x00, 0x00, 0x00, 0x58, 0x00, 0x00, 0x00, 0x05,
	0x00, 0x00, 0x00, 0x01, 0x0f, 0x00, 0x04, 0x00
};

class MockStream final : public swanson::Stream {
	uint64_t offset;
public:
	/// Default constructor
	MockStream() noexcept : offset(0) { }
	/// Default deconstructor
	~MockStream() { }
	/// Reads memory from the ELF file.
	void Read(void *buf, uint64_t bufSize) {

		if ((offset + bufSize) > elf_data_size)
			throw swanson::Exception("Buffer overflow detected.");

		std::memcpy(buf, (const char *) elf_data + offset, bufSize);

		offset += bufSize;
	}
	/// Set the position of the mock stream.
	void SetPosition(uint64_t position) {
		if (position < elf_data_size)
			offset = position;
	}
	/// Just a stub, not used by the test.
	void Write(const void *, uint64_t) {
		throw swanson::Exception("Write operations not permitted.");
	}
};

void TestDecode() {

	::MockStream mockStream;

	swanson::elf::File file;
	auto err = file.Decode(mockStream);
	assert(err == 0);
	assert(file.GetSegmentCount() == 1);
	auto segment = file.GetSegment(0);
	assert(segment->GetSize() == 88);
	assert(std::memcmp(segment->GetData(), segment_data, 88) == 0);
}

} // namespace

namespace swanson::tests {

void TestELF() {
	::TestDecode();
}

} // namespace swanson::tests
