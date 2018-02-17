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

/// greater than.
constexpr uint32_t gt = 1 << 0;

/// less than.
constexpr uint32_t lt = 1 << 1;

/// Equal to.
constexpr uint32_t eq = 1 << 2;

/// greater than (unsigned).
constexpr uint32_t gtu = 1 << 3;

/// less than (unsigned).
constexpr uint32_t ltu = 1 << 4;

/// greater than or equal to.
constexpr uint32_t ge = gt | eq;

/// greater than or equal to (unsigned).
constexpr uint32_t geu = gtu | eq;

/// less than or equal to.
constexpr uint32_t le = lt | eq;

/// less than or equal to (unsigned).
constexpr uint32_t leu = ltu | eq;

/// not equal to.
constexpr uint32_t ne = ~eq;

} // namespace swanson::conditions

#endif // SWANSON_CONDITIONS_HPP
