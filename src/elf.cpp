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

#include <swanson/elf.hpp>

#include <swanson/exception.hpp>
#include <swanson/stream.hpp>

#include <cstdlib>
#include <cstring>

namespace swanson::elf {

Segment::Segment() noexcept {
	data = nullptr;
	dataSize = 0;
	readPermission = true;
	writePermission = true;
	executePermission = false;
}

Segment::~Segment() {
	free(data);
	data = nullptr;
	dataSize = 0;
}

void Segment::Read(Stream &stream, uint64_t size) {

	if (dataSize < size)
		Resize(size);

	stream.Read(data, size);
}

void Segment::Resize(uint64_t size) {

	auto tmp = std::realloc(data, size);
	if (tmp == nullptr)
		throw Exception("Failed to allocate memory.");

	data = tmp;
	dataSize = size;
}

int File::Decode(Stream &stream) {

	/* Verify ELF signature */
	unsigned char signature[4];
	stream.SetPosition(0x00);
	stream.Read(signature, 4);
	if ((signature[0] != 0x7f)
	 || (signature[1] != 'E')
	 || (signature[2] != 'L')
	 || (signature[3] != 'F'))
		return -1;

	/* Verify class is 32-bit */
	uint8_t elf_class;
	stream.SetPosition(0x04);
	stream.Read(&elf_class, 1);
	if (elf_class != 1) {
		/* 32-bit: 1 */
		/* 64-bit: 2 */
		return -1;
	}

	/* Verify endianness is big-endian */
	uint8_t endianness;
	stream.SetPosition(0x05);
	stream.Read(&endianness, 1);
	if (endianness != 2) {
		/* 1 : little endian */
		/* 2 : big endian */
		return -1;
	}

	/* Verify machine type is Moxie */
	uint16_t machine_type;
	stream.SetPosition(0x12);
	stream.DecodeBE(machine_type);
	if (machine_type != 0xdf)
		return -1;

	/* Get entry point of the program. */
	stream.SetPosition(0x18);
	stream.DecodeBE(entryPoint);

	/* Get program header offset */
	uint32_t ph_offset;
	stream.SetPosition(0x1c);
	stream.DecodeBE(ph_offset);

	/* Verify program header size */
	uint16_t ph_size;
	stream.SetPosition(0x2a);
	stream.DecodeBE(ph_size);

	/* Get program header count */
	uint16_t ph_count;
	stream.SetPosition(0x2c);
	stream.DecodeBE(ph_count);

	/* Read program headers */
	for (decltype(ph_count) ph_index = 0; ph_index < ph_count; ph_index++) {

		auto segment = std::make_shared<Segment>();

		stream.SetPosition(ph_offset + (ph_index * ph_size));

		uint32_t segmentType;
		stream.DecodeBE(segmentType);
		if (segmentType == 0x01) {
			/* LOAD : Segment contains loadable data */
		} else if (segmentType == 0x02) {
			/* DYNAMIC : Segment contains dynamic linking information */
			continue;
		} else if (segmentType == 0x03) {
			/* INTERP : Segment contains path to interpreter */
			continue;
		} else {
			continue;
		}

		uint32_t segmentFileOffset;
		stream.DecodeBE(segmentFileOffset);

		uint32_t segmentAddr;
		stream.DecodeBE(segmentAddr);

		/* physical address is read but ignored afterwards */
		uint32_t paddr;
		stream.DecodeBE(paddr);

		uint32_t segmentFileSize;
		stream.DecodeBE(segmentFileSize);

		uint32_t segmentMemorySize;
		stream.DecodeBE(segmentMemorySize);

		uint32_t segmentFlags;
		stream.DecodeBE(segmentFlags);

		if (segmentFlags & 0x01)
			segment->AllowExecute(true);
		else
			segment->AllowExecute(false);

		if (segmentFlags & 0x02)
			segment->AllowWrite(true);
		else
			segment->AllowWrite(false);

		if (segmentFlags & 0x04)
			segment->AllowRead(true);
		else
			segment->AllowRead(false);

		/* if the segment's file size is greater than
		 * the size it occupies in memory, this segment
		 * may be corrupt on file */
		if (segmentFileSize > segmentMemorySize)
			continue;

		segment->Resize(segmentMemorySize);
		segment->SetVirtualAddress(segmentAddr);

		stream.SetPosition(segmentFileOffset);

		segment->Read(stream, segmentFileSize);

		Push(segment);
	}

	return 0;
}

void File::Push(std::shared_ptr<Segment> &segment) {
	segments.emplace_back(segment);
}

} // namespace elf
