/* Copyright (C) 2017 Taylor Holberton
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

#ifndef SWANSON_ELF_HPP
#define SWANSON_ELF_HPP

#include <vector>
#include <memory>

#include <cstdint>

namespace swanson {

class Stream;

} // namespace swanson

namespace swanson::elf {

/// A segment within an ELF file.
class Segment {
	/// Section data.
	void *data;
	/// The number of bytes in the
	/// segment data.
	uintmax_t dataSize;
	/// The virtual address of the
	/// segment.
	uint32_t virtualAddress;
	/// Whether or not the segment
	/// has read permission.
	bool readPermission;
	/// Whether or not the segmnet
	/// has write permission.
	bool writePermission;
	/// Whether or not the segment
	/// has execute permission.
	bool executePermission;
public:
	/// Default constructor
	Segment() noexcept;
	/// Default deconstructor
	~Segment();
	/// Determine whether or not
	/// read operations are accepted.
	/// @param state If true, then read
	/// operations are accepted. If false,
	/// then read operations are NOT accepted.
	void AllowRead(bool state) noexcept {
		readPermission = state;
	}
	/// Determines whether or not
	/// write operations are accepted.
	/// @param state If true, then write
	/// operations are accepted. If false,
	/// then write operations are NOT accepted.
	void AllowWrite(bool state) noexcept {
		writePermission = state;
	}
	/// Determines whether or not
	/// execute operations are accepted.
	/// @param state If true, then execute
	/// operations are accepted. If false,
	/// execute operations are NOT accepted.
	void AllowExecute(bool state) noexcept {
		executePermission = state;
	}
	/// Set the virtual address of the segment.
	/// @param addr The new virtual address.
	void SetVirtualAddress(uint32_t addr) noexcept {
		virtualAddress = addr;
	}
	/// Read data from a stream.
	/// @param stream The stream to read the data from.
	/// @param size The number of bytes to read.
	void Read(Stream &stream, uintmax_t size);
	/// Resizes the segment.
	/// @param size The new size of the segment.
	void Resize(uintmax_t size);
	/// Get the virtual address of the segment.
	/// @returns The virtual address of the segment.
	auto GetAddr() { return virtualAddress; }
	/// Get the data pointer for the segment.
	/// @returns The segment data (nullptr if
	/// the segment doesn't currently have data.
	auto GetData() { return data; }
	/// Get the size of the segment, in bytes.
	/// This is the size of the actual segment
	/// data.
	/// @returns The size of the segment.
	auto GetSize() { return dataSize; }
};

/// Contains the contents of an ELF file that
/// are relevant to its execution.
class File {
	/// The segments contained by the ELF file.
	std::vector<std::shared_ptr<Segment>> segments;
public:
	/// Default constructor
	File() noexcept { }
	/// Default deconstructor
	~File() { }
	/// Decode an ELF file from a stream.
	/// @param stream The stream to decode
	/// the ELF file from.
	/// @returns Zero on success, a negative
	/// number on failure.
	int Decode(Stream &stream);
	/// Add an ELF segment to the
	/// ELF file.
	/// @param segment The segment
	/// to add to the file.
	void Push(std::shared_ptr<Segment> &segment);
	/// Get the number of segments in the file.
	auto GetSegmentCount() { return segments.size(); }
	/// Get a segment pointer from the file.
	auto GetSegment(uint64_t index) { return segments[index]; }
	/// Get the beginning segment iterator.
	/// Used for loops.
	/// @returns The beginning iterator.
	auto begin() const { return segments.begin(); }
	/// Get the ending segment iterator.
	/// Used for loops.
	/// @returns The ending iterator.
	auto end() const { return segments.end(); }
};

} // namespace swanson::elf

#endif /* SWANSON_ELF_HPP */
