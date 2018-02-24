// Copyright (C) 2017 Taylor Holberton
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

#include <swanson/stream.hpp>

namespace swanson {

void Stream::DecodeBE(uint64_t &n) {

	unsigned char buf[8];

	Read(buf, 8);

	n = 0;
	n |= ((uint64_t) buf[0]) << 0x38;
	n |= ((uint64_t) buf[1]) << 0x30;
	n |= ((uint64_t) buf[2]) << 0x28;
	n |= ((uint64_t) buf[3]) << 0x20;
	n |= ((uint64_t) buf[4]) << 0x18;
	n |= ((uint64_t) buf[5]) << 0x10;
	n |= ((uint64_t) buf[6]) << 0x08;
	n |= ((uint64_t) buf[7]) << 0x00;
}

void Stream::DecodeBE(uint32_t &n){

	unsigned char buf[4];

	Read(buf, 4);

	n = 0;
	n |= ((uint64_t) buf[0]) << 0x18;
	n |= ((uint64_t) buf[1]) << 0x10;
	n |= ((uint64_t) buf[2]) << 0x08;
	n |= ((uint64_t) buf[3]) << 0x00;
}

void Stream::DecodeBE(uint16_t &n){

	unsigned char buf[2];

	Read(buf, 2);

	n = 0;
	n |= ((uint64_t) buf[0]) << 0x08;
	n |= ((uint64_t) buf[1]) << 0x00;
}

void Stream::EncodeLE(uint64_t n) {

	unsigned char buf[8];

	buf[0] = (n >> 0x00) & 0xff;
	buf[1] = (n >> 0x08) & 0xff;
	buf[2] = (n >> 0x10) & 0xff;
	buf[3] = (n >> 0x18) & 0xff;

	buf[4] = (n >> 0x20) & 0xff;
	buf[5] = (n >> 0x28) & 0xff;
	buf[6] = (n >> 0x30) & 0xff;
	buf[7] = (n >> 0x38) & 0xff;

	Write(buf, 8);
}

} // namespace swanson
