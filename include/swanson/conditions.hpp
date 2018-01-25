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

#ifndef SWANSON_CONDITIONS_HPP
#define SWANSON_CONDITIONS_HPP

#include <cstdint>

namespace swanson::conditions {

/// Equal to.
uint32_t eq = 0x00;

/// greater than or equal to.
uint32_t ge = 0x06;

/// greater than or equal to (unsigned).
uint32_t geu = 0x08;

/// greater than.
uint32_t gt = 0x03;

/// greater than (unsigned).
uint32_t gtu = 0x05;

/// less than or equal to.
uint32_t le = 0x07;

/// less than or equal to (unsigned).
uint32_t leu = 0x09;

/// less than.
uint32_t lt = 0x02;

/// less than (unsigned).
uint32_t ltu = 0x04;

/// not equal to.
uint32_t ne = 0x01;

} // namespace swanson::conditions

#endif // SWANSON_CONDITIONS_HPP
