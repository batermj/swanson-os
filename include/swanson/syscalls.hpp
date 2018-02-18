// Copyright (C) 2018 Taylor Holberton
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

#ifndef SWANSON_SYSCALLS_HPP
#define SWANSON_SYSCALLS_HPP

namespace swanson {

namespace syscalls {

constexpr uint32_t exit = 1;

constexpr uint32_t open = 2;

constexpr uint32_t close = 3;

constexpr uint32_t read = 4;

constexpr uint32_t write = 5;

constexpr uint32_t lseek = 6;

constexpr uint32_t unlink = 7;

constexpr uint32_t getpid = 8;

constexpr uint32_t kill = 9;

constexpr uint32_t fstat = 10;

constexpr uint32_t sbrk = 11;

constexpr uint32_t chdir = 14;

constexpr uint32_t stat = 15;

constexpr uint32_t chmod = 16;

constexpr uint32_t utime = 17;

constexpr uint32_t time = 18;

constexpr uint32_t gettimeofday = 19;

constexpr uint32_t times = 20;

constexpr uint32_t link = 21;

} // namespace syscalls

} // namespace swanson

#endif // SWANSON_SYSCALLS_HPP
